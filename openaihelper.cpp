#include "openaihelper.h"

OpenAIHelper::OpenAIHelper(QObject *parent)
    : QObject(parent), manager(new QNetworkAccessManager(this))
{
    connect(manager, &QNetworkAccessManager::finished, this, &OpenAIHelper::onReplyFinished);
}

void OpenAIHelper::analyserMaintenance(const QString &description)
{
    QUrl url("https://api.openai.com/v1/chat/completions");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());

    QJsonObject message;
    message["role"] = "user";
    message["content"] = QString(
                             "Analyse cette description de maintenance : \"%1\".\n"
                             "- Qui doit faire cette réparation ? (plombier, électricien...)\n"
                             "- Estimation du coût en DT.\n"
                             "- Matériaux nécessaires.\n"
                             "- Étapes pour réparer.").arg(description);

    QJsonArray messages;
    messages.append(message);

    QJsonObject payload;
    payload["model"] = "gpt-3.5-turbo";
    payload["messages"] = messages;
    payload["temperature"] = 0.7;

    QJsonDocument doc(payload);
    QByteArray data = doc.toJson();

    manager->post(request, data);
}

void OpenAIHelper::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit analysisReady("Erreur lors de la requête : " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    qDebug() << "Réponse brute de l'API OpenAI:\n" << QString(response);
    qDebug() << "Code HTTP:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();


    QJsonDocument doc = QJsonDocument::fromJson(response);
    QString result = doc["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();

    emit analysisReady(result);
    reply->deleteLater();
}
