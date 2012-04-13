#ifndef RAPORLA_H
#define RAPORLA_H

#include <QDialog>
#include <QModelIndex>

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
    void yukleme();
    QString dersID;

public slots:
    //void filtreKonu(QString q);//cbKonu nun içeriği --- olursa txtKonuPuan'ı devredışı bırakıyor. ve lblPuanı değiştiriyor
    void sirala(int);

private:

};

#endif // RAPORLA_H
