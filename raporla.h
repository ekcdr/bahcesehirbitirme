#ifndef RAPORLA_H
#define RAPORLA_H

#include <QDialog>
#include <QModelIndex>
#include <QKeyEvent>

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
    //QString dersID;

public slots:
    //void filtreKonu(QString q);//cbKonu nun içeriği --- olursa txtKonuPuan'ı devredışı bırakıyor. ve lblPuanı değiştiriyor
    void sirala(int);

private:

};

#endif // RAPORLA_H
