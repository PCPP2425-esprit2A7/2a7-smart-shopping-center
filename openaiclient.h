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
    void envoyerRequeteDescription(const QString &prompt);
    void envoyerMessageChatbot(const QString &message); // 🔥 Ajout du chatbot

signals:
    void requeteSQLPr(const QString &sqlQuery);
    void descriptionGeneree(const QString &description);
    void chatbotResponse(const QString &response); // 🔥 Nouveau signal pour le chatbot

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;

};

#endif // OPENAICLIENT_H
