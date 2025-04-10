#include "mailsender.h"
#include <QDebug>

MailSender::MailSender(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
}
void MailSender::sendEmail(const QString &to, const QString &subject, const QString &body) {
    socket->connectToHost("smtp.gmail.com", 587);
    if (!socket->waitForConnected(5000)) {
        qDebug() << "Erreur : Impossible de se connecter au serveur SMTP.";
        return;
    }

    QTextStream stream(socket);
    stream << "EHLO smtp.gmail.com\r\n";
    stream.flush();

    // Ajouter authentification SMTP
    stream << "AUTH LOGIN\r\n";
    stream.flush();
    stream << QByteArray("jouiniyoussef200@gmail.com").toBase64() << "\r\n";  // Encodage Base64
    stream.flush();
    stream << QByteArray("dkwn rszj yeqz djwn").toBase64() << "\r\n";
    stream.flush();

    // Envoyer l'email
    stream << "MAIL FROM:<jouiniyoussef200@gmail.com>\r\n";
    stream.flush();
    stream << "RCPT TO:<" << to << ">\r\n";
    stream.flush();
    stream << "DATA\r\n";
    stream.flush();
    stream << "Subject: " << subject << "\r\n";
    stream << "From: jouiniyoussef200@gmail.com\r\n";
    stream << "To: " << to << "\r\n";
    stream << "\r\n" << body << "\r\n.\r\n";
    stream.flush();

    qDebug() << "E-mail envoyé avec succès !";
}
