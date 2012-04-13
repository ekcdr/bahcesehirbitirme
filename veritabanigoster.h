#ifndef VERITABANIGOSTER_H
#define VERITABANIGOSTER_H

#include <QDialog>

namespace Ui {
class veritabaniGoster;
}

class veritabaniGoster : public QDialog
{
    Q_OBJECT
    
public:
    explicit veritabaniGoster(QWidget *parent = 0);
    ~veritabaniGoster();
    Ui::veritabaniGoster *ui;

public slots:
    void kapat();
    
private:
};

#endif // VERITABANIGOSTER_H
