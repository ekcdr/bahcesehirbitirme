#ifndef KRAPOR_H
#define KRAPOR_H

#include <QDialog>
#include <QtSql>
#include <QLabel>
#include <QMessageBox>
#include <QKeyEvent>
#include "charts/barchartplotter.h"
#include "charts/axisbase.h"

namespace Ui {
class kRapor;
}

class kRapor : public QDialog
{
    Q_OBJECT
    
public:
    explicit kRapor(QWidget *parent = 0);
    ~kRapor();
    Ui::kRapor *ui;
    void keyPressEvent(QKeyEvent *e);
    bool ilkAcilis;//cbsinav'a sinavlar eklenirken fonk a girmesin
    QString dersid;
    QStandardItemModel *viewCizelge;
    QStandardItemModel *sonuc2Yuzdeler;
    void yukleme();
    void kraporOncesi(QString);
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

#endif // KRAPOR_H
