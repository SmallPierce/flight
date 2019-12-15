//
// Created by jie wang on 2019-12-13.
//

//
// Created by smallwang911 on 12/12/19.
//
#include <iostream>
#include "flightbase.hpp"
#include <map>
#include <sstream>
#include <iomanip>
using namespace std;

// 字符串分割函数
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

// 计算年
int nyear(const int year)
{
    if ((year%400 == 0) || (year%4 == 0 && year%100!=0))
        return 366;
    else return 365;
}

int nmonth(int m, int y)
{
    if (m==1 || m==3 || m==7 || m==8 || m==10 || m==12)
        return 31;
    else if (nyear(y)==366 && m==2)
        return 29;
    else if (nyear(y)==365 && m==2)
        return 28;
    else return 30;
}

int getdays(int year , int month)
{
    int i;
    int sum = 0;
    if (year > 1900)
        for(i=1900; i < year; i++)
        {
            sum += nyear(i);
        }
    if(month>1)
        for(i=1;i<month;i++)
        {
            sum += nmonth(i,year);
        }
    return sum +1;
}
// 生成年月日
void Month_Calendar::calendar_init() {
    int sum = getdays(this->m_year, this->m_month);
    int day_count = nmonth(this->m_month,this->m_year);
    this->m_date_size = day_count;
    for(int i = 1; i <= day_count; i++)
    {
        auto weeks = (WeekEND)((sum+i)%7);
        DATE day_temp(i, weeks);
        this->m_calendar.push_back(day_temp);
    }
}

int Month_Calendar::get_date(const DATE &day) {
    for(auto it : this->m_calendar)
    {
        if(day.m_day == it.m_day)
            return it.m_day;
    }
    return 0;
}

WeekEND Month_Calendar::get_week(const DATE &day) {
    for(auto it : this->m_calendar)
    {
        if(day.m_day == it.m_day)
            return it.m_week;
    }
}

void Month_Calendar::cprint() const {
    cout << setiosflags(ios::right) << setw(5) <<
         "Sun" << setw(5) << "Mon" <<
         setw(5) <<"Tue" <<
         setw(5) <<"Wen" <<
         setw(5) <<"Tur" <<
         setw(5) <<"Fri" <<
         setw(5) <<"Sat" << endl;
    int space = (int)this->m_calendar[0].m_week;
    for(int i = 0 ; i < space ; i++)
    {
        cout << setw(5) << "" ;
    }
    for(int i = 0 ; i < this->m_date_size ; i++ )
    {
        if(this->m_calendar[i].m_week != Saturday)
        {
            cout << setw(5) << this->m_calendar[i].m_day;
        }
        else
            cout << setw(5) << this->m_calendar[i].m_day <<endl;
    }
}
Flight_Info::Flight_Info(int no, float wei, int duty[7]) {
    this->m_flight_no = no;
    this->m_flight_weight = wei;
    for(int i =0 ; i < 7 ; i++)
        this->m_on_duty[i] = duty[i];
}
// 生成月度飞行表
void Flight_Schedule::init(const Month_Calendar & calendar) {
    this->m_calendar = calendar;
}
Flight_Info Flight_Schedule::get_by_no(int flight_no) {
    for(auto itr : m_flight_schedule)
    {
        if(itr.m_flight_no == flight_no)
            return itr;
    }
}

void Flight_Schedule::create_schedule(const char *path) {
    fstream read_file(path, ios::in);
    if(!read_file)
        cout << "open csv file err" << endl;

    string record_line;
    while(getline(read_file, record_line))
    {

        vector<string> f_vec ;
        int temp[7] = {0};
        SplitString(record_line, f_vec, ",");
        const char * s = f_vec[0].c_str();
        int flight_no = atoi(s);
        float wight = stof(f_vec[2].c_str());
        if(!f_vec[1].empty())
        {
            for(auto it : f_vec[1])
            {
                int index = (it - '0')%7;
                temp[index] = 1;
            }
        }
        Flight_Info f_temp(flight_no, wight, temp);
        this->m_flight_schedule.push_back(f_temp);
    }
    cout << "read csv file finished"  << endl;
}
void Flight_Schedule::cprint() {
    for(auto it : m_flight_schedule)
    {
        cout << it.m_flight_no  << ":" << it.m_flight_weight << ":";
        for(auto itr : it.m_on_duty)
        {
            cout << itr;
        }
        cout << endl;
    }
}
void Month_Arrange::Month_init(const Month_Calendar &calendar) {
    this->m_calendar = calendar;
    this->m_size = calendar.size();
    for (int i = 0; i < this->m_size; i++) {
        Flight_Base base_Temp;
        std::get<0>(base_Temp) = i + 1;
        std::get<1>(base_Temp) = this->m_calendar.get_Date(i);
        this->m_arragement.push_back(base_Temp);
    }
}

void Month_Arrange::cprint() {
    for(int i = 0 ; i < this->m_arragement.size(); i++)
    {
        cout << "[" << std::get<0>(m_arragement[i]) << "]" <<  std::get<1>(m_arragement[i]).m_day
             << ":"<<std::get<1>(m_arragement[i]).m_week << endl;
    }
}

void Month_Arrange::Create_Arragement(Flight_Schedule &schedule) {
    vector<Flight_Info> scheduel_vec = schedule.get_schedule();
    for(auto sch : scheduel_vec)
    {
        for(int i = 0 ;i < this->m_size; i++) {
            Flight_Base &temp = this->m_arragement[i];
            int index = (int) std::get<1>(temp).m_week;// 0 周日
            if (sch.m_on_duty[index] == 1) // 1234567
            {
                get<2>(temp).push_back(sch);
            }
        }
    }
}

bool Month_Arrange::check(int flight_no, int day) {
    Flight_Base &temp = this->m_arragement[day];
    vector<Flight_Info> vec = get<2>(temp);
    for(auto itr : vec)
    {
        if(itr.m_flight_no == flight_no) {
            return true;
        }
    }
    return false;
}

void Month_Arrange::printarrange() {
    for(int i = 0 ; i < this->m_size ; i++)
    {
        cout << "DAY" << ":" << std::get<0>(this->m_arragement[i]) << " ";
        cout << "WEEK" << ":" << std::get<1>(this->m_arragement[i]).m_week  << " ";
        vector<Flight_Info> temp = get<2>(this->m_arragement[i]);
        for(auto itr : get<2>(this->m_arragement[i]))
        {
            cout << itr.m_flight_no << " " ;
        }
        cout << endl;
    }
}

void Group_Arrage::init(Month_Calendar & calendar) {
    this->m_flight_weight = 0.0;
    this->m_month_weight = 0.0;
    this->m_size = calendar.size();
    this->m_calendar = calendar;
    for (int i = 0; i < this->m_size; i++)
    {
        Group_Base base_temp;
        get<0>(base_temp) = i + 1;
        get<1>(base_temp) = this->m_calendar.get_Date(i);
        get<2>(base_temp) = 0.0;
        this->m_group_arrage.push_back(base_temp);
    }
}

void Group_Arrage::insert(int day, Flight_Info flight_info) {
    this->m_flight_weight += flight_info.m_flight_weight;
    this->m_month_weight += flight_info.m_flight_weight;
    float & day_map = get<2>(this->m_group_arrage[day]);
    day_map += flight_info.m_flight_weight;
    vector<Flight_Info> &vec = get<3>(this->m_group_arrage[day]);
    vec.push_back(flight_info);
}

void Month_Arrange::init_force(const char *path) {
    fstream read_file(path, ios::in);
    if(!read_file)
        cout << "没有强制飞行文件" << endl;
    string record;
    while(getline(read_file, record))
    {
        vector<string> vec_temp ;
        SplitString(record,vec_temp, ",");
        Force_Arrage temp;
        get<0>(temp) = stoi(vec_temp[0]);
        get<1>(temp) = stoi(vec_temp[1]);
        get<2>(temp) = stoi(vec_temp[2]);

        m_force_arrange.push_back(temp);
    }
}