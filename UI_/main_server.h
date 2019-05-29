#ifndef DIALOG_SERVER_H
#define DIALOG_SERVER_H

#include <QMainWindow>
#include <QThread>

#include "output_director.hpp"

#include "../RemoteConsole/class_server.h"

class MainServerWindow;
class QPushButton;
class QLabel;

class Client;




class ServerMainThread : public QThread
{
    Q_OBJECT

public:
    ServerMainThread(MainServerWindow&);

private:
    void run() override;

    MainServerWindow& m_window;

signals:
    void connectionLost();
    void connectionCreated();
};




class MainServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainServerWindow(QWidget *parent = nullptr);
    ~MainServerWindow();

private:

    void MainServerWindow::showEvent(QShowEvent* event) override;

    std::unique_ptr<ServerMainThread> m_thread;

    QLabel *m_label_wait_connection;

    QPlainTextEdit
        *m_text_commands,
        *m_text_errors;

    QLabel
        *m_label_commands,
        *m_label_errors;

    TextEditWrapper
        m_wrapper_output,
        m_wrapper_errors;

    WOutputStream
        m_stream_output,
        m_stream_errors;

    Server m_server;


    void initialize_window();

private slots:
    void showWaitingLabel();
    void hideWaitingLabel();
    void displayOutput();
    void displayErrors();
};



#endif // DIALOG_SERVER_H
