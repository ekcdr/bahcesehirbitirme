#ifndef VERIGIRISI_H
#define VERIGIRISI_H

#include <QDialog>
#include <QKeyEvent>

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
    void keyPressEvent(QKeyEvent *e);

public slots:
    void kapat();
    
private:

};

#endif // VERIGIRISI_H
