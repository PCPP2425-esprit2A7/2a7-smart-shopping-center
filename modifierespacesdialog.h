#ifndef MODIFIERESPACESDIALOG_H
#define MODIFIERESPACESDIALOG_H

#include <QDialog>
#include "espace.h"

namespace Ui {
class modifierespacesDialog;
}

class modifierespacesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit modifierespacesDialog(int id, QWidget *parent = nullptr);
    ~modifierespacesDialog();

private slots:
    void on_enregistrer_clicked();

private:
    Ui::modifierespacesDialog *ui;
    int espaceID;
    Espace espace;

    void chargerDonnees();
};

#endif // MODIFIERESPACESDIALOG_H
