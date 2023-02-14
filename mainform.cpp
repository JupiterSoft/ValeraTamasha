/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "mainform.h"
#include "./ui_mainform.h"

#include "settingsdialog.h"
#include <QCryptographicHash>
#include <QFile>
#include <QKeyEvent>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>

MainForm::MainForm(QWidget *parent) : QWidget(parent), ui(new Ui::MainForm) {

    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("vt.db");
    if (!db.open()) {
        qDebug() << "Error not open database";
    }
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS prod ("
               "  code VARCHAR(20) NOT NULL UNIQUE,"
               "  barcode VARCHAR(128),"
               "  name    VARCHAR(1024),"
               "  price   FLOAT,"
               "  vcode   INTEGER,"
               "  PRIMARY KEY(code)"
               ");");
    query.exec("INSERT OR REPLACE INTO prod VALUES (\'1\',\'123\',\'TEST\',100,0)");
    _started = false;
    loadSettings();
}

MainForm::~MainForm() { delete ui; }

void MainForm::search() {
    qDebug() << _text;
    QSqlQuery query;
    query.prepare("SELECT name, price FROM prod WHERE barcode = :BarCode");
    query.bindValue(":BarCode", _text);
    query.exec();
    if (query.next()) {
        ui->lname->setText(query.value(0).toString());
        ui->lprice->setText("Цена: " + query.value(1).toString() + " тенге");
    } else {
        ui->lname->setText("Не найден");
        ui->lprice->setText("Цена: ");
    }
    _text = "";
}

void MainForm::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        search();
        break;
    case Qt::Key_1:
        _text += "1";
        break;
    case Qt::Key_2:
        _text += "2";
        break;
    case Qt::Key_3:
        _text += "3";
        break;
    case Qt::Key_4:
        _text += "4";
        break;
    case Qt::Key_5:
        _text += "5";
        break;
    case Qt::Key_6:
        _text += "6";
        break;
    case Qt::Key_7:
        _text += "7";
        break;
    case Qt::Key_8:
        _text += "8";
        break;
    case Qt::Key_9:
        _text += "9";
        break;
    case Qt::Key_0:
        _text += "0";
        break;
    case Qt::Key_A:
        qDebug() << "A";
        if (event->modifiers() == Qt::ShiftModifier) {
            // settings
            qDebug() << "Ctrl";
            SettingsDialog *dlg = new SettingsDialog(this);
            if (dlg->exec() == QDialog::Accepted) {
                loadSettings();
            }
            delete dlg;
        }
        break;
    }
}

void MainForm::loadSettings() {
    QSettings settings("JupiterSoft", "ValeraTamasha");
    if (settings.contains("Password")) {
        _timeUpdate = settings.value("TimeUpdate").toInt();
        _flagFile = settings.value("FileFlag").toString();
        _dataFile = settings.value("DataFile").toString();
        QFont font = ui->lname->font();
        font.setPointSize(settings.value("NameFont").toInt());
        ui->lname->setFont(font);
        font = ui->lprice->font();
        font.setPointSize(settings.value("PriceFont").toInt());
        ui->lprice->setFont(font);
        if (!_started && (_timeUpdate != 0)) {
            _started = true;
            QTimer::singleShot(_timeUpdate * 1000, this, &MainForm::singleShot);
        }
    } else {
        QByteArray hash =
            QCryptographicHash::hash(QString("112233").toUtf8(), QCryptographicHash::Sha256);
        settings.setValue("Password", hash);
        settings.setValue("TimeUpdate", 0);
        settings.setValue("FileFlag", "");
        settings.setValue("DataFile", "");
        settings.setValue("NameFont", 40);
        settings.setValue("PriceFont", 60);
    }
}

void MainForm::singleShot() {
    // todo
    qDebug() << "Single shot";
    if (QFile::exists(_flagFile)) {

        QFile::remove(_flagFile);
    }
    if (_timeUpdate != 0) {
        _started = true;
        QTimer::singleShot(_timeUpdate * 1000, this, &MainForm::singleShot);
    } else {
        _started = false;
    }
}
