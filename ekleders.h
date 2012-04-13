#ifndef EKLEDERS_H
#define EKLEDERS_H

#include <QDialog>

namespace Ui {
    class ekleDers;
}

class ekleDers : public QDialog
{
    Q_OBJECT

public:
    explicit ekleDers(QWidget *parent = 0);
    ~ekleDers();
    Ui::ekleDers *ui;
    void uyari(int i);

public slots:
    void yilEksilt();
    void yilArttir();
    void kapat();

private:

};

#endif // EKLEDERS_H
