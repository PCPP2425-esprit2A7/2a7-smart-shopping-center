#ifndef MODIFIERDIALOG_H
#define MODIFIERDIALOG_H

#include <QDialog>
#include "employe.h"

namespace Ui {
class ModifierDialog;
}

class ModifierDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifierDialog(QWidget *parent = nullptr);
    ~ModifierDialog();

    void setEmploye(const Employe &employe);
    Employe getEmployeModifie() const; // ✅ Ajout de la déclaration correcte
    void accept() override; // ✅ Déclaration de la fonction accept()

private slots:
    void on_pushButton_modifierPhoto_clicked();
    void on_pushButton_enregistrer_clicked();
    void trierEmployes(int columnIndex);  // 📌 Vérifie bien que cette ligne est présente !


private:
    Ui::ModifierDialog *ui;
    QString cheminImage; // Pour stocker le chemin de la photo de profil
    Employe employe;
    Employe employeModifie;


};

#endif // MODIFIERDIALOG_H
