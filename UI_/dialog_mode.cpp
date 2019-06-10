#include "dialog_mode.h"
#include "main_server.h"
#include "dialog_client_authorization.h"

#include <QPushButton>
#include <QLabel>

#include <QLayout>

DialogMode::DialogMode(QWidget *parent)
    : QDialog(parent),
      m_button_server(new QPushButton("Server", this)),
      m_button_client(new QPushButton("Client", this)),
      m_label_text(new QLabel("Choose mode of the application")),
      m_mode(Mode::None)
{
    initialize_window();
}


DialogMode::Mode DialogMode::getMode() const
{
    return m_mode;
}

void DialogMode::initialize_window()
{
    const int
        min_width   = 300,
        min_height  = 100,
        min_button_height = 30;


    setMinimumSize(min_width, min_height);
    setMaximumSize(min_width * 2, min_height * 2);

    m_button_server->setMinimumHeight(min_button_height);
    m_button_client->setMinimumHeight(min_button_height);


    connect(m_button_server, SIGNAL(clicked()), this, SLOT(run_server()));
    connect(m_button_client, SIGNAL(clicked()), this, SLOT(run_client()));


    auto* layout_main = new QVBoxLayout;

        layout_main->addWidget(m_label_text);

        auto* layout_button = new QHBoxLayout;

            layout_button->addWidget(m_button_server);
            layout_button->addWidget(m_button_client);

        layout_main->addLayout(layout_button);

    layout_main->setAlignment(m_label_text, Qt::AlignHCenter);
    setLayout(layout_main);
}

void DialogMode::run_server()
{
    m_mode = Mode::Server;
    accept();
}

void DialogMode::run_client()
{
    m_mode = Mode::Client;
    accept();
}
