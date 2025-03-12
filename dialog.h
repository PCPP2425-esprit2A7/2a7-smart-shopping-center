#ifndef DIALOG_H
#define DIALOG_H

#include "locataire.h"
#include <QDialog>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(int id, QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_saveButton_clicked();

private:
    Ui::Dialog *ui;  // Utiliser la classe générée par uic
    int locataireId;
    void loadLocataireDetails();
};
#endif // MODIFIERLOCATAIREDIALOG_H
