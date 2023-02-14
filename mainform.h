/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainForm;
}
QT_END_NAMESPACE

class MainForm : public QWidget {
    Q_OBJECT

  public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();

    void search();

  protected slots:
    virtual void keyPressEvent(QKeyEvent *event);
    void loadSettings();

    void singleShot();

  private:
    Ui::MainForm *ui;
    bool _started;

    QString _text;
    int _timeUpdate;
    QString _dataFile;
    QString _flagFile;
};
#endif // MAINFORM_H
