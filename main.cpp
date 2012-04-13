#include <QtGui/QApplication>
#include <QMessageBox>
#include "proje.h"

bool baglantiOlustur()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("proje.db");
    if (!db.open())
    {
        QMessageBox::warning(0, QObject::tr("Veritabanı Dosyası Açılamıyor"),db.lastError().text());
        return false;
    }
    return true;
}

void veritabaniYukleme()
{
    QSqlQuery query;    
    query.exec("CREATE TABLE ders (dersid VARCHAR(40) primary key NOT NULL, dersisim VARCHAR(40) NOT NULL, dersdonem VARCHAR(40) NOT NULL, dersyil VARCHAR(40) NOT NULL)");
    query.exec("CREATE TABLE ogrenci (ogrenciid VARCHAR(40) primary key NOT NULL, isim VARCHAR(40))");
    query.exec("CREATE TABLE sinav (sinavid integer primary key, sinavisim VARCHAR(40) NOT NULL, sorusayisi NUMBER NOT NULL, sinavpuan NUMBER NOT NULL)");
    query.exec("CREATE TABLE konu (konuid integer primary key,konuisim VARCHAR(40) NOT NULL, dersid VARCHAR(40) NOT NULL)");
    query.exec("CREATE TABLE soru (sinavid VARCHAR(40) NOT NULL, sorunumarasi NUMBER NOT NULL, konuisim VARCHAR(40) NOT NULL, puan NUMBER NOT NULL)");
    query.exec("CREATE TABLE sonuc (ogrenciid VARCHAR(40) NOT NULL, sinavid VARCHAR(40) NOT NULL, sorunumarasi NUMBER NOT NULL, yuzde NUMBER NOT NULL,alinanpuan NUMBER NOT NULL)");
    query.exec("CREATE TABLE derssinav (sinavid VARCHAR(40) NOT NULL, dersid VARCHAR(40) NOT NULL)");
    query.exec("CREATE TABLE dersogrenci (ogrenciid VARCHAR(40) NOT NULL, dersid VARCHAR(40) NOT NULL)");
    query.exec("CREATE TABLE sinavogrenci (ogrenciid VARCHAR(40) NOT NULL, sinavid VARCHAR(40) NOT NULL, toplampuan NUMBER NOT NULL)");
    //query.exec("CREATE TABLE sonucsinav (sinavid VARCHAR(40) NOT NULL,durum NUMBER NOT NULL)");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool create = !QFile::exists("proje.db");
    if (!baglantiOlustur())
    {
        return 1;
    }
    if (create)
    {
        veritabaniYukleme();
    }

    proje w;
    w.show();

    return a.exec();
}
