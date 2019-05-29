#pragma once

#include <QDialog>
#include <QThread>

class Server;
class QLabel;

class DialogWaitConnection;

class WaitingThread : public QThread
{
    Q_OBJECT

public:

    WaitingThread(DialogWaitConnection& dialog);

private:

    void run() override;
    DialogWaitConnection& m_dialog;

signals:

    void ready();
    void fail();
};

class DialogWaitConnection : public QDialog
{
    Q_OBJECT

    friend WaitingThread;

public:
    DialogWaitConnection(Server&, QWidget *parent = nullptr);
    ~DialogWaitConnection();

    virtual int exec() override;

private:

    void initialize_window();

    Server& m_server;

    QLabel*      m_label_text;
    QPushButton* m_button_cancel;

    WaitingThread m_thread;

private slots:

    void cancel();
};
