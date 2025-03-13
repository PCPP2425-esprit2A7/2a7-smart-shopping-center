#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "event.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(int id, QWidget *parent = nullptr);
    ~Dialog();
    void setEventDetails(const Evenement &event);
    void loadEventDetails();

private slots:
    void on_enregistrer_clicked();
    void on_modifier_affiche_clicked();

private:
    Ui::Dialog *ui;
    int eventId;
};

#endif // DIALOG_H
