#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <ctime>
#include<fstream>
#include<cstring>
#include<vector>
#include<iostream>

using namespace std;



using namespace std;

// 用于获取数组的长度
template<class T> //任意类型
int getLength(T &arr);

// 存储工具
class storageComponent{
public:
    static string PATH;

    // 读取数据
    static char *getStorageSync(char *fname)
    {
        FILE *fp;
        char *str;
        char txt[1000];
        int filesize;
        if ((fp=fopen(fname,"r"))==NULL){
            printf("打开文件%s错误\n",fname);
            return "";
        }

        fseek(fp,0,SEEK_END);

        filesize = ftell(fp);
        str=(char *)malloc(filesize);
        str[0]=0;

        rewind(fp);
        while((fgets(txt,1000,fp))!=NULL){
            strcat(str,txt);
        }
        fclose(fp);
        return str;
    }

    // 写入数据
    static bool setStorageSync(const char filename[], const char a[]){
        int i = 0;
        FILE *fp = NULL;
        if ((fp=fopen(filename,"a"))==NULL){
            printf("打开文件%s错误\n",filename);
            return false;
        }
        for (i = 0; i < strlen(a); i++)
        {
            fputc(a[i], fp);
        }
        fclose(fp);
        return true;
    }
};

// 遗失物品对象
class lost{
private:
  char lostName[30],place[30]; // 遗失物名称、遗失地点
  tm lostTime,pickUpTime; // 丢失时间、领取时间
  time_t lostTimeStamp,pickUpTimeStamp; // 丢失时间戳、领取时间戳
  int state; // 物品状态{1:丢失,2:管理员已确认,3:已取回}
  char stuName[30],phoneNumber[30],stuId[30]; // 领取学生姓名、电话、学号
  int type; // 物品类型{1:电子设备,2:书,3:其他}
public:
    lost(const char *name0,const char *place0,tm &t);
    lost(string &name0,string &place0,tm *t);
    lost();
    void verify();
    void pickUp(string stuName0,string phoneNumber0,string stuId0,tm pt);
    string state2String();
    string type2String();
    string getLostDay();
    string getName();
    string getPickUpTime();
    string getPlace();
    int getState();
    string getStuName();
    string getPhoneNumber();
    string getStuId();
    tm getLostTime();
    tm getFoundTime();
    time_t getLostTimeStamp();
    time_t getFoundTimeStamp();
    static bool lostTimeComp( lost &a,  lost &b);
    static bool pickUpTimeComp( lost &a, lost &b);
    bool isLastYearLost();
    bool isLastWeekLost();
};

// 数据处理工具
class Util{
public:
    static string DAT_PATH;
    static string recordPath(string &filename);
    static bool generater(vector<lost> &r);
    static bool generater(vector<lost> &r,string filename);
    // 存储任意类型对象的Vector数组
    template <class T>
    static vector<T> getStorageSync(const char filename[]){
        vector<T> dest;
        ifstream inF(DAT_PATH + string(filename) + ".dat", ios::in | ios::binary);
        if (inF.is_open())
        {
          T temp;
          while (inF.read((char *)&temp, sizeof(T)))
          {
            dest.push_back(temp);
          }
          inF.close();
          return dest;
        }
    }
    // 读取任意类型对象的Vector数组
    template <class T>
    static bool setStorageSync(const char filename[], vector<T> &t){
        try{
            ofstream outF(DAT_PATH + string(filename) + ".dat", ios::out | ios::binary);
            if(outF.is_open()){
                T *ta = new T[t.size()];
                memcpy(ta, &t[0], t.size() * sizeof(t[0]));
                outF.write((char *)ta, sizeof(ta[0])*t.size());
                outF.close();
                delete ta;
                return true;
            }
        }
        catch(exception e){
            cout<<e.what()<<endl;
            return false;
        }
    }
};

#endif // COMPONENT_H
