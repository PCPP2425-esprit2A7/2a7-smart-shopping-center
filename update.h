#ifndef UPDATE_H
#define UPDATE_H

#include <QWidget>
#include <QString>

namespace Ui {
class update;
}

class update : public QWidget
{
    Q_OBJECT

public:
    explicit update(QWidget *parent = nullptr);  // Constructor
    ~update();  // Destructor

    // Method to set values in the update dialog
    void setValues(int id, QString nom, QString type, int superficie, QString emplacement, QString statut, double loyer, QString dateDebut, QString dateFin);

private slots:
    // Slot for the update button click event
    void on_update_2_clicked();

private:
    Ui::update *ui;  // Pointer to the UI
    int espaceID;  // Variable to store the ID of the space being updated
};

#endif // UPDATE_H
