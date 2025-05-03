#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>
#include <QDateTime>
#include <stdexcept>

class ChatException : public std::runtime_error {
public:
    explicit ChatException(const QString& message)
        : std::runtime_error(message.toStdString()) {}
};

struct ChatMessage {
    QString sender;
    QString content;
    QDateTime timestamp;

    QString extracted() const {
        return QString("[%1] %2: %3")
        .arg(timestamp.toString("hh:mm:ss"))
            .arg(sender)
            .arg(content);
    }
    QString toString() const { return extracted(); }
};

class ChatManager : public QObject {
    Q_OBJECT
public:
    explicit ChatManager(quint16 port, QObject *parent = nullptr);
    ~ChatManager();

    void sendMessage(const QString &sender, const QString &message);
    QList<ChatMessage> messageHistory() const { return m_messages; }

signals:
    void newMessageReceived(const ChatMessage &message);
    void errorOccurred(const QString &error);

private slots:
    void onNewConnection();
    void processMessage(const QString &message);
    void socketDisconnected();

private:
    QWebSocketServer *m_webSocketServer;
    QList<QWebSocket *> m_clients;
    QList<ChatMessage> m_messages;

    void initializeServer(quint16 port);
    void broadcast(const QString &message);
};

#endif // CHATMANAGER_H
