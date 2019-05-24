#ifndef DIALOG_CLIENT_AUTHORIZATION_H
#define DIALOG_CLIENT_AUTHORIZATION_H

#include <QDialog>

class Client;


class QLineEdit;
class QLabel;

class Client;

class DialogClientAuthorization : public QDialog
{
    Q_OBJECT

public:
    DialogClientAuthorization(Client&, QWidget *parent = nullptr);

private:
    QLineEdit
        *m_line_login,
        *m_line_password;

    QLabel
        *m_label_login,
        *m_label_password;

    QPushButton
        *m_button_signin,
        *m_button_cancel;

    Client& m_client;

    void initialize_window();

private slots:

    void signin();
    void cancel();
};

#endif // DIALOG_CLIENT_AUTHORIZATION_H
