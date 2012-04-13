#ifndef VERIGIRISI_H
#define VERIGIRISI_H

#include <QDialog>

namespace Ui {
class veriGirisi;
}

class veriGirisi : public QDialog
{
    Q_OBJECT
    
public:
    explicit veriGirisi(QWidget *parent = 0);
    ~veriGirisi();
    Ui::veriGirisi *ui;

public slots:
    void kapat();
    
private:

};

#endif // VERIGIRISI_H
