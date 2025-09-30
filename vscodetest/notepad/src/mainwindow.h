#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <QFileInfo>
#include <QDateTime>
#include <QStatusBar>
#include <QInputDialog>
#include <QContextMenuEvent>
#include <vector>
#include <memory>
#include "notepaditem.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addNewTab();
    void deleteCurrentTab();
    void onTabSelectionChanged();
    void onContentChanged();
    void onTabTitleChanged(QListWidgetItem* item);
    void onTabDoubleClicked(QListWidgetItem* item);

    // 文件操作槽函数
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void autoSave();

    // 标签编辑相关槽函数
    void renameTab();
    void duplicateTab();

private:
    void setupUI();
    void setupMenuBar();
    void setupContextMenu();
    void updateContentDisplay();
    bool maybeSave();
    void loadFromFile(const QString& fileName);
    void saveToFile(const QString& fileName);
    void updateWindowTitle();
    void setModified(bool modified);

    // 上下文菜单事件
    void contextMenuEvent(QContextMenuEvent *event) override;

    QWidget *centralWidget;
    QSplitter *mainSplitter;

    // 左侧面板
    QWidget *leftPanel;
    QVBoxLayout *leftLayout;
    QLabel *tabsLabel;
    QListWidget *tabsList;
    QPushButton *addButton;
    QPushButton *deleteButton;

    // 右侧面板
    QWidget *rightPanel;
    QVBoxLayout *rightLayout;
    QLabel *contentLabel;
    QTextEdit *contentEdit;

    // 菜单栏
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;

    // 标签操作菜单
    QMenu *tabContextMenu;
    QAction *renameTabAction;
    QAction *duplicateTabAction;
    QAction *deleteTabAction;

    std::vector<std::shared_ptr<NotepadItem>> notepadItems;
    int currentItemId = -1;
    QString currentFilePath;
    bool isModified = false;

    // 自动保存定时器
    QTimer *autoSaveTimer;
};

#endif // MAINWINDOW_H
