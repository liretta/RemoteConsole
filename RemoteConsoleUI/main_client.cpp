#include "main_client.h"
#include "stub_client.h"

#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QTextEdit>
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
    m_stream_output(std::cout, m_text_output),
    m_stream_errors(std::cerr, m_text_errors)
{
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
    m_client.execute();
}
