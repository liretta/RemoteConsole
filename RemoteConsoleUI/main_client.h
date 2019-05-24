#ifndef DIALOG_CLIENT_H
#define DIALOG_CLIENT_H

#include "output_director.hpp"

#include <QMainWindow>
#include <QDebug>

class QLineEdit;
class QPushButton;
class QLabel;

class Client;



class MainClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainClientWindow(Client&, QWidget *parent = nullptr);

private:

    QLineEdit   *m_line_command;
    QPushButton *m_button_execute;

    QPlainTextEdit
        *m_text_output,
        *m_text_errors;

    QLabel
        *m_label_command,
        *m_label_output,
        *m_label_errors;

    Client& m_client;

    WOutputStream
        m_stream_output,
        m_stream_errors;

    void initialize_window();

private slots:

    void execute();
};

#endif // DIALOG_CLIENT_H
