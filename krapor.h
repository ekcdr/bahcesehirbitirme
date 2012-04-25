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
    void yukleme2();
    void kendiligindenRapor1();
    void kendiligindenRapor2();
    void sonucRapor1();
    void sonucRapor2();
    void cizelgeYukleme();
    void cizelgeViewOlustur(int,QStringList);
    void konulariEkle();
    void cizelgeOlustur();
    bool denetle();

public slots:
    void kapat();
    void raporOlustur(int);
    void raporOlustur2();
    void sinavDegisti();
    void raporTuru(int);
    void rbCizelge();

private:

};

#endif // KRAPOR_H
