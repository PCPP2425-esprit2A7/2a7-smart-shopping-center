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
     void genererImage(const QString &prompt);  // <<==== AJOUTE CETTE LIGNE

signals:
    void reponseRecue(QString response);
    void imageGeneree(const QString &imageUrl);


private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QString apiKey;
};

#endif // OPENAICLIENT_H
