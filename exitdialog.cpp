/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "exitdialog.h"
#include "ui_exitdialog.h"

#include <QSettings>
#include <QTimer>

ExitDialog::ExitDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ExitDialog) {
    ui->setupUi(this);
    count = 30;
    ui->lInfo->setText(QString("Отключение через: %1 сек").arg(count));
    QTimer::singleShot(1000, this, &ExitDialog::singleShots);
    stoped = false;
}

ExitDialog::~ExitDialog() { delete ui; }

void ExitDialog::on_pb1_clicked() { ui->le->setText(ui->le->text() + "1"); }

void ExitDialog::on_pb2_clicked() { ui->le->setText(ui->le->text() + "2"); }

void ExitDialog::on_pb3_clicked() { ui->le->setText(ui->le->text() + "3"); }

void ExitDialog::on_pb4_clicked() { ui->le->setText(ui->le->text() + "4"); }

void ExitDialog::on_pb5_clicked() { ui->le->setText(ui->le->text() + "5"); }

void ExitDialog::on_pb6_clicked() { ui->le->setText(ui->le->text() + "6"); }

void ExitDialog::on_pb7_clicked() { ui->le->setText(ui->le->text() + "7"); }

void ExitDialog::on_pb8_clicked() { ui->le->setText(ui->le->text() + "8"); }

void ExitDialog::on_pb9_clicked() { ui->le->setText(ui->le->text() + "9"); }

void ExitDialog::on_pbDot_clicked() { ui->le->setText(""); }

void ExitDialog::on_pbSlash_clicked() {
    ui->le->setText(ui->le->text().left(ui->le->text().length() - 1));
}

void ExitDialog::on_pbOk_clicked() {
    QSettings settings("JupiterSoft", "ValeraTamasha");
    if (ui->le->text() == settings.value("ExitPassword").toString()) {
        accept();
    }
}

void ExitDialog::on_pbCancel_clicked() { reject(); }

void ExitDialog::singleShots() {
    count--;
    if ((count > 0) && !stoped) {
        ui->lInfo->setText(QString("Отключение через: %1 сек").arg(count));
        QTimer::singleShot(1000, this, &ExitDialog::singleShots);
    } else if (count == 0) {
        QDialog::reject();
    }
}
