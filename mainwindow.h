#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

#include <QDebug>
#include <QFuture>
#include <QProcess>
#include <QtConcurrent/QtConcurrent>

class Pinger : public QObject
{
    Q_OBJECT
public:
    explicit Pinger(QObject *parent = nullptr)
        : QObject{parent}
        , m_pProcessPing{nullptr}
        , m_processPing(this)
    {
        qDebug() << "+Pinger()";
        qDebug() << "-Pinger()";
    }
    ~Pinger()
    {
        qDebug() << "+~Pinger()";
        stopSync();
        qDebug() << "-~Pinger()";
    }

    QFuture<int> startAsync()
    {
        qDebug() << "+startAsync()";
        QPromise<int> promise;
        auto future = QtConcurrent::run([this](QPromise<int>& promise)
        {
            auto errorCode = startSync();
            promise.addResult(errorCode);
        });
        qDebug() << "-startAsync()";
        return future;
    }

    int startSync()
    {
        qDebug() << "+startSync()";
        if (m_pProcessPing)
        {
            throw QException();
        }

        m_pProcessPing = &m_processPing;

        connect(&m_processPing, &QProcess::errorOccurred, this, [](QProcess::ProcessError error)
        {
            qDebug() << "errorOccurred error=" << error;
        });
        connect(&m_processPing, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus)
        {
            auto program = m_processPing.program();
            auto args = m_processPing.arguments();
            auto error = m_processPing.readAllStandardError();
            auto output = m_processPing.readAllStandardOutput();
            qDebug() << "finished args" << args;
            qDebug() << "finished exitCode" << exitCode;
            qDebug() << "finished exitStatus" << exitStatus;
            qDebug() << "finished error" << QString::fromUtf8(error);
            qDebug() << "finished output" << QString::fromUtf8(output);
            if (exitCode)
            {
                emit onError(exitCode, program, args, output, error);
            }
            else
            {
                emit onStopped();
            }
        });
        connect(&m_processPing, &QProcess::readyReadStandardError, this, [this]()
        {
            emit onStandardError(m_processPing.readAllStandardError());
        });
        connect(&m_processPing, &QProcess::readyReadStandardOutput, this, [this]()
        {
            emit onStandardOutput(m_processPing.readAllStandardOutput());
        });
        connect(&m_processPing, &QProcess::started, this, [this]()
        {
            qDebug() << "started";
            emit onStart(0);
        });
        connect(&m_processPing, &QProcess::stateChanged, this, [](QProcess::ProcessState newState)
        {
            qDebug() << "stateChanged newState=" << newState;
        });

        m_processPing.start("ping", QStringList() << "-t" << "127.0.0.1");

        int errorCode;
        if (m_processPing.waitForStarted(1000))
        {
            // IF THIS LINE ISN'T EXECUTED THEN readyReadStandardError AND readyReadStandardOutput NEVER GET CALLED
            // Strangely enough, all of the other signals get called. :/
            m_processPing.waitForFinished(-1);

            errorCode = 0;
        }
        else
        {
            qDebug() << "process failed to start";
            errorCode = 1;
        }
        qDebug().nospace() << "errorCode=" << errorCode;

        qDebug() << "-startSync()";
        return errorCode;
    }

    QFuture<void> stopAsync()
    {
        qDebug() << "+stopAsync()";
        QPromise<void> promise;
        auto future = QtConcurrent::run([this](QPromise<void>& promise)
        {
            stopSync();
        });
        qDebug() << "-stopAsync()";
        return future;
    }

    void stopSync()
    {
        qDebug() << "+stopSync()";
        if (m_pProcessPing)
        {
#if 0
            // Why doesn't `QProcess::close()` work nicely?
            qDebug() << "+m_pProcessPing->close()";
            m_pProcessPing->close();
            qDebug() << "-m_pProcessPing->close()";
#else
            qDebug() << "+m_pProcessPing->kill()";
            m_pProcessPing->kill();
            qDebug() << "-m_pProcessPing->kill()";
#endif
            m_pProcessPing = nullptr;
        }
        qDebug() << "-stopSync()";
    }

    QProcess* m_pProcessPing;
    QProcess m_processPing;

signals:
    void onStart(int errorCode);
    void onStandardError(const QString& error);
    void onStandardOutput(const QString& output);
    void onError(int exitCode,
                 const QString& program,
                 const QStringList& args,
                 const QString& output,
                 const QString& error);
    void onStopped();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTextEdit textEditLog;
    Pinger pinger;
private slots:
    void onPingStart(int errorCode);
    void onPingStandardError(const QString& error);
    void onPingStandardOutput(const QString& output);
    void onPingError(int exitCode,
                     const QString& program,
                     const QStringList& args,
                     const QString& output,
                     const QString& error);
    void onPingStopped();
};
#endif // MAINWINDOW_H
