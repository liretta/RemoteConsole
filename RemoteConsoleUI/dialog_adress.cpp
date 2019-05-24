#include "dialog_adress.h"
#include "stub_client.h"

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QMessageBox>
#include <QRegExpValidator>

DialogAdress::DialogAdress(Client& client, QWidget *parent) :
    QDialog (parent),
    m_button_connect(new QPushButton("Connect", this)),
    m_button_back(new QPushButton("Back", this)),
    m_line_adress(new QLineEdit(this)),
    m_label_text(new QLabel("IP adress", this)),
    m_client(client)
{
    initialize_window();
}

void DialogAdress::initialize_window()
{

    const int min_width = 300;

    setMinimumWidth(min_width);

    connect(m_button_connect, SIGNAL(clicked()), this, SLOT(try_connect()));
    connect(m_button_back, SIGNAL(clicked()), this, SLOT(reject()));

    QRegExp re("^\\d{1,3}(\\.\\d{1,3}){3}$");
    QRegExpValidator *validator = new QRegExpValidator(re, this);
    m_line_adress->setValidator(validator);

    auto* layout_main = new QHBoxLayout;

        layout_main->addWidget(m_label_text);
        layout_main->addWidget(m_line_adress);
        layout_main->addWidget(m_button_connect);
        layout_main->addWidget(m_button_back);

    setLayout(layout_main);
}

void DialogAdress::try_connect()
{
    Client::Error error = m_client.getNetworker().init();

    if (error == Client::Error::OK)
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this, "Connection", "Cannot connect to server");
    }
}

