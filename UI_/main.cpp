#include "dialog_mode.h"
#include "dialog_client_authorization.h"
#include "dialog_adress.h"
#include "main_server.h"
#include "main_client.h"
#include "../RemoteConsole/class_client.h"

#include <QApplication>


bool runClientAuthorization(Client& client)
{
    bool status = true;

    DialogAdress dialog_adress(client);

    int adress_status = dialog_adress.exec();
    if (adress_status == QDialog::Accepted)
    {
        DialogClientAuthorization dialog_authorization(client);
        int authorization_status = dialog_authorization.exec();

        if (authorization_status == QDialog::Rejected)
        {
            status = false;
        }
        dialog_authorization.close();
    }
    else if (adress_status == QDialog::Rejected)
    {
        status = false;
    }

    dialog_adress.close();
    return status;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	a.setApplicationName(QString("Remote Console"));

    DialogMode::Mode mode   = DialogMode::Mode::None;
    int is_opened_status    = QDialog::Accepted;

    std::unique_ptr<Client> client_ptr(nullptr);

    while (mode == DialogMode::Mode::None && is_opened_status == QDialog::Accepted)
    {
        DialogMode dialog_mode;
        is_opened_status    = dialog_mode.exec();
        mode                = dialog_mode.getMode();

        if (mode == DialogMode::Mode::Client)
        {
            client_ptr.reset(new Client{});

            if (!runClientAuthorization(*client_ptr))
            {
                mode = DialogMode::Mode::None;
                client_ptr.reset(nullptr);
            }
        }

        dialog_mode.close();
    }


    bool is_executable = false;
    if (mode == DialogMode::Mode::Client && client_ptr)
    {
        auto* main_client = new MainClientWindow(*client_ptr);
        main_client->show();
        is_executable = true;
    }
    else if (mode == DialogMode::Mode::Server)
    {
        auto* main_server = new MainServerWindow;
        main_server->show();
        is_executable = true;
    }

    if (is_executable)
    {
        a.exec();
    }
    else
    {
        a.quit();
    }

    return 0;
}
