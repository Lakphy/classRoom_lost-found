#ifndef INDEX_H
#define INDEX_H

#include <QMainWindow>
#include "component.h"

namespace Ui {
class index;
}

class index : public QMainWindow
{
    Q_OBJECT

public:
    explicit index(QWidget *parent = nullptr);
    ~index();

private:
    Ui::index *ui;
};

#endif // INDEX_H
