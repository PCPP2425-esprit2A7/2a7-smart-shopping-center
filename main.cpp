#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Connection c;

    bool test = c.createconnect();
    if (test) {
        w.show();
        QMessageBox::information(nullptr, 
            QObject::tr("Connexion réussie"),
            QObject::tr("La connexion à la base de données a été établie avec succès.\n"
                       "Cliquez sur OK pour continuer."), 
            QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, 
            QObject::tr("Erreur de connexion"),
            QObject::tr("Impossible de se connecter à la base de données.\n"
                       "Erreur : %1\n"
                       "Cliquez sur OK pour quitter.").arg(c.getLastError()),
            QMessageBox::Ok);
        return -1;
    }

    return a.exec();
}
