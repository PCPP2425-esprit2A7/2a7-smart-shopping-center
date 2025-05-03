#include "openaiclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>




OpenAIClient::OpenAIClient(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    //apiKey
    connect(manager, &QNetworkAccessManager::finished, this, &OpenAIClient::onReplyFinished);
}

void OpenAIClient::envoyerRequete(const QString &question) {
    QUrl url("https://api.openai.com/v1/chat/completions");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["model"] = "gpt-4-turbo";
    QJsonArray messages;
    messages.append(QJsonObject{{"role", "system"}, {"content", "Tu es un assistant répondant aux questions sur les évenements."}});
    messages.append(QJsonObject{{"role", "user"}, {"content", question}});
    json["messages"] = messages;

    manager->post(request, QJsonDocument(json).toJson());
}

void OpenAIClient::onReplyFinished(QNetworkReply *reply) {
    QByteArray response_data = reply->readAll();

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
        QJsonObject jsonObject = jsonResponse.object();

        if (reply->request().url().path().contains("/images/generations")) {
            // Traitement de l'image
            if (jsonObject.contains("data")) {
                QJsonArray data = jsonObject["data"].toArray();
                if (!data.isEmpty()) {
                    QString imageUrl = data[0].toObject()["url"].toString();
                    emit imageGeneree(imageUrl);
                }
            }
        } else if (reply->request().url().path().contains("/chat/completions")) {
            // Traitement du texte (comme avant)
            if (jsonObject.contains("choices")) {
                QJsonArray choices = jsonObject["choices"].toArray();
                if (!choices.isEmpty()) {
                    QString responseText = choices[0].toObject()["message"].toObject()["content"].toString().trimmed();
                    emit reponseRecue(responseText);
                }
            }
        }
    } else {
        qDebug() << "Erreur API OpenAI : " << reply->errorString();
    }

    reply->deleteLater();
}

void OpenAIClient::genererImage(const QString &prompt) {
    QUrl url("https://api.openai.com/v1/images/generations");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["prompt"] = prompt;
    json["n"] = 1;
    json["size"] = "512x512";  // Tu peux mettre "1024x1024" aussi

    manager->post(request, QJsonDocument(json).toJson());
}

