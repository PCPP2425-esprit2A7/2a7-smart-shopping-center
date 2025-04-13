#include "dimensionsdialog.h"
#include "ui_dimensionsdialog.h"

DimensionsDialog::DimensionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DimensionsDialog)
{
    ui->setupUi(this);
    setWindowTitle("Dimensions de l'espace");
    setModal(true);
}

DimensionsDialog::~DimensionsDialog()
{
    delete ui;
}

float DimensionsDialog::getLargeur() const
{
    return ui->inputLargeur->text().toFloat();
}

float DimensionsDialog::getHauteur() const
{
    return ui->inputHauteur->text().toFloat();
}

float DimensionsDialog::getProfondeur() const
{
    return ui->inputProfondeur->text().toFloat();
} 