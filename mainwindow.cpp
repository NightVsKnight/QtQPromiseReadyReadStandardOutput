#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(960, 540);
    setCentralWidget(&textEditLog);
    textEditLog.setReadOnly(true);
    connect(&pinger, &Pinger::onStart, this, &MainWindow::onPingStart);
    connect(&pinger, &Pinger::onStandardError, this, &MainWindow::onPingStandardError);
    connect(&pinger, &Pinger::onStandardOutput, this, &MainWindow::onPingStandardOutput);
    connect(&pinger, &Pinger::onError, this, &MainWindow::onPingError);
    connect(&pinger, &Pinger::onStopped, this, &MainWindow::onPingStopped);
    pinger.startAsync();
}

MainWindow::~MainWindow()
{
    pinger.stopSync();
}

void MainWindow::onPingStart(int errorCode)
{
    qDebug().nospace() << "+onPingStart(errorCode=" << errorCode << ")";
    if (errorCode)
    {
        qDebug() << "ping start errorCode" << errorCode;
    }
    else
    {
        qDebug() << "ping start success";
    }
    qDebug().nospace() << "-onPingStart(errorCode=" << errorCode << ")";
}

void MainWindow::onPingStandardError(const QString& error)
{
    qDebug() << "ping error:" << error;
    textEditLog.insertPlainText(error);
}

void MainWindow::onPingStandardOutput(const QString& output)
{
    qDebug() << "ping output:" << output;
    textEditLog.insertPlainText(output);
}

void MainWindow::onPingError(int exitCode,
                             const QString& program,
                             const QStringList& args,
                             const QString& output,
                             const QString& error)
{
    qDebug() << "+onPingError(...)";
    qDebug() << "ping error exitCode=" << exitCode << ", error=" << error;
    qDebug() << "-onPingError(...)";
}

void MainWindow::onPingStopped()
{
    qDebug() << "+onPingStopped()";
    qDebug() << "ping stopped!";
    qDebug() << "-onPingStopped()";
}

