#ifndef MAILSENDER_H
#define MAILSENDER_H

#include <QObject>
#include <QTcpSocket>
#include <QTextStream>

class MailSender : public QObject {
    Q_OBJECT

public:
    explicit MailSender(QObject *parent = nullptr);
    void sendEmail(const QString &to, const QString &subject, const QString &body);

private:
    QTcpSocket *socket;
    QString message;
};

#endif // MAILSENDER_H
