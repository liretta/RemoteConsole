#include "main_server.h"

#include <QLabel>
#include <QLayout>






MainServerWindow::MainServerWindow(QWidget *parent) :
    QMainWindow(parent),
    m_thread(nullptr),
    m_label_wait_connection(new QLabel("Waiting for connection", this)),
    m_text_commands(new QPlainTextEdit("", this)),
    m_text_errors(new QPlainTextEdit("", this)),
    m_label_commands(new QLabel("Commands", this)),
    m_label_errors(new QLabel("Errors", this)),
    m_stream_output(std::wcout, &m_wrapper_output),
    m_stream_errors(std::wcerr, &m_wrapper_errors)
{
    connect(&m_wrapper_output, SIGNAL(outputRequest()),
            this,              SLOT(displayOutput()));

    connect(&m_wrapper_errors, SIGNAL(outputRequest()),
            this,              SLOT(displayErrors()));

    initialize_window();
}

MainServerWindow::~MainServerWindow()
{
    if (m_thread)
    {
        m_thread->quit();
    }
}

ServerMainThread::ServerMainThread(MainServerWindow& window) :
    m_window(window)
{
    connect(this,      SIGNAL(connectionLost()),
            &m_window, SLOT(showWaitingLabel()));
    connect(this,      SIGNAL(connectionCreated()),
            &m_window, SLOT(hideWaitingLabel()));
}

void ServerMainThread::run()
{
    emit connectionLost();
    std::wcout << L"--> waiting for connection" << std::endl;
    m_window.m_server.waitingForConnection();

    int i = 0;
    while (true)
    {
        bool is_sent_key = m_window.m_server.sendKey();
        bool is_got_key = m_window.m_server.getKey();

        if (is_got_key && is_sent_key)
        {
            emit connectionCreated();
            std::wcout << L"--> waiting for logging in" << std::endl;
            m_window.m_server.logIn();

            std::wcout << L"--> running" << std::endl;
            m_window.m_server.run();
        }

        emit connectionLost();
        std::wcout << L"--> waiting for reconnection" << std::endl;
        m_window.m_server.reconnect();

        ++i;
    }
}


void MainServerWindow::showEvent(QShowEvent* event)
{
    static bool is_once_shown = false;

    QMainWindow::showEvent(event);

    if (!is_once_shown)
    {
        m_thread.reset(
            new ServerMainThread(*this));
        m_thread->start();

        is_once_shown = true;
    }
}

void MainServerWindow::initialize_window()
{
    const int
        min_width = 500,
        min_height = 300;

    setMinimumSize(min_width, min_height);
    resize(min_width, min_height);

    m_text_commands->setReadOnly(true);
    m_text_errors->setReadOnly(true);

    QFont font("unexistent");
    font.setStyleHint(QFont::Monospace);

    m_text_commands->setFont(font);
    m_text_errors->setFont(font);

    QWidget * window = new QWidget;

        auto* layout_main = new QVBoxLayout;
            
            layout_main->addWidget(m_label_wait_connection);
            layout_main->setAlignment(m_label_wait_connection, Qt::AlignRight);

            layout_main->addWidget(m_label_commands);
            layout_main->addWidget(m_text_commands);
            layout_main->addWidget(m_label_errors);
            layout_main->addWidget(m_text_errors);

        window->setLayout(layout_main);

    setCentralWidget(window);
}

void MainServerWindow::showWaitingLabel()
{
    m_label_wait_connection->setHidden(false);
    m_label_wait_connection->repaint();
}

void MainServerWindow::hideWaitingLabel()
{
    m_label_wait_connection->setHidden(true);
    m_label_wait_connection->repaint();
}

void MainServerWindow::displayOutput()
{
    while (!m_wrapper_output.m_queue.empty())
    {
        m_text_commands->appendPlainText(m_wrapper_output.m_queue.pop());
    }
}

void MainServerWindow::displayErrors()
{
    while (!m_wrapper_errors.m_queue.empty())
    {
        m_text_errors->appendPlainText(m_wrapper_errors.m_queue.pop());
    }
}



