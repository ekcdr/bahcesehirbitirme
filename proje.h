#ifndef PROJE_H
#define PROJE_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTextCodec>
#include <QtSql>
#include <QTableWidgetItem>
#include <QCheckBox>
#include "ekleogrenci.h"
#include "ekleders.h"
#include "eklesinav.h"
#include "raporla.h"
#include "veritabanigoster.h"
#include "verigirisi.h"
#include "eklesonuc.h"
#include "listele.h"
#include "krapor.h"
#include "sinavistatistik.h"

namespace Ui {
    class proje;
}

class proje : public QMainWindow
{
    Q_OBJECT

public:
    explicit proje(QWidget *parent = 0);
    ~proje();
    void yukleme();
    void ogrenciSayisiBul(QString ders); //dersi alan öğrenci sayısını döndürüyor.
    void sinavSayisiBul(QString ders); //dersin sinav sayısını döndürüyor.
    void kalinFont(QTableWidgetItem *itm);
    void closeEvent(QCloseEvent *event);

public slots:
    void tamamOgrenci();
    void tamamSinav();
    void tamamDers();
    void tamamSonuc2();
    void fEkleOgrenci();
    void fEkleDers();
    void fEkleSinav();
    void fEkleSonuc();
    void dersleriYazdir(); //dersleri ana ekrana yazdırıyor
    void raporDers();
    void filtreSinav();
    //void raporOlustur();
    void raporOlustur2();
    void veritabani();
    void veritabaniGuncelle();
    void veriGirisiYap(int i, int j);
    void sonucDoldur();//sınav sonuclarını tablewidget e dolduruyor
    void ogrenciListele();
    void sinavListele();
    void sonucKapat();
    void krapor();
    void konuEtkin();
    void sinavGrafikGoster();

private:
    Ui::proje *ui;
    QSqlRelationalTableModel *modelOgrenci;
    QSqlRelationalTableModel *modelKonu;
    QSqlRelationalTableModel *modelSinav;
    QSqlRelationalTableModel *modelDers;
    QSqlRelationalTableModel *modelDersSinav;
    QSqlRelationalTableModel *modelDersOgrenci;
    QSqlRelationalTableModel *modelSoru;
    QSqlRelationalTableModel *modelSonuc;
    QSqlRelationalTableModel *modelSinavOgrenci;
    QSqlRelationalTableModel *modelSonucSinav;

    ekleOgrenci formEkleOgrenci;
    ekleDers formEkleDers;
    ekleSinav formEkleSinav;
    raporla formRaporla;
    veritabaniGoster formVeritabani;
    veriGirisi formVeriGirisi;
    ekleSonuc formEkleSonuc;
    listele formListele;
    kRapor formKRapor;
    sinavIstatistik formSinavIstatistik;
};

#endif // PROJE_H
