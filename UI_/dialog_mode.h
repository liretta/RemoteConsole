#ifndef DIALOG_MODE_H
#define DIALOG_MODE_H

#include <QDialog>


class QLabel;

class DialogMode : public QDialog
{
    Q_OBJECT

public:
    DialogMode(QWidget *parent = nullptr);

    enum class Mode
    {
        Server,
        Client,
        None
    };

    Mode getMode() const;

private:

    QPushButton
        *m_button_server,
        *m_button_client;

    QLabel *m_label_text;

    Mode m_mode;

    void initialize_window();

private slots:

    void run_server();
    void run_client();
};

#endif // DIALOG_MODE_H
