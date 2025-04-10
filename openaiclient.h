#ifndef OPENAICLIENT_H
#define OPENAICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class OpenAIClient : public QObject {
    Q_OBJECT
public:
    explicit OpenAIClient(QObject *parent = nullptr);
    void envoyerRequete(const QString &question);

signals:
    void reponseRecue(QString response);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QString apiKey;
};

#endif // OPENAICLIENT_H
