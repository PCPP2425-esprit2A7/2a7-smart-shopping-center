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
    void envoyerMessageChatbot(const QString &message);

signals:
    void requeteSQLPr(const QString &sqlQuery);
    void descriptionGeneree(const QString &description);
    void chatbotResponse(const QString &response);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    const QString apiUrl = "https://api.openai.com/v1/chat/completions";

};

#endif // OPENAICLIENT_H
