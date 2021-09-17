#include "index.h"
#include "ui_index.h"

using namespace std;

index::index(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::index)
{
    ui->setupUi(this);
//    if(power){
//        ui->confirmRecord->setDisabled(false);
//        ui->manageAdmin->setDisabled(false);
//        ui->found->setDisabled(false);
//        ui->removeRecord->setDisabled(false);\
//        ui->removeAll->setDisabled(false);
//    }
    ui->recordTable->setColumnCount(8);
    ui->recordTable->setColumnWidth(0,120);
    ui->recordTable->setColumnWidth(1,120);
    ui->recordTable->setColumnWidth(2,120);
    ui->recordTable->setColumnWidth(3,120);
    ui->recordTable->setColumnWidth(4,120);
    ui->recordTable->setColumnWidth(5,120);
    ui->recordTable->setColumnWidth(6,120);
    ui->recordTable->setColumnWidth(7,200);
    ui->recordTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->recordTable->setSelectionMode ( QAbstractItemView::SingleSelection);
    ui->recordTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->recordTable->verticalHeader()->setVisible(true);
//    QHeaderView *header = ui->recordTable->horizontalHeader();
//    header->setSortIndicator(1,Qt::AscendingOrder);
//    header->setSortIndicator(1,Qt::DescendingOrder);
//    header->setSortIndicator(6,Qt::AscendingOrder);
//    header->setSortIndicator(6,Qt::DescendingOrder);
//    header->setSortIndicatorShown(true);
//    connect(header, SIGNAL(sectionClicked(int)), ui->recordTable, SLOT (sortByColumn(int)));
    QStringList sListHeader;
    sListHeader<<"物品名称"<<"丢失时间"<<"丢失地点"<<"状态"<<"认领人姓名"<<"认领人学号"<<"认领人学号"<<"认领时间";
    ui->recordTable->setHorizontalHeaderLabels(sListHeader);
    vector<Record> allRecord = Util::getStorageSync<Record>("record");
    this->syncTable(allRecord);
}


index::~index()
{
    delete ui;
}
