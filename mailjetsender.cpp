#include "mailjetsender.h"
#include <QNetworkRequest>
#include <QJsonArray>
#include <QDebug>

MailJetSender::MailJetSender(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MailJetSender::onFinished);
}

void MailJetSender::envoyerMail(const QString &fromName,
                                const QString &fromEmail,
                                const QString &toEmail,
                                const QString &subject,
                                const QString &body)
{
    QUrl url("https://api.mailjet.com/v3.1/send");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Basic " +
                                              QString("%1:%2").arg(apiKey, apiSecret).toUtf8().toBase64());

    QJsonObject fromObj{{"Email", fromEmail}, {"Name", fromName}};
    QJsonObject toObj{{"Email", toEmail}, {"Name", toEmail}};
    QJsonArray toArr; toArr.append(toObj);

    QJsonObject message;
    message["From"] = fromObj;
    message["To"] = toArr;
    message["Subject"] = subject;
    message["TextPart"] = body;

    QJsonObject root;
    root["Messages"] = QJsonArray{ message };

    QJsonDocument doc(root);
    manager->post(request, doc.toJson());
}

void MailJetSender::onFinished(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();
    qDebug() << "📬 MailJet response:" << response;

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "❌ MailJet Error:" << reply->errorString();
    } else {
        qDebug() << "✅ Email envoyé via MailJet.";
    }

    reply->deleteLater();
}
