#include "chatwindow.h"
#include "chatmanager.h"
#include "ui_chatwindow.h"
#include <QMessageBox>
#include <QScrollBar>
#include <qpushbutton.h>

chatwindow::chatwindow(const QString &username, ChatManager *chatManager, QWidget *parent)
    : QWidget(parent), ui(new Ui::chatwindow), m_username(username), m_chatManager(chatManager) {
    ui->setupUi(this);
    setWindowTitle("Chat - " + username);

    // Charger l'historique
    for (const auto &msg : m_chatManager->messageHistory()) {
        displayMessage(msg);
    }

    setupConnections();
}

chatwindow::~chatwindow() {
    delete ui;
}
void chatwindow::setSenderName(const QString& name) {
    m_username = name;
    setWindowTitle("Chat - " + name); // Optionnel: mettre à jour le titre
}
void chatwindow::setupConnections()
{
    // Connexion du bouton Envoyer
    connect(ui->sendButton, &QPushButton::clicked,
            this, &chatwindow::onSendButtonClicked);

    // Supprimez la ligne avec returnPressed

    // Connexion au ChatManager
    if(m_chatManager) {
        connect(m_chatManager, &ChatManager::newMessageReceived,
                this, &chatwindow::onNewMessageReceived);
    }
}

void chatwindow::onSendButtonClicked()
{
    QString message = ui->messageInput->toPlainText().trimmed();
    qDebug() << "Tentative d'envoi du message :" << message;

    if(message.isEmpty()) {
        qDebug() << "Message vide ignoré";
        return;
    }

    if(!m_chatManager) {
        qDebug() << "Erreur : ChatManager non disponible";
        QMessageBox::warning(this, "Erreur", "Connexion au chat non disponible");
        return;
    }

    try {
        // Affichez immédiatement votre propre message
        displayMessage(m_username, message);

        // Envoyez au serveur
        m_chatManager->sendMessage(m_username, message);

        ui->messageInput->clear();
        ui->messageInput->setFocus();
    } catch(const std::exception& e) {
        qDebug() << "Exception lors de l'envoi :" << e.what();
        QMessageBox::critical(this, "Erreur", QString("Erreur d'envoi : %1").arg(e.what()));
    }
}
void chatwindow::displayMessage(const QString &sender, const QString &message)
{
    if (!ui->chatDisplay) return;

    QString timestamp = QDateTime::currentDateTime().toString("[hh:mm:ss] ");
    QString formattedMsg = QString("<b>%1%2:</b> %3<br>")
                               .arg(timestamp)
                               .arg(sender.toHtmlEscaped())
                               .arg(message.toHtmlEscaped());

    ui->chatDisplay->append(formattedMsg);
    QScrollBar *sb = ui->chatDisplay->verticalScrollBar();
    sb->setValue(sb->maximum());
}
void chatwindow::onNewMessageReceived(const ChatMessage &message) {
    displayMessage(message);
}

void chatwindow::displayMessage(const ChatMessage &message) {
    QString formattedMsg = message.toString();
    ui->chatDisplay->append(formattedMsg);

    // Auto-scroll
    QScrollBar *sb = ui->chatDisplay->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void chatwindow::onErrorOccurred(const QString &error) {
    showError(error);
}

void chatwindow::showError(const QString &error) {
    QMessageBox::critical(this, "Erreur de chat", error);
}

void chatwindow::closeEvent(QCloseEvent *event) {
    emit windowRole();
    event->accept();
}
