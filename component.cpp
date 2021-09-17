#include "component.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// lost对象
// 丢失物品构造函数，传入丢失物品名称，丢失地点，时间
lost::lost(const char *name0,const char *place0, tm &t) : lostTime(t),state(1),lostTimeStamp(mktime(&t)){
    strcpy(lostName, name0);
    strcpy(place, place0);
}
lost::lost(string &name0,string &place0,tm *t) : lostTime(*t),state(1),lostTimeStamp(mktime(t)){
    strcpy(lostName,name0.c_str());
    strcpy(place,place0.c_str());
}
lost::lost():state(1){}
// 确认函数，管理员确认物品
void lost::verify(){
    this->state=2;
};
// 遗失物品取回函数
void lost::pickUp(string stuName0,string phoneNumber0,string stuId0,tm pt){
    strcpy(this->stuName,stuName0.c_str());
    strcpy(this->phoneNumber,phoneNumber0.c_str());
    strcpy(this->stuId,stuId0.c_str());
    this->pickUpTimeStamp=mktime(&pt);
    this->pickUpTime=pt;
    this->state=3;
};
// 获取物品状态信息
string lost::state2String(){
    string stateString;
    switch(state){
        case 1:
            stateString = "等待管理员确认";
            break;
        case 2:
            stateString = "管理员已确认";
            break;
        case 3:
            stateString = "已被认领";
        break;
        default:
            stateString = "";
    }
    return stateString;
}
// 获取丢失日期
string lost::getLostDay(){
    // 涉及一些tm库时间对象操作
    string year = to_string(this->lostTime.tm_year + 1900);
    string month = to_string(lostTime.tm_mon + 1);
    string day = to_string(lostTime.tm_mday);
    string hour = to_string(lostTime.tm_hour);
    string minute = to_string(lostTime.tm_min);
    string second = to_string(lostTime.tm_sec);
    return year + "年" + month + "月" + day + "日" + " " + (hour.length()<2?"0"+hour:hour) + ":" + (minute.length()<2?"0"+minute:minute) + ":" + (second.length()<2?"0"+second:second);
}
// 获取物品名
string lost::getName(){
    return string(this->lostName);
}
// 获取丢失地点
string lost::getPlace(){
    return string(this->place);
}
// 获取物品状态
int lost::getState(){
    return this->state;
}
// 获取领取时间
string lost::getPickUpTime(){
    // 涉及一些tm库时间对象操作
    string year = to_string(this->pickUpTime.tm_year + 1900);
    string month = to_string(pickUpTime.tm_mon + 1);
    string day = to_string(pickUpTime.tm_mday);
    string hour = to_string(pickUpTime.tm_hour);
    string minute = to_string(pickUpTime.tm_min);
    string second = to_string(pickUpTime.tm_sec);
    if(this->state==3) return year + "年" + month + "月" + day + "日" + " " + (hour.length()<2?"0"+hour:hour) + ":" + (minute.length()<2?"0"+minute:minute) + ":" + (second.length()<2?"0"+second:second);
    else return "";
}
// 获取领取学生姓名
string lost::getStuName(){
    return string(this->stuName);
}
// 获取领取学生手机号
string lost::getPhoneNumber(){
    return string(this->phoneNumber);
}
// 获取领取学生学号
string lost::getStuId(){
    return string(this->stuId);
}
// 获取物品类型
string lost::type2String(){
    string typeString;
    switch(state){
        case 1:
            typeString = "电子设备";
            break;
        case 2:
            typeString = "书";
            break;
        default:
            typeString = "其他";
    }
    return typeString;
}
// 获取丢失时间
tm lost::getLostTime(){
    return this->lostTime;
}
// 获取取回时间
tm lost::getFoundTime(){
    return this->pickUpTime;
}
// 获取丢失时间戳
time_t lost::getLostTimeStamp(){
    return this->lostTimeStamp;
}
// 获取取回时间
time_t lost::getFoundTimeStamp(){
    return this->pickUpTimeStamp;
}
// 丢失时间比较函数
bool lost::lostTimeComp(lost &a,lost &b){
    return difftime(a.getLostTimeStamp(), b.getLostTimeStamp()) > 0;
}
// 取回时间比较函数
bool lost::pickUpTimeComp( lost &a,lost &b){
    return difftime(a.getLostTimeStamp(), b.getLostTimeStamp()) > 0;
}
bool lost::isLastWeekLost(){
    int ONE_DAY = 86400;
    time_t t=time(0);
    long diff = t-this->getLostTimeStamp();
    return diff < (ONE_DAY*7);
}

bool lost::isLastYearLost(){
    int ONE_YEAR = 31536000;
    time_t t=time(0);
    long diff = t - this->getLostTimeStamp();
    return diff > ONE_YEAR;
}



// 数据处理工具
// 定义文件存储根目录
string Util::DAT_PATH= "./";
// 合成存储路径
string Util::recordPath(string &filename){
    return "./" + filename + ".txt";
}
// 生成导出文字数据
bool Util::generater(vector<lost> &r,string filename){
    string result;
    for(int i=0,length=r.size();i<length;i++){
        result+=("物品名称:" + string(r[i].getName()) + " 位置:" + string(r[i].getPlace())+" 丢失时间:"+r[i].getLostDay()+" " +r[i].state2String()+"\n");
    }
    ofstream outF(recordPath(filename), ios::out);
    if(outF.is_open()){
        outF<<result<<endl;
        outF.close();
        return true;
    }
    else{
        return false;
    }
}




