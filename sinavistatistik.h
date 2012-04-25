#ifndef SINAVISTATISTIK_H
#define SINAVISTATISTIK_H

#include <QDialog>
#include <QtSql>
#include "charts/barchartplotter.h"
#include "charts/axisbase.h"

namespace Ui {
class sinavIstatistik;
}

class sinavIstatistik : public QDialog
{
    Q_OBJECT
    
public:
    explicit sinavIstatistik(QWidget *parent = 0);
    ~sinavIstatistik();
    Ui::sinavIstatistik *ui;
    QStandardItemModel *viewCizelge;
    void cizelgeYukleme(int);
    //void cizelgeOlustur(QString, int, QString, QStringList);
    void cizelgeOlustur();
    void ilkYukleme();

public slots:
    void kapat();
    
private:
};

#endif // SINAVISTATISTIK_H
