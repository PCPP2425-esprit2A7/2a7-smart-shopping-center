#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QCloseEvent>
#include "chatmanager.h"

namespace Ui {
class chatwindow;
}

class chatwindow : public QWidget {
    Q_OBJECT

public:
    explicit chatwindow(const QString &username, ChatManager *chatManager, QWidget *parent = nullptr);
    ~chatwindow();
    void setSenderName(const QString& name);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onSendButtonClicked();
    void onNewMessageReceived(const ChatMessage &message);
    void onErrorOccurred(const QString &error);
    void displayMessage(const QString &sender, const QString &message); // Ajoutez cette ligne

private:
    Ui::chatwindow *ui;
    QString m_username;
    ChatManager *m_chatManager;

    void setupConnections();
    void displayMessage(const ChatMessage &message);
    void showError(const QString &error);
};

#endif // CHATWINDOW_H
