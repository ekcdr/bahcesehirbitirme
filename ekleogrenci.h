#ifndef EKLEOGRENCI_H
#define EKLEOGRENCI_H

#include <QDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTextCodec>
#include <QtSql>

namespace Ui {
    class ekleOgrenci;
}

class ekleOgrenci : public QDialog
{
    Q_OBJECT

public:
    explicit ekleOgrenci(QWidget *parent = 0);
    ~ekleOgrenci();
    Ui::ekleOgrenci *ui;
    void keyPressEvent(QKeyEvent *e);
    void uyari(int i);
    void ogrenciEklemeOncesi(QString);
    bool tamam(QString ogrenciDers);

public slots:
    void kapat();

private:

};

#endif // EKLEOGRENCI_H
