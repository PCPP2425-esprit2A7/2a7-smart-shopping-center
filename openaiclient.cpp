#include "openaiclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

OpenAIClient::OpenAIClient(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    // Remplace par la clé OpenAI
    connect(manager, &QNetworkAccessManager::finished, this, &OpenAIClient::onReplyFinished);
}

void OpenAIClient::envoyerRequete(const QString &question) {
    QUrl url("https://api.openai.com/v1/chat/completions");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["model"] = "gpt-3.5-turbo";
    QJsonArray messages;
    messages.append(QJsonObject{{"role", "system"}, {"content", "Tu es un assistant répondant aux questions sur les évenements."}});
    messages.append(QJsonObject{{"role", "user"}, {"content", question}});
    json["messages"] = messages;

    manager->post(request, QJsonDocument(json).toJson());
}

void OpenAIClient::onReplyFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response_data = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
        QJsonObject jsonObject = jsonResponse.object();

        if (jsonObject.contains("choices")) {
            QJsonArray choices = jsonObject["choices"].toArray();
            if (!choices.isEmpty()) {
                QString responseText = choices[0].toObject()["message"].toObject()["content"].toString().trimmed();
                emit reponseRecue(responseText);
            }
        }
    } else {
        qDebug() << "Erreur API OpenAI : " << reply->errorString();
    }
    reply->deleteLater();
}
