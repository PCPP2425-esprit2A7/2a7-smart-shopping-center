#include "openaiclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

OpenAIClient::OpenAIClient(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &OpenAIClient::onReplyFinished);
}

void OpenAIClient::envoyerRequeteDescription(const QString &prompt) {
    if (apiKey.isEmpty()) {
        qDebug() << "Erreur : La clé API est manquante.";
        return;
    }

    QNetworkRequest request((QUrl(apiUrl)));
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["model"] = "gpt-3.5-turbo";

    QJsonArray messages;
    messages.append(QJsonObject{{"role", "system"}, {"content", "Tu es un assistant qui génère des descriptions pour des services."}});
    messages.append(QJsonObject{{"role", "user"}, {"content", prompt}});
    json["messages"] = messages;

    manager->post(request, QJsonDocument(json).toJson());
}

void OpenAIClient::envoyerRequete(const QString &question) {
    if (apiKey.isEmpty()) {
        qDebug() << "Erreur : La clé API est manquante.";
        return;
    }

    QNetworkRequest request((QUrl(apiUrl)));
    request.setRawHeader("Authorization", "Bearer " + apiKey.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["model"] = "gpt-3.5-turbo";

    QJsonArray messages;
    messages.append(QJsonObject{{"role", "system"}, {"content", "Tu es un assistant qui génère uniquement des requêtes SQL valides."}});
    messages.append(QJsonObject{{"role", "user"}, {"content", "Transforme cette question en requête SQL valide pour Oracle pour la table 'service' (ID, NOM, DESCRIPTION, STATUT, COUT, FREQUENCE, DATE_DEB, DATE_FIN, ID_ESPACE), Utilise TO_DATE('DD/MM/YYYY') pour les dates. Exemple correct : INSERT INTO service (ID, NOM, DESCRIPTION, STATUT, COUT, FREQUENCE, DATE_DEB, DATE_FIN, ID_ESPACE) VALUES (55, 'parking', 'il coute 200', 'actif', 200, 'annuel', TO_DATE('20/11/2025', 'DD/MM/YYYY'), TO_DATE('20/11/2028', 'DD/MM/YYYY'), 1). La réponse doit être uniquement la requête SQL sans explication : " + question}});
    json["messages"] = messages;

    manager->post(request, QJsonDocument(json).toJson());
}


//______________malek_____________
void OpenAIClient::envoyerRequete_chat(const QString &question) {
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

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Erreur : " << reply->errorString();
        qDebug() << "Réponse du serveur : " << response_data;
        reply->deleteLater();
        return;
    }

    QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
    QJsonObject jsonObject = jsonResponse.object();

    if (!jsonObject.contains("choices")) {
        qDebug() << "Réponse JSON invalide : champ 'choices' manquant.";
        reply->deleteLater();
        return;
    }

    QJsonArray choices = jsonObject["choices"].toArray();
    if (choices.isEmpty()) {
        qDebug() << "Réponse JSON invalide : tableau 'choices' vide.";
        reply->deleteLater();
        return;
    }

    QString responseText = choices[0].toObject()["message"].toObject()["content"].toString().trimmed();

    // Nettoyage Markdown
    if (responseText.startsWith("```") && responseText.endsWith("```")) {
        responseText = responseText.mid(3, responseText.length() - 6).trimmed();
    }

    // Si la réponse contient un mot-clé de description
    QString lowerText = responseText.toLower();
    if (lowerText.contains("service") || lowerText.contains("découvrir") ||
        lowerText.contains("actif") || lowerText.contains("profiter")) {
        qDebug() << "Description générée : " << responseText;
        emit descriptionGeneree(responseText);
    }
    // Sinon si c'est une requête SQL
    else if (responseText.startsWith("SELECT") || responseText.startsWith("INSERT") ||
             responseText.startsWith("UPDATE") || responseText.startsWith("DELETE")) {
        qDebug() << "Requête SQL générée : " << responseText;
        emit requeteSQLPr(responseText);
    }
    // Sinon, réponse normale
    else {
        qDebug() << "Réponse standard : " << responseText;
        emit reponseRecue(responseText);
    }

    reply->deleteLater();
}
