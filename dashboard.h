#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include "component.h"

namespace Ui {
class dashboard;
}

class dashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit dashboard(bool power,QWidget *parent = nullptr);
    ~dashboard();

private slots:
    void on_newRecord_clicked();

    void on_confirmRecord_clicked();

    void on_found_clicked();

    void on_removeAll_clicked();

    void on_removeRecord_clicked();

    void on_sortByLost_clicked();

    void on_sortByFound_clicked();

    void on_exportRecord_clicked();

    void on_export_last_week_clicked();

    void on_export_last_year_clicked();

private:
    Ui::dashboard *ui;
    bool isSortByLostDesc = true;
    bool isSortByFoundDesc = true;
    void update(vector<lost> &allRecord);
    int getSelectedSingleRow();
};


#endif // DASHBOARD_H
