#include "chatmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

ChatManager::ChatManager(quint16 port, QObject *parent)
    : QObject(parent), m_webSocketServer(nullptr) {
    try {
        initializeServer(port);
    } catch (const ChatException& e) {
        emit errorOccurred(e.what());
        throw;
    }
}

ChatManager::~ChatManager() {
    m_webSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void ChatManager::initializeServer(quint16 port) {
    m_webSocketServer = new QWebSocketServer(
        "Chat Server",
        QWebSocketServer::NonSecureMode,
        this
        );

    if (!m_webSocketServer->listen(QHostAddress::Any, port)) {
        QString errorMsg = QString("Impossible de démarrer le serveur sur le port %1: %2")
                               .arg(port)
                               .arg(m_webSocketServer->errorString());
        throw ChatException(errorMsg);
    }

    connect(m_webSocketServer, &QWebSocketServer::newConnection,
            this, &ChatManager::onNewConnection);
}

void ChatManager::onNewConnection() {
    QWebSocket *socket = m_webSocketServer->nextPendingConnection();

    if (!socket) {
        emit errorOccurred("Erreur lors de l'acceptation d'une nouvelle connexion");
        return;
    }

    connect(socket, &QWebSocket::textMessageReceived,
            this, &ChatManager::processMessage);
    connect(socket, &QWebSocket::disconnected,
            this, &ChatManager::socketDisconnected);

    m_clients << socket;
    qDebug() << "Nouveau client connecté:" << socket->peerAddress().toString();
}

void ChatManager::processMessage(const QString &message) {
    try {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            throw ChatException("Message JSON invalide: " + parseError.errorString());
        }

        QJsonObject obj = doc.object();
        if (!obj.contains("sender") || !obj.contains("content")) {
            throw ChatException("Message incomplet: sender ou content manquant");
        }

        ChatMessage msg;
        msg.sender = obj["sender"].toString();
        msg.content = obj["content"].toString();
        msg.timestamp = QDateTime::currentDateTime();

        m_messages.append(msg);
        emit newMessageReceived(msg);

        // Broadcast à tous les clients
        broadcast(message);
    } catch (const ChatException& e) {
        emit errorOccurred(e.what());
    }
}

void ChatManager::sendMessage(const QString &sender, const QString &message) {
    try {
        if (sender.isEmpty() || message.isEmpty()) {
            throw ChatException("Sender ou message vide");
        }

        QJsonObject obj;
        obj["sender"] = sender;
        obj["content"] = message;

        QJsonDocument doc(obj);
        QString jsonString = doc.toJson(QJsonDocument::Compact);

        broadcast(jsonString);

        // Ajouter au historique local
        ChatMessage msg;
        msg.sender = sender;
        msg.content = message;
        msg.timestamp = QDateTime::currentDateTime();
        m_messages.append(msg);
    } catch (const ChatException& e) {
        emit errorOccurred(e.what());
    }
}

void ChatManager::broadcast(const QString &message) {
    for (QWebSocket *client : qAsConst(m_clients)) {
        if (client && client->isValid()) {
            client->sendTextMessage(message);
        }
    }
}

void ChatManager::socketDisconnected() {
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        m_clients.removeAll(client);
        client->deleteLater();
        qDebug() << "Client déconnecté";
    }
}
