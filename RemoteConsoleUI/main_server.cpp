#include "main_server.h"

#include <QLabel>
#include <QTextEdit>
#include <QLayout>

MainServerWindow::MainServerWindow(QWidget *parent) :
    QMainWindow(parent),
    m_text_commands(new QPlainTextEdit("", this)),
    m_text_errors(new QPlainTextEdit("", this)),
    m_label_commands(new QLabel("Commands", this)),
    m_label_errors(new QLabel("Errors", this)),
    m_stream_output(std::wcout, m_text_commands),
    m_stream_errors(std::wcerr, m_text_errors)
{
    initialize_window();
}

void MainServerWindow::run()
{
    m_server.run();
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

            layout_main->addWidget(m_label_commands);
            layout_main->addWidget(m_text_commands);
            layout_main->addWidget(m_label_errors);
            layout_main->addWidget(m_text_errors);

        window->setLayout(layout_main);

    setCentralWidget(window);
}
