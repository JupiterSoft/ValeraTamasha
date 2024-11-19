/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "mainform.h"
#include "./ui_mainform.h"

#include "exitdialog.h"
#include "settingsdialog.h"
#include <QCryptographicHash>
#include <QFile>
#include <QKeyEvent>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QTextCodec>
#include <QTimer>

QString getLastExecutedQuery(const QSqlQuery &query) {
    QString sql = query.executedQuery();
    int nbBindValues = query.boundValues().size();

    for (int i = 0, j = 0; j < nbBindValues;) {
        int s = sql.indexOf(QLatin1Char('\''), i);
        i = sql.indexOf(QLatin1Char('?'), i);
        if (i < 1) {
            break;
        }

        if ((s < i) && (s > 0)) {
            i = sql.indexOf(QLatin1Char('\''), s + 1) + 1;
            if (i < 2) {
                break;
            }
        } else {
            const QVariant &var = query.boundValue(j);
            QSqlField field(QLatin1String(""), var.type());
            if (var.isNull()) {
                field.clear();
            } else {
                field.setValue(var);
            }
            QString formatV = query.driver()->formatValue(field);
            sql.replace(i, 1, formatV);
            i += formatV.length();
            ++j;
        }
    }

    return sql;
}

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
               "  name    VARCHAR(1024),"
               "  price   FLOAT,"
               "  vcode   INTEGER,"
               "  PRIMARY KEY(code)"
               ");");
    query.exec("CREATE TABLE IF NOT EXISTS bc ("
               "  barcode VARCHAR(128) NOT NULL UNIQUE,"
               "  code    VARCHAR(20),"
               "  PRIMARY KEY(barcode)"
               ");");
    _started = false;
    loadSettings();
    ui->pg->hide();
}

MainForm::~MainForm() { delete ui; }

void MainForm::search() {
    qDebug() << _text;
    QSqlQuery query;
    int wps = _wPrefix.length();
    if (_text.left(wps) == _wPrefix) {
        QString w, c;
        if ((_wS == 0) || (_wS == 2)) {
            w = _text.mid(wps, _wWeightLength);
            c = _text.mid(wps + _wWeightLength, _wCodeLength);
        } else if ((_wS == 3) || (_wS == 1)) {
            c = _text.mid(wps, _wCodeLength);
            w = _text.mid(wps + _wCodeLength, _wWeightLength);
        }
        c = QString("%1").arg(c.toInt());
        qDebug() << w << c;
        query.prepare("SELECT name, price FROM prod WHERE code = :Code");
        query.bindValue(":Code", c);
        query.exec();
        if (query.next()) {
            ui->lname->setText(query.value(0).toString());
            QString sum;
            if ((_wS == 0) || (_wS == 1)) {
                double m = w.toDouble() / 1000.0;
                w = QString("%1").arg(m, 0, 'f', 3);
                sum = QString("%1").arg(m * query.value(1).toDouble(), 0, 'f', 0);
            } else if ((_wS == 2) || (_wS == 3)) {
                double m = w.toDouble();
                w = QString("%1").arg(m, 0, 'f', 0);
                sum = QString("%1").arg(m * query.value(1).toDouble(), 0, 'f', 0);
            }
            QString str = query.value(1).toString() + "*" + w + "=" + sum + " тенге";
            ui->lprice->setText(str);
            ui->lCode->setText(c);
        } else {
            ui->lname->setText("Не найден");
            ui->lprice->setText("");
            ui->lCode->setText("");
        }
    } else {
        query.prepare("SELECT code FROM bc WHERE barcode = :BarCode");
        query.bindValue(":BarCode", _text);
        query.exec();
        if (query.next()) {
            QString code = query.value(0).toString();
            query.prepare("SELECT name, price FROM prod WHERE code = :code");
            query.bindValue(":code", code);
            query.exec();
            if (query.next()) {
                ui->lname->setText(query.value(0).toString());
                ui->lprice->setText(query.value(1).toString() + " тенге");
                ui->lCode->setText(code);
            } else {
                ui->lname->setText("Не найден");
                ui->lprice->setText("");
                ui->lCode->setText("");
            }
        } else {
            ui->lname->setText("Не найден");
            ui->lprice->setText("");
            ui->lCode->setText("");
        }
    }
    _text = "";
}

void MainForm::on_pbExit_clicked() {
    ExitDialog *dialog = new ExitDialog(this);
    if (dialog->exec() == QDialog::Accepted) {
        delete dialog;
        close();
    } else {
        delete dialog;
    }
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
        _flagFile = settings.value("FlagFile").toString();
        _dataFile = settings.value("DataFile").toString();
        QFont font = ui->lname->font();
        font.setPointSize(settings.value("NameFont").toInt());
        ui->lname->setFont(font);
        font = ui->lprice->font();
        font.setPointSize(settings.value("PriceFont").toInt());
        ui->lprice->setFont(font);

        _wS = settings.value("WS").toInt();
        _wPrefix = settings.value("WPrefix").toString();
        _wCodeLength = settings.value("WCodeLength").toInt();
        _wWeightLength = settings.value("WWeightLength").toInt();

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
        settings.setValue("WPrefix", "99");
        settings.setValue("WCodeLength", 5);
        settings.setValue("WWeightLength", 5);
        settings.setValue("WS", 1);
    }
}

void MainForm::singleShot() {
    // todo
    qDebug() << "Single shot";

    if (QFile::exists(_flagFile)) {
        QFile file(_dataFile);
        if (file.open(QIODevice::ReadOnly)) {
            QSqlQuery query;
            QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
            int mode = 0;
            int count = file.size() / 80;
            QByteArray array = file.readLine();
            QString line = codec->toUnicode(array);
            count = line.toInt();
            ui->pg->show();
            ui->pg->setMaximum(count);
            ui->pg->setValue(0);
            count = 0;
            while (true) {
                QByteArray array = file.readLine();
                ui->pg->update();
                this->update();
                QCoreApplication::processEvents();
                if (array.size() > 0) {
                    QString line = codec->toUnicode(array);
                    if (line == "$$$MODE1\r\n") {
                        mode = 1;
                        continue;
                    } else if (line == "$$$MODE2\r\n") {
                        mode = 2;
                        continue;
                    }
                    ui->pg->setValue(++count);
                    if (count > ui->pg->maximum()) {
                        ui->pg->setMaximum(count + 20);
                    }
                    QStringList list = line.split(";");
                    if (mode == 1) {
                        QString code = list[0];
                        QString name = list[1];
                        QString price = list[2];
                        QString wc = list[3];
                        if (!price.isEmpty()) {
                            query.prepare(
                                "INSERT OR REPLACE INTO prod (code,name,price,vcode) VALUES"
                                " (:code, :name, :price, :wcode)");
                            query.bindValue(":code", code);
                            query.bindValue(":name", name);
                            query.bindValue(":price", price.toDouble());
                            query.bindValue(":wcode", wc.toInt());
                            if (!query.exec(/*str*/)) {
                                qDebug() << query.lastError().text();
                                qDebug() << getLastExecutedQuery(query);
                                // qDebug() << str;
                            }
                        }
                    } else if (mode == 2) {
                        QString bc = list[0];
                        QString code = list[1];
                        query.prepare(
                            "INSERT OR REPLACE INTO bc (barcode, code) VALUES (:bc, :code)");
                        query.bindValue(":code", code);
                        query.bindValue(":bc", bc);
                        if (!query.exec()) {
                            qDebug() << query.lastError().text();
                            qDebug() << getLastExecutedQuery(query);
                        }
                    }
                } else
                    break;
            }
            ui->pg->hide();
        }
        QFile::remove(_flagFile);
    }
    if (_timeUpdate != 0) {
        _started = true;
        QTimer::singleShot(_timeUpdate * 1000, this, &MainForm::singleShot);
    } else {
        _started = false;
    }
}
// 4690228103179
