#include "dashboard.h"
#include "ui_dashboard.h"
#include "QInputDialog.h"
#include "QMessageBox.h"
#include <windows.h>

using namespace std;

dashboard::dashboard(bool power,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dashboard)
{
    ui->setupUi(this);
    this->setWindowTitle("机房失物招领");
    if(power){
        ui->confirmRecord->setDisabled(false);
        ui->found->setDisabled(false);
        ui->removeRecord->setDisabled(false);
        ui->removeAll->setDisabled(false);
    }
    ui->recordTable->setColumnCount(8);
    ui->recordTable->setColumnWidth(0,120);
    ui->recordTable->setColumnWidth(1,140);
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
    QStringList sListHeader;
    sListHeader<<"物品名称"<<"丢失时间"<<"丢失地点"<<"状态"<<"认领人姓名"<<"认领人手机号"<<"认领人学号"<<"认领时间";
    ui->recordTable->setHorizontalHeaderLabels(sListHeader);
    vector<lost> allRecord = Util::getStorageSync<lost>("record");
    this->update(allRecord);
}


dashboard::~dashboard()
{
    delete ui;
}

// 更新表格
void dashboard::update(vector<lost> &allRecord){
    int length = allRecord.size();
    ui->recordTable->setRowCount(length);
    for(int i=0;i<length;i++){
        string name = allRecord[i].getName();
        string lostTime = allRecord[i].getLostDay();
        string place = allRecord[i].getPlace();
        string state = allRecord[i].state2String();
        string personName =allRecord[i].getStuName();
        string personPhone =allRecord[i].getPhoneNumber();
        string personId =allRecord[i].getStuId();
        string pickUpTime =allRecord[i].getPickUpTime();
        ui->recordTable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(name)));
        ui->recordTable->setItem(i,1,new QTableWidgetItem(QString::fromStdString(lostTime)));
        ui->recordTable->setItem(i,2,new QTableWidgetItem(QString::fromStdString(place)));
        ui->recordTable->setItem(i,3,new QTableWidgetItem(QString::fromStdString(state)));
        ui->recordTable->setItem(i,4,new QTableWidgetItem(QString::fromStdString(personName)));
        ui->recordTable->setItem(i,5,new QTableWidgetItem(QString::fromStdString(personPhone)));
        ui->recordTable->setItem(i,6,new QTableWidgetItem(QString::fromStdString(personId)));
        ui->recordTable->setItem(i,7,new QTableWidgetItem(QString::fromStdString(pickUpTime)));
    }
}


int dashboard::getSelectedSingleRow(){
    QList<QTableWidgetSelectionRange> ranges =ui->recordTable->selectedRanges();
    int length = ranges.length();
    if(length!=0){
        return ranges[0].topRow();
    }
    else{
        return -1;
    }
}

void dashboard::on_newRecord_clicked()
{
    bool nameOk,placeOk;
    string nameText = QInputDialog::getText(NULL, "新建记录","请输入物品名称",QLineEdit::Normal,"物品名称",&nameOk).toStdString().c_str();
    if(!nameOk) return;
    string placeText = QInputDialog::getText(NULL, "新建记录","请输入丢失地点",QLineEdit::Normal,"物品地点",&placeOk).toStdString().c_str();
    if(!placeOk) return;
    time_t t=time(0);
    tm *ltm = localtime(&t);
    lost a(nameText,placeText,ltm);
    vector<lost> allRecord = Util::getStorageSync<lost>("record");
    allRecord.push_back(a);
    Util::setStorageSync("record",allRecord);
    this->update(allRecord);
    QMessageBox::about(NULL, "创建成功", "记录创建成功");
}

void dashboard::on_found_clicked()
{
    //点击认领
    vector<lost> allRecord = Util::getStorageSync<lost>("record");
    if(allRecord[this->getSelectedSingleRow()].getState()!=2){
        QMessageBox::about(NULL, "出错啦", "需管理老师确认才可认领物品");
        return;
    }
    bool nameOk,phoneOk,idOk;
    string nameText = QInputDialog::getText(NULL, "认领物品","请输入认领人名称",QLineEdit::Normal,"认领人名称",&nameOk).toStdString();
    if(!nameOk) return;
    string phoneText = QInputDialog::getText(NULL, "认领物品","请输入认领人手机号",QLineEdit::Normal,"认领人手机号",&phoneOk).toStdString();
    if(!phoneOk) return;
    string idText = QInputDialog::getText(NULL, "认领物品","请输入认领人学号",QLineEdit::Normal,"认领人学号",&idOk).toStdString();
    if(!idOk) return;
    time_t t=time(0);
    tm *ltm = localtime(&t);
    allRecord[this->getSelectedSingleRow()].pickUp(nameText,phoneText,idText,*ltm);
    Util::setStorageSync<lost>("record",allRecord);
    this->update(allRecord);
    QMessageBox::about(NULL, "认领成功", "认领成功");
}

void dashboard::on_confirmRecord_clicked()
{
    //点击确认记录
    vector<lost> allRecord = Util::getStorageSync<lost>("record");
    allRecord[this->getSelectedSingleRow()].verify();
    Util::setStorageSync<lost>("record",allRecord);
    this->update(allRecord);
}

void dashboard::on_removeAll_clicked()
{
    vector<lost> allRecord;
    Util::setStorageSync<lost>("record",allRecord);
    this->update(allRecord);
    QMessageBox::about(NULL, "所有记录已删除", "所有记录已删除");
}

void dashboard::on_removeRecord_clicked()
{
    int index = this->getSelectedSingleRow();
    if(index!=-1){
        vector<lost> allRecord = Util::getStorageSync<lost>("record");
        allRecord.erase(allRecord.begin()+index);
        Util::setStorageSync<lost>("record",allRecord);
        this->update(allRecord);
        QMessageBox::about(NULL, "记录已删除", "记录已删除");
    }
    else{
        QMessageBox::about(NULL, "出错啦", "请选择一项记录");
    }
}

void dashboard::on_sortByLost_clicked()
{
    // 点击按丢失时间排序
    vector<lost> allRecord = Util::getStorageSync<lost>("record");
    sort(allRecord.begin(),allRecord.end(),lost::lostTimeComp);
    if(this->isSortByLostDesc){
       reverse(allRecord.begin(),allRecord.end());
    }
    this->isSortByLostDesc = !this->isSortByLostDesc;
    this->update(allRecord);
}

void dashboard::on_sortByFound_clicked()
{
    // 点击按拾取时间排序
    vector<lost> allRecord = Util::getStorageSync<lost>("record");
    sort(allRecord.begin(),allRecord.end(),lost::pickUpTimeComp);
    if(this->isSortByFoundDesc){
        reverse(allRecord.begin(),allRecord.end());
    }
    this->isSortByFoundDesc = !this->isSortByFoundDesc;
    this->update(allRecord);
}

void dashboard::on_exportRecord_clicked()
{
    // 点击导出文本
    vector<lost> allRecord = Util::getStorageSync<lost>("record");
    Util::generater(allRecord,"total_record");
    QMessageBox::about(NULL, "导出成功", "所有遗失物品记录已成功导出至total_record.txt文件！");
    system("total_record.txt");
}

void dashboard::on_export_last_week_clicked()
{
    vector<lost> allRecord = Util::getStorageSync<lost>("record");
    vector<lost> lastWeek;
    for(int i=0,length=allRecord.size();i<length;i++){
        if(allRecord[i].isLastWeekLost()){
            lastWeek.push_back(allRecord[i]);
        }
    }
    Util::generater(lastWeek,"last_weeek");
    QMessageBox::about(NULL, "导出成功", "上周遗失物品记录已成功导出至last_weeek.txt文件！");
    system("last_weeek.txt");
}

void dashboard::on_export_last_year_clicked()
{
    vector<lost> allRecord = Util::getStorageSync<lost>("record");
    vector<lost> lastYear;
    for(int i=0,length=allRecord.size();i<length;i++){
        if(allRecord[i].isLastYearLost()){
            lastYear.push_back(allRecord[i]);
        }
    }
    Util::generater(lastYear,"c");
    QMessageBox::about(NULL, "导出成功", "一年以上的遗失物品记录已成功导出至years_up.txt文件！");
    system("years_up.txt");
}

