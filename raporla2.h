#ifndef RAPORLA2_H
#define RAPORLA2_H

#include <QDialog>
#include <QtSql>
#include <QLabel>
#include <QMessageBox>
#include <QKeyEvent>
#include "charts/barchartplotter.h"
#include "charts/axisbase.h"

namespace Ui {
class raporla2;
}

class raporla2 : public QDialog
{
    Q_OBJECT
    
public:
    explicit raporla2(QWidget *parent = 0);
    ~raporla2();
    void keyPressEvent(QKeyEvent *e);
    Ui::raporla2 *ui;
    bool ilkAcilis;//cbsinav'a sinavlar eklenirken fonk a girmesin
    QString dersid;
    QStandardItemModel *viewCizelge;
    QStandardItemModel *sonuc2Yuzdeler;
    void yukleme();
    void raporIkiOncesi(QString);
    void rapor2birer();
    void rapor2ikiser();
    void rapor3birer();
    void rapor3ikiser();
    void cizelgeYukleme();
    void cizelgeViewOlustur(int,QStringList);
    void konulariEkle();
    void cizelgeOlustur();
    QStringList rapor3birerYazdir(QStringList cizelgeListe, int sayac, double yuzde, QString konuIsim, int tur);
    void rapor3ikiserYazdir(int sayacIlk, int sayac, double yuzde, QString konuBir,QString konuIki, int tur);
    bool denetle();

public slots:
    void kapat();
    void raporOlustur(int);
    void raporOlusturDenetleme();
    void sinavDegisti();
    void raporTuru(int);
    void rbCizelge();
    
private:
};

#endif // RAPORLA2_H
