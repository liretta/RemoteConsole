#include "dialog_wait_connection.h"
#include "../RemoteConsole/class_server.h"

#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <thread>



WaitingThread::WaitingThread(DialogWaitConnection& dialog) :
    m_dialog(dialog)
{ }

void WaitingThread::run()
{
    bool is_initialized = m_dialog.m_server.waitingForConnection();

    if (is_initialized)
    {
        emit ready();
    }
    else
    {
        emit fail();
    }
}


DialogWaitConnection::DialogWaitConnection(Server& server, QWidget* parent) :
    m_server(server),
    m_label_text(new QLabel("Waiting for connection", this)),
    m_button_cancel(new QPushButton("Cancel", this)),
    m_thread(*this)
{
    initialize_window();
}

DialogWaitConnection::~DialogWaitConnection()
{
    m_thread.quit();
}


int DialogWaitConnection::exec()
{
    connect(&m_thread, SIGNAL(ready()), this, SLOT(accept()));
    connect(&m_thread, SIGNAL(fail()),  this, SLOT(reject()));

    m_thread.start();
    return QDialog::exec();
}

void DialogWaitConnection::initialize_window()
{
    setFixedWidth(400);

    connect(m_button_cancel, SIGNAL(clicked()), this, SLOT(cancel()));

    auto* layout_main = new QVBoxLayout;
    layout_main->setAlignment(Qt::AlignHCenter);

        layout_main->addWidget(m_label_text);
        layout_main->addWidget(m_button_cancel);

    setLayout(layout_main);
}

void DialogWaitConnection::cancel()
{
    reject();
}
