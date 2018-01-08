#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->setWindowTitle("Cluster Search Settings");
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pushButton_clicked()
{
    this->hide();
    emit sendParams(ui->MagT->text().toDouble(),
                    ui->DistT->text().toDouble(),
                    ui->TimeT->text().toInt(),
                    ui->spinBox->value(),
                    ui->checkBox->isChecked());
}
