#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <QObject>
#include <QSslSocket>
#include <QTextStream>

class MailSender : public QObject {
    Q_OBJECT

public:
    explicit MailSender(QObject *parent = nullptr);
    void sendEmail(const QString &to, const QString &subject, const QString &body);

private:
    QSslSocket *socket;
    QTextStream *stream;
    void waitForResponse();
};

#endif // MAILSENDER_H
