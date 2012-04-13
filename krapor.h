#ifndef KRAPOR_H
#define KRAPOR_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
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
    bool ilkAcilis;//cbsinav'a sinavlar eklenirken fonk a girmesin
    QString dersid;
    QStandardItemModel *viewCizelge;
    void yukleme();
    void kendiligindenRapor1();
    void kendiligindenRapor2();
    void sonucRapor1();
    void sonucRapor2();
    void cizelgeYukleme();
    void cizelgeOlustur(int,QStringList);
    bool denetle();

public slots:
    void kapat();
    void raporOlustur(int);
    void raporOlustur2();
    void sinavDegisti();
    void raporTuru(int);

private:

};

#endif // KRAPOR_H
