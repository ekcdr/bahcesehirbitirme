#ifndef EKLEOGRENCI_H
#define EKLEOGRENCI_H

#include <QDialog>

namespace Ui {
    class ekleOgrenci;
}

class ekleOgrenci : public QDialog
{
    Q_OBJECT

public:
    explicit ekleOgrenci(QWidget *parent = 0);
    ~ekleOgrenci();
    Ui::ekleOgrenci *ui;
    void uyari(int i);

public slots:
    void kapat();

private:

};

#endif // EKLEOGRENCI_H
