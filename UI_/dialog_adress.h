#ifndef DIALOG_ADRESS_H
#define DIALOG_ADRESS_H

#include <QDialog>


class QLabel;
class QLineEdit;

class Client;

class DialogAdress : public QDialog
{
    Q_OBJECT

public:
    DialogAdress(Client&, QWidget *parent = nullptr);

private:

    QPushButton
        *m_button_connect,
        *m_button_back;

    QLineEdit   *m_line_adress;
    QLabel      *m_label_text;

    void initialize_window();


    Client& m_client;

private slots:

    void try_connect();
};



#endif // DIALOG_ADRESS_H
