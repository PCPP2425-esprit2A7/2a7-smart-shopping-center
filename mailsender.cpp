#include "mailsender.h"
#include <QDebug>

MailSender::MailSender(QObject *parent) : QObject(parent) {
    socket = new QSslSocket(this);
}

void MailSender::waitForResponse() {
    while (socket->waitForReadyRead(3000)) {
        qDebug() << socket->readAll();
    }
}

void MailSender::sendEmail(const QString &to, const QString &subject, const QString &body) {
    socket->connectToHostEncrypted("smtp.gmail.com", 465);
    if (!socket->waitForConnected(5000)) {
        qDebug() << "Connexion échouée.";
        return;
    }

    stream = new QTextStream(socket);
    stream->setAutoDetectUnicode(true);

    *stream << "EHLO smtp.gmail.com\r\n";
    stream->flush();
    waitForResponse();

    *stream << "AUTH LOGIN\r\n";
    stream->flush();
    waitForResponse();

    *stream << QByteArray("jouiniyoussef200@gmail.com").toBase64() << "\r\n";
    stream->flush();
    waitForResponse();

    *stream << QByteArray("dkwnrszjyeqzdjwn").toBase64() << "\r\n"; // Supprime les espaces
    stream->flush();
    waitForResponse();

    *stream << "MAIL FROM:<jouiniyoussef200@gmail.com>\r\n";
    stream->flush();
    waitForResponse();

    *stream << "RCPT TO:<" << to << ">\r\n";
    stream->flush();
    waitForResponse();

    *stream << "DATA\r\n";
    stream->flush();
    waitForResponse();

    *stream << "Subject: " << subject << "\r\n";
    *stream << "From: jouiniyoussef200@gmail.com\r\n";
    *stream << "To: " << to << "\r\n";
    *stream << "Content-Type: text/html; charset=UTF-8\r\n";
    *stream << "\r\n" << body << "\r\n.\r\n";
    stream->flush();
    waitForResponse();

    *stream << "QUIT\r\n";
    stream->flush();
    waitForResponse();

    qDebug() << "Email envoyé.";
}
