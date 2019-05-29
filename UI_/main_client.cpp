#include "main_client.h"
#include "../RemoteConsole/class_client.h"

#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>

MainClientWindow::MainClientWindow(Client& client, QWidget *parent) :
    QMainWindow(parent),
    m_line_command(new QLineEdit(this)),
    m_button_execute(new QPushButton("Execute", this)),
    m_text_output(new QPlainTextEdit(this)),
    m_text_errors(new QPlainTextEdit(this)),
    m_label_command(new QLabel("Command", this)),
    m_label_output(new QLabel("Output", this)),
    m_label_errors(new QLabel("Errors", this)),
    m_client(client),
    m_stream_output(std::wcout, &m_wrapper_output),
    m_stream_errors(std::wcerr, &m_wrapper_errors)
{
    connect(&m_wrapper_output, SIGNAL(outputRequest()),
            this,              SLOT(displayOutput()));

    connect(&m_wrapper_errors, SIGNAL(outputRequest()),
            this,              SLOT(displayErrors()));

    initialize_window();
}

void MainClientWindow::initialize_window()
{
    const int
        min_width = 500,
        min_height = 300;

    setMinimumSize(min_width, min_height);
    resize(min_width, min_height);

    m_text_output->setReadOnly(true);
    m_text_errors->setReadOnly(true);

    QFont font("unexistent");
    font.setStyleHint(QFont::Monospace);

    m_text_output->setFont(font);
    m_text_errors->setFont(font);

    connect(m_button_execute, SIGNAL(clicked()), this, SLOT(execute()));

    QWidget * window = new QWidget;

        auto* layout_main = new QVBoxLayout;

            auto *layout_command = new QHBoxLayout;

                layout_command->addWidget(m_label_command);
                layout_command->addWidget(m_line_command);
                layout_command->addWidget(m_button_execute);

            layout_main->addLayout(layout_command);

            layout_main->addWidget(m_label_output);
            layout_main->addWidget(m_text_output);
            layout_main->addWidget(m_label_errors);
            layout_main->addWidget(m_text_errors);

        window->setLayout(layout_main);

    setCentralWidget(window);
}

void MainClientWindow::execute()
{
    std::wstring command = m_line_command->text().toStdWString();
    
    std::wcout << m_client.getExecutor().execute(command);
}


void MainClientWindow::displayOutput()
{
    while (!m_wrapper_output.m_queue.empty())
    {
        m_text_output->appendPlainText(m_wrapper_output.m_queue.pop());
    }
}

void MainClientWindow::displayErrors()
{
    while (!m_wrapper_errors.m_queue.empty())
    {
        m_text_errors->appendPlainText(m_wrapper_errors.m_queue.pop());
    }
}