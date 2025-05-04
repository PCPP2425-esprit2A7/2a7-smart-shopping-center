#ifndef MAILJETSENDER_H
#define MAILJETSENDER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

class MailJetSender : public QObject
{
    Q_OBJECT
public:
    explicit MailJetSender(QObject *parent = nullptr);
    void envoyerMail(const QString &fromName,
                     const QString &fromEmail,
                     const QString &toEmail,
                     const QString &subject,
                     const QString &body);

private slots:
    void onFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    const QString apiKey = "2e234ba12ed6dba0ed2ea8f444f51590";        // ← Mets ta vraie clé ici
    const QString apiSecret = "1d90fa3b57f3e0af9467b2d96eddcb20";  // ← Mets ta vraie clé ici
};

#endif // MAILJETSENDER_H
