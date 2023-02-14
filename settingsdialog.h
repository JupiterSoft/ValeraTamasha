/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

  public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

  public slots:
    void on_pbCheck_clicked();
    virtual void accept();

  private:
    Ui::SettingsDialog *ui;
    bool _isOpened;
};

#endif // SETTINGSDIALOG_H
