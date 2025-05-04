#ifndef OPENAIHELPER_H
#define OPENAIHELPER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class OpenAIHelper : public QObject
{
    Q_OBJECT

public:
    explicit OpenAIHelper(QObject *parent = nullptr);
    void analyserMaintenance(const QString &description);

signals:
    void analysisReady(const QString &result);  // Pour envoyer la réponse au GUI

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // OPENAIHELPER_H
