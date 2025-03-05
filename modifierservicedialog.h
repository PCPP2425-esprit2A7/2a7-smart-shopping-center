#ifndef MODIFIERSERVICEDIALOG_H
#define MODIFIERSERVICEDIALOG_H

#include <QDialog>

namespace Ui {
class ModifierServiceDialog;
}

class ModifierServiceDialog : public QDialog  // Vérifie que ça hérite bien de QDialog
{
    Q_OBJECT

public:
    explicit ModifierServiceDialog(int id, QWidget *parent = nullptr);
    ~ModifierServiceDialog();

private slots:
    void on_saveButton_clicked();

private:
    Ui::ModifierServiceDialog *ui;
    int serviceId; // Stocke l'ID du service sélectionné
    void loadServiceDetails();
};

#endif // MODIFIERSERVICEDIALOG_H
