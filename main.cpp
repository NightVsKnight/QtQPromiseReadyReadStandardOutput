#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qSetMessagePattern("[%{time hh:mm:ss.zzz} p%{pid} t%{threadid} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{function}:%{line} | %{message}");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
