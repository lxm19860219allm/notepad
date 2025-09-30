#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    app.setApplicationName("Qt6.6 Notepad");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("MyCompany");

    MainWindow window;
    window.show();

    return app.exec();
}
