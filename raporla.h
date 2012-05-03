#ifndef RAPORLA_H
#define RAPORLA_H

#include <QDialog>
#include <QModelIndex>
#include <QKeyEvent>
#include <QTextCodec>
#include <QtSql>
#include <QCheckBox>

namespace Ui {
    class raporla;
}

class raporla : public QDialog
{
    Q_OBJECT

public:
    explicit raporla(QWidget *parent = 0);
    ~raporla();
    Ui::raporla *ui;
    void keyPressEvent(QKeyEvent *e);
    void yukleme();
    void kapat();
    void raporBirOncesi(QString);
    QString dersID;

public slots:
    //void filtreKonu(QString q);//cbKonu nun içeriği --- olursa txtKonuPuan'ı devredışı bırakıyor. ve lblPuanı değiştiriyor
    void sirala(int);
    void yazdir();
    void filtreSinav();//rapor1 de sınav değişince konuları dolduruyor
    void konuEtkin();//rapor1 de konu başındaki tik var mı diye bakıp etkinlik durumunu değiştiriyor

private:

};

#endif // RAPORLA_H
