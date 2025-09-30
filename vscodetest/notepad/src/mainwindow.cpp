#include "mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileInfo>
#include <QDateTime>
#include <QStatusBar>
#include <QInputDialog>
#include <QContextMenuEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupMenuBar();
    setupContextMenu();

    // 连接信号和槽
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addNewTab);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteCurrentTab);
    connect(tabsList, &QListWidget::currentRowChanged, this, &MainWindow::onTabSelectionChanged);
    connect(tabsList, &QListWidget::itemChanged, this, &MainWindow::onTabTitleChanged);
    connect(tabsList, &QListWidget::itemDoubleClicked, this, &MainWindow::onTabDoubleClicked);
    connect(contentEdit, &QTextEdit::textChanged, this, &MainWindow::onContentChanged);

    // 连接菜单动作
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // 连接标签操作动作
    connect(renameTabAction, &QAction::triggered, this, &MainWindow::renameTab);
    connect(duplicateTabAction, &QAction::triggered, this, &MainWindow::duplicateTab);
    connect(deleteTabAction, &QAction::triggered, this, &MainWindow::deleteCurrentTab);

    // 设置自动保存定时器（每30秒自动保存）
    autoSaveTimer = new QTimer(this);
    connect(autoSaveTimer, &QTimer::timeout, this, &MainWindow::autoSave);
    autoSaveTimer->start(30000); // 30秒

    // 添加一个初始标签
    addNewTab();
    updateWindowTitle();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("Qt6.6 记事本应用程序");
    setMinimumSize(800, 600);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建状态栏
    statusBar()->showMessage("就绪");

    // 创建主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // 创建分割器
    mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    mainLayout->addWidget(mainSplitter);

    // 设置左侧面板
    leftPanel = new QWidget();
    leftLayout = new QVBoxLayout(leftPanel);

    tabsLabel = new QLabel("标签列表 (双击或右键点击编辑)");
    tabsLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin: 10px;");

    tabsList = new QListWidget();
    tabsList->setMinimumWidth(250);
    tabsList->setSelectionMode(QListWidget::SingleSelection);

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("添加标签");
    deleteButton = new QPushButton("删除标签");

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);

    leftLayout->addWidget(tabsLabel);
    leftLayout->addWidget(tabsList);
    leftLayout->addLayout(buttonLayout);

    // 设置右侧面板
    rightPanel = new QWidget();
    rightLayout = new QVBoxLayout(rightPanel);

    contentLabel = new QLabel("内容编辑");
    contentLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin: 10px;");

    contentEdit = new QTextEdit();
    contentEdit->setPlaceholderText("在这里输入内容...");

    rightLayout->addWidget(contentLabel);
    rightLayout->addWidget(contentEdit);

    // 添加到分割器
    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(rightPanel);

    // 设置分割器比例
    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 3);
}

void MainWindow::setupMenuBar()
{
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // 文件菜单
    fileMenu = menuBar->addMenu("文件");

    newAction = new QAction("新建", this);
    newAction->setShortcut(QKeySequence::New);

    openAction = new QAction("打开", this);
    openAction->setShortcut(QKeySequence::Open);

    saveAction = new QAction("保存", this);
    saveAction->setShortcut(QKeySequence::Save);

    saveAsAction = new QAction("另存为", this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);

    exitAction = new QAction("退出", this);
    exitAction->setShortcut(QKeySequence::Quit);

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
}

void MainWindow::setupContextMenu()
{
    // 创建标签上下文菜单
    tabContextMenu = new QMenu(this);

    renameTabAction = new QAction("重命名", this);
    renameTabAction->setShortcut(QKeySequence(Qt::Key_F2));

    duplicateTabAction = new QAction("复制标签", this);
    duplicateTabAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));

    deleteTabAction = new QAction("删除标签", this);
    deleteTabAction->setShortcut(QKeySequence(Qt::Key_Delete));

    tabContextMenu->addAction(renameTabAction);
    tabContextMenu->addAction(duplicateTabAction);
    tabContextMenu->addSeparator();
    tabContextMenu->addAction(deleteTabAction);
}

void MainWindow::addNewTab()
{
    auto newItem = std::make_shared<NotepadItem>(QString("标签 %1").arg(notepadItems.size() + 1));
    notepadItems.push_back(newItem);

    QListWidgetItem* item = new QListWidgetItem(newItem->getTitle());
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    tabsList->addItem(item);
    tabsList->setCurrentRow(tabsList->count() - 1);

    setModified(true);
    statusBar()->showMessage("已添加新标签", 2000);
}

void MainWindow::deleteCurrentTab()
{
    int currentRow = tabsList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(notepadItems.size())) {
        if (notepadItems.size() > 1) {
            QString tabTitle = notepadItems[currentRow]->getTitle();
            auto result = QMessageBox::question(this, "确认删除",
                                                QString("确定要删除标签 \"%1\" 吗？").arg(tabTitle),
                                                QMessageBox::Yes | QMessageBox::No);
            if (result == QMessageBox::Yes) {
                notepadItems.erase(notepadItems.begin() + currentRow);
                delete tabsList->takeItem(currentRow);

                // 如果删除后还有标签，选择第一个
                if (!notepadItems.empty()) {
                    tabsList->setCurrentRow(0);
                } else {
                    // 如果没有标签了，清空内容显示
                    contentEdit->clear();
                    currentItemId = -1;
                }

                setModified(true);
                statusBar()->showMessage("已删除标签", 2000);
            }
        } else {
            QMessageBox::warning(this, "警告", "至少需要保留一个标签！");
        }
    }
}

void MainWindow::onTabSelectionChanged()
{
    int currentRow = tabsList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(notepadItems.size())) {
        updateContentDisplay();
    }
}

void MainWindow::onTabTitleChanged(QListWidgetItem* item)
{
    int row = tabsList->row(item);
    if (row >= 0 && row < static_cast<int>(notepadItems.size())) {
        QString newTitle = item->text().trimmed();
        if (newTitle.isEmpty()) {
            // 如果标题为空，恢复原来的标题
            item->setText(notepadItems[row]->getTitle());
            QMessageBox::warning(this, "错误", "标签标题不能为空！");
        } else {
            notepadItems[row]->setTitle(newTitle);
            setModified(true);
            statusBar()->showMessage("已更新标签标题", 1500);
        }
    }
}

void MainWindow::onTabDoubleClicked(QListWidgetItem* item)
{
    // 双击标签进入编辑模式
    tabsList->editItem(item);
}

void MainWindow::onContentChanged()
{
    if (currentItemId != -1) {
        // 找到当前选中的项目并更新内容
        for (auto& item : notepadItems) {
            if (item->getId() == currentItemId) {
                item->setContent(contentEdit->toPlainText());
                setModified(true);
                break;
            }
        }
    }
}

void MainWindow::updateContentDisplay()
{
    int currentRow = tabsList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(notepadItems.size())) {
        auto& currentItem = notepadItems[currentRow];
        currentItemId = currentItem->getId();

        // 暂时断开信号以避免递归调用
        contentEdit->blockSignals(true);
        contentEdit->setPlainText(currentItem->getContent());
        contentEdit->blockSignals(false);
    }
}

void MainWindow::setModified(bool modified)
{
    isModified = modified;
    updateWindowTitle();
}

void MainWindow::updateWindowTitle()
{
    QString title = "Qt6.6 记事本";
    if (!currentFilePath.isEmpty()) {
        title += " - " + QFileInfo(currentFilePath).fileName();
    }
    if (isModified) {
        title += " [已修改]";
    }
    setWindowTitle(title);
}

bool MainWindow::maybeSave()
{
    if (isModified) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, "文档已修改",
                                   "文档已被修改，是否保存更改？",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            saveFile();
            return true;
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        notepadItems.clear();
        tabsList->clear();
        contentEdit->clear();
        currentFilePath.clear();
        setModified(false);

        // 添加一个初始标签
        addNewTab();
        statusBar()->showMessage("已创建新文件");
    }
}

void MainWindow::openFile()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        "打开记事本文件", "", "记事本文件 (*.json);;所有文件 (*)");
        if (!fileName.isEmpty()) {
            loadFromFile(fileName);
        }
    }
}

void MainWindow::saveFile()
{
    if (currentFilePath.isEmpty()) {
        saveAsFile();
    } else {
        saveToFile(currentFilePath);
    }
}

void MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "另存为记事本文件", "", "记事本文件 (*.json);;所有文件 (*)");
    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".json", Qt::CaseInsensitive)) {
        fileName += ".json";
    }

    saveToFile(fileName);
}

void MainWindow::autoSave()
{
    if (isModified && !currentFilePath.isEmpty()) {
        saveToFile(currentFilePath);
        statusBar()->showMessage("已自动保存", 2000);
    }
}

void MainWindow::loadFromFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "错误", "无法打开文件: " + file.errorString());
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));

    if (doc.isNull()) {
        QMessageBox::warning(this, "错误", "文件格式错误");
        return;
    }

    QJsonObject root = doc.object();
    if (!root.contains("notepadItems") || !root["notepadItems"].isArray()) {
        QMessageBox::warning(this, "错误", "文件格式错误：缺少notepadItems数组");
        return;
    }

    // 清空当前数据
    notepadItems.clear();
    tabsList->clear();

    // 加载数据
    QJsonArray itemsArray = root["notepadItems"].toArray();
    for (const QJsonValue& value : itemsArray) {
        if (value.isObject()) {
            auto item = std::make_shared<NotepadItem>();
            item->fromJson(value.toObject());
            notepadItems.push_back(item);

            QListWidgetItem* listItem = new QListWidgetItem(item->getTitle());
            listItem->setFlags(listItem->flags() | Qt::ItemIsEditable);
            tabsList->addItem(listItem);
        }
    }

    if (!notepadItems.empty()) {
        tabsList->setCurrentRow(0);
    }

    currentFilePath = fileName;
    setModified(false);
    statusBar()->showMessage("文件已加载: " + fileName, 3000);
}

void MainWindow::saveToFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "错误", "无法保存文件: " + file.errorString());
        return;
    }

    QJsonObject root;
    QJsonArray itemsArray;

    for (const auto& item : notepadItems) {
        itemsArray.append(item->toJson());
    }

    root["notepadItems"] = itemsArray;
    root["version"] = "1.0";
    root["saveTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();

    currentFilePath = fileName;
    setModified(false);
    statusBar()->showMessage("文件已保存: " + fileName, 3000);
}

// 标签编辑相关方法
void MainWindow::renameTab()
{
    int currentRow = tabsList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(notepadItems.size())) {
        QString currentTitle = notepadItems[currentRow]->getTitle();
        bool ok;
        QString newTitle = QInputDialog::getText(this, "重命名标签",
                                                 "请输入新标签名称:",
                                                 QLineEdit::Normal,
                                                 currentTitle, &ok);
        if (ok && !newTitle.trimmed().isEmpty()) {
            notepadItems[currentRow]->setTitle(newTitle.trimmed());
            tabsList->item(currentRow)->setText(newTitle.trimmed());
            setModified(true);
            statusBar()->showMessage("已重命名标签", 1500);
        }
    }
}

void MainWindow::duplicateTab()
{
    int currentRow = tabsList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(notepadItems.size())) {
        auto originalItem = notepadItems[currentRow];
        auto newItem = std::make_shared<NotepadItem>(
            originalItem->getTitle() + " - 副本",
            originalItem->getContent()
            );
        notepadItems.push_back(newItem);

        QListWidgetItem* item = new QListWidgetItem(newItem->getTitle());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        tabsList->addItem(item);
        tabsList->setCurrentRow(tabsList->count() - 1);

        setModified(true);
        statusBar()->showMessage("已复制标签", 2000);
    }
}

// 上下文菜单事件
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (tabsList->geometry().contains(event->pos())) {
        // 检查是否点击在标签项上
        QListWidgetItem* item = tabsList->itemAt(tabsList->mapFromGlobal(event->globalPos()));
        if (item) {
            tabContextMenu->exec(event->globalPos());
        }
    } else {
        QMainWindow::contextMenuEvent(event);
    }
}
