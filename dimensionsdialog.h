#ifndef DIMENSIONSDIALOG_H
#define DIMENSIONSDIALOG_H

#include <QDialog>

namespace Ui {
class DimensionsDialog;
}

class DimensionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DimensionsDialog(QWidget *parent = nullptr);
    ~DimensionsDialog();

    float getLargeur() const;
    float getHauteur() const;
    float getProfondeur() const;

private:
    Ui::DimensionsDialog *ui;
};

#endif // DIMENSIONSDIALOG_H
