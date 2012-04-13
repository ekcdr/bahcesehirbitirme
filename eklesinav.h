#ifndef EKLESINAV_H
#define EKLESINAV_H

#include <QDialog>

namespace Ui {
    class ekleSinav;
}

class ekleSinav : public QDialog
{
    Q_OBJECT

public:
    explicit ekleSinav(QWidget *parent = 0);
    ~ekleSinav();
    Ui::ekleSinav *ui;
    void uyari(int i);
    void yukleme();
    bool bosVar;//konu sutununda bos alan var mı

public slots:
    void doldur();
    void puanKontrol(int i,int j); //puanın int olup olmadığını kontrol ediyor
    void kapat();

private:
};

#endif // EKLESINAV_H
