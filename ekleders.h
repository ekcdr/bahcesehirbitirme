#ifndef EKLEDERS_H
#define EKLEDERS_H

#include <QDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDate>
#include <QtSql>

namespace Ui {
    class ekleDers;
}

class ekleDers : public QDialog
{
    Q_OBJECT

public:
    explicit ekleDers(QWidget *parent = 0);
    ~ekleDers();
    Ui::ekleDers *ui;
    void keyPressEvent(QKeyEvent *e);
    void uyari(int i);
    void dersEklemeOncesi();
    bool tamam();

public slots:
    void yilEksilt();
    void yilArttir();
    void kapat();

private:

};

#endif // EKLEDERS_H
