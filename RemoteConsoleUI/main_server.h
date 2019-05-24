#ifndef DIALOG_SERVER_H
#define DIALOG_SERVER_H

#include <QMainWindow>
#include "output_director.hpp"

#include "../RemoteConsole/class_server.h"

class QPushButton;
class QLabel;

class Client;

class MainServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainServerWindow(QWidget *parent = nullptr);
    void run();

private:

    QPlainTextEdit
        *m_text_commands,
        *m_text_errors;

    QLabel
        *m_label_commands,
        *m_label_errors;

    WOutputStream
        m_stream_output,
        m_stream_errors;

    Server m_server;

    void initialize_window();
};



#endif // DIALOG_SERVER_H
