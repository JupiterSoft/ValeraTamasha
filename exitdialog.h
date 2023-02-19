/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef EXITDIALOG_H
#define EXITDIALOG_H

#include <QDialog>

namespace Ui {
    class ExitDialog;
}

class ExitDialog : public QDialog {
    Q_OBJECT

  public:
    explicit ExitDialog(QWidget *parent = nullptr);
    ~ExitDialog();

  protected slots:
    void on_pb1_clicked();
    void on_pb2_clicked();
    void on_pb3_clicked();
    void on_pb4_clicked();
    void on_pb5_clicked();
    void on_pb6_clicked();
    void on_pb7_clicked();
    void on_pb8_clicked();
    void on_pb9_clicked();
    void on_pbDot_clicked();
    void on_pbSlash_clicked();
    void on_pbOk_clicked();
    void on_pbCancel_clicked();

    void singleShots();

  private:
    Ui::ExitDialog *ui;
    int count;
    bool stoped;
};

#endif // EXITDIALOG_H
