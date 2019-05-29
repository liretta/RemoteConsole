#include "dialog_client_authorization.h"
#include "../RemoteConsole/class_client.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QMessageBox>
#include <QRegExpValidator>

DialogClientAuthorization::DialogClientAuthorization(Client& client, QWidget *parent) :
    QDialog (parent),
    m_line_login(new QLineEdit(this)),
    m_line_password(new QLineEdit(this)),
    m_label_login(new QLabel("Login", this)),
    m_label_password(new QLabel("Password", this)),
    m_button_signin(new QPushButton("Sign In", this)),
    m_button_cancel(new QPushButton("Cancel", this)),
    m_client(client)
{
    initialize_window();
}


void DialogClientAuthorization::initialize_window()
{
    const int
        min_width   = 300,
        min_height  = 100;

    setMinimumSize(min_width, min_height);
    setMaximumSize(min_width * 2, min_height * 2);

    connect(m_button_signin, SIGNAL(clicked()), this, SLOT(signin()));
    connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(cancel()));

    QRegExp re("^[0-9a-zA-Z]{1,45}$");
    QRegExpValidator *validator = new QRegExpValidator(re, this);
    m_line_login->setValidator(validator);
    m_line_password->setValidator(validator);

    auto* layout_main = new QVBoxLayout;

        auto* layout_fields = new QGridLayout;

            layout_fields->addWidget(m_label_login, 0, 0);
            layout_fields->addWidget(m_line_login, 0, 1);
            layout_fields->addWidget(m_label_password, 1, 0);
            layout_fields->addWidget(m_line_password, 1, 1);

        layout_main->addLayout(layout_fields);

        auto* layout_button = new QHBoxLayout;

            layout_button->addWidget(m_button_signin);
            layout_button->addWidget(m_button_cancel);

        layout_main->addLayout(layout_button);

        setLayout(layout_main);
}

void DialogClientAuthorization::signin()
{
    std::string
        login = m_line_login->text().toStdString(),
        password = m_line_password->text().toStdString();

    bool is_authorized = m_client.getLogger().check_password(login, password);

    if (!is_authorized)
    {
        QMessageBox::warning(this, "Sign In status", "Wrong login or password", QMessageBox::Ok);
    }
    else
    {
        accept();
    }
}

void DialogClientAuthorization::cancel()
{
    reject();
}
