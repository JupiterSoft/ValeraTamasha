/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QCryptographicHash>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog) {
    ui->setupUi(this);
    _isOpened = false;
    ui->gbData->setEnabled(false);
}

SettingsDialog::~SettingsDialog() { delete ui; }

void SettingsDialog::on_pbCheck_clicked() {
    QSettings settings("JupiterSoft", "ValeraTamasha");
    QByteArray arr =
        QCryptographicHash::hash(ui->lePassword->text().toUtf8(), QCryptographicHash::Sha256);
    if (settings.value("Password").toByteArray() == arr) {
        _isOpened = true;
        ui->gbData->setEnabled(true);
        ui->sbTimeout->setValue(settings.value("TimeUpdate").toInt());
        ui->sbName->setValue(settings.value("NameFont").toInt());
        ui->sbPrice->setValue(settings.value("PriceFont").toInt());
        ui->leFileFlag->setText(settings.value("FlagFile").toString());
        ui->leDataFile->setText(settings.value("DataFile").toString());
    }
}

void SettingsDialog::accept() {

    if (_isOpened) {
        QSettings settings("JupiterSoft", "ValeraTamasha");
        settings.setValue("TimeUpdate", ui->sbTimeout->value());
        settings.setValue("NameFont", ui->sbName->value());
        settings.setValue("PriceFont", ui->sbPrice->value());
        settings.setValue("FlagFile", ui->leFileFlag->text());
        settings.setValue("DataFile", ui->leDataFile->text());
        if (!ui->leNewPassword->text().isEmpty()) {
            QByteArray arr = QCryptographicHash::hash(ui->leNewPassword->text().toUtf8(),
                                                      QCryptographicHash::Sha256);
            settings.setValue("Password", arr);
        }
    }

    QDialog::accept();
}
