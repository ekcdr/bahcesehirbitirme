#ifndef LISTELE_H
#define LISTELE_H

#include <QDialog>

namespace Ui {
class listele;
}

class listele : public QDialog
{
    Q_OBJECT
    
public:
    explicit listele(QWidget *parent = 0);
    ~listele();
    Ui::listele *ui;
    void yuklemeOgrenci();
    void yuklemeSinav();

public slots:
    void kapat();
    
private:
};

#endif // LISTELE_H
