#include <iostream>
#include <algorithm>
#include <iomanip>
#include "flightbase.hpp"

void find_flight_Min(Group_Arrage arrage[],vector<int> &vec);
void find_day_min_flight(Group_Arrage arrage[], const int day, vector<int> & vec);
void find_month_min_flight(vector<int> &vec, Group_Arrage arrage[]);
void start_by_force(Group_Arrage arrage[], Month_Arrange & arrange, Flight_Schedule & schedule);
void create_the_arrage(Month_Calendar & calendar
        , Flight_Schedule & schedule
        , Month_Arrange & arrange);
void check(Flight_Info & flight, Group_Arrage arrange[]);
bool checkout(Flight_Info & flightInfo, Group_Arrage arrage[],int day);
int main() {
    // 生成日历
    Month_Calendar calendar(2019,12);
    calendar.calendar_init();
    calendar.cprint();

    Flight_Schedule schedule;
    schedule.init(calendar);
    schedule.create_schedule("../flight_plan");
    schedule.cprint();

    Month_Arrange arrange;
    arrange.Month_init(calendar);
    arrange.Create_Arragement(schedule);
    arrange.init_force("../initplan");
    arrange.printarrange();
    create_the_arrage(calendar, schedule, arrange);
    return 0;
}

void create_the_arrage(Month_Calendar & calendar
        , Flight_Schedule & schedule
        , Month_Arrange & arrange){
    Group_Arrage arrage[6];
    for(int i = 0; i < 6 ; i++)
    {
        arrage[i].init(calendar);
    }
    start_by_force(arrage,arrange, schedule);
    for(auto itr :schedule.get_schedule())
    {
        for(int i = 0 ; i < 6 ; i ++){
            arrage[i].m_flight_weight = 0.0;
        }
        check(itr, arrage);
        // 开始对每天插入该航班
        for(int i = 0 ; i < 31; i++)
        {
            if(!arrange.check(itr.m_flight_no, i)) continue;
            if(checkout(itr,arrage,i)) continue;
            vector<int> vec = {0,1,2,3,4,5};
            find_flight_Min(arrage, vec);
            find_day_min_flight(arrage, i, vec);
            find_month_min_flight(vec, arrage);

            if(vec.size() == 0)
                arrage[0].insert(i,itr);
            else
                arrage[vec[0]].insert(i, itr);
        }
        cout << setw(4)<< itr.m_flight_no << ":";
        for(int i = 0 ; i < 6 ; i++)
        {
            cout << setw(3)<< arrage[i].m_flight_weight << " ";
        }
        cout << endl;
    }
    for(int i = 0; i < 6 ; i++)
    {

        cout << "【GROUP : " << i+1 << "】" << endl;
        for(int j = 0 ; j < arrage[i].m_size ; j++) {
            cout << setw(6)<< "[Date" << j+1 << "] : ";
            vector<Flight_Info> vec = get<3>(arrage[i].m_group_arrage[j]);
            for (auto iter:vec)
                cout << iter.m_flight_no << " ";
            cout << get<2>(arrage[i].m_group_arrage[j]);
            cout << endl;
        }
    }
    for(int i = 0; i < 6; i++)
        cout << "[GROUP]" << i+1 << "    ";
    cout << endl;
    for(int i = 1; i <= 31 ; i++)
    {
        for (int j = 0; j <6; j++)
        {
            cout << setw(4)<< get<2>(arrage[j].m_group_arrage[i-1]) << "        ";
        }
        cout << endl;
    }
    for(int i =0 ;i <6 ; i++)
    {
        cout << arrage[i].m_month_weight << "          ";
    }
    cout << endl;
}

// 当天，找本月飞行时间最短的组 删除飞行时间比较大的组
void find_flight_Min(Group_Arrage arrage[], vector<int> &vec)  //找到最小的集合
{
    if(vec.size() ==1) return;
    if(vec.size() == 0)
    {
        for(int i=0;i<6 ;i++)
            vec.push_back(i);
    }
    float max_weight = 0.0;
    float min_weight = 10.0;

    // 找到最小的值
    for(int i = 0 ; i < vec.size() ; i++)
    {
        if (arrage[i].m_flight_weight > max_weight)
            max_weight = arrage[i].m_flight_weight;
        if (arrage[i].m_month_weight < min_weight)
            min_weight = arrage[i].m_month_weight;
    }
    vector<int>::iterator itr = vec.begin();
    while(itr!= vec.end())
    {   //  删除飞行时间比较长的组
        if(min_weight == max_weight) break;
        if (arrage[*itr].m_flight_weight == max_weight)
        {
            itr = vec.erase(itr);
        }
        else itr++;
    }
}

void check(Flight_Info & flight, Group_Arrage arrange[])
{
    for(int i = 0 ; i < 6; i++)
    {
        for(int j = 0; j < arrange[i].m_size; j++)
        {
            vector<Flight_Info> &temp = get<3>(arrange[i].m_group_arrage[j]);
            for(auto itr: temp)
            {
                if(flight.m_flight_no == itr.m_flight_no)
                    arrange[i].m_flight_weight+=flight.m_flight_weight;
            }
        }
    }
}

bool checkout(Flight_Info & flightInfo, Group_Arrage arrage[],int day)
{
    for(int i=0; i <6 ;i++)
    {
        vector<Flight_Info> & temp = get<3>(arrage[i].m_group_arrage[day]);
        for(auto itr: temp)
        {
            if(flightInfo.m_flight_no == itr.m_flight_no)
                return true;
        }
    }
    return false;
}

void start_by_force(Group_Arrage arrage[], Month_Arrange & arrange, Flight_Schedule & schedule){
    vector<Force_Arrage> temp = arrange.get_force();
    for(auto itr : temp)
    {
        int index = get<0>(itr);
        Flight_Info flight = schedule.get_by_no(get<2>(itr));
        arrage[index-1].insert(get<1>(itr)-1, flight);
    }
}
// 删除日飞行时间最小的组
void find_day_min_flight(Group_Arrage arrage[], const int day, vector<int> &vec)
{
    if (vec.size() == 1)
        return;
    else if(vec.size() == 0)
    {
        for(int i = 0 ;i <6; i++)
        {
            vec.push_back(i);
        }
    }

    float max_weight = 0.0;
    float min_weight = 10.0;

    // 找到天最小
    for(int i = 0 ; i < vec.size(); i++)
    {
        if(get<2>(arrage[vec[i]].m_group_arrage[day]) > max_weight)
            max_weight = get<2>(arrage[vec[i]].m_group_arrage[day]);
        if (get<2>(arrage[vec[i]].m_group_arrage[day]) < min_weight)
            min_weight = get<2>(arrage[vec[i]].m_group_arrage[day]);
    }
    // 删除比他大的
    vector<int>::iterator itr = vec.begin();
    while(itr!= vec.end())
    {   //  删除飞行时间比较长的组
        if( min_weight == max_weight ) break;
        if (get<2>(arrage[*itr].m_group_arrage[day]) == min_weight)
        {
            itr++;
        }
        else itr = vec.erase(itr);;
    }
}

// 如果该航班今天可以有N个组可执飞，我们查看今天的月飞行安排，安排月飞行压力最小的组去飞这个航班
void find_month_min_flight(vector<int> &vec, Group_Arrage arrage[])
{
    if(vec.size() == 1) return;
    if(vec.size() == 0) {
        for(int i = 0 ; i < 6; i++)
            vec[i]=i;
    }
    // 找到月最大的删除
    float max_weight = 0.0;
    float min_weight = 100.0;
    for(int i =0; i < vec.size(); i++)
    {
        if(arrage[vec[i]].m_month_weight > max_weight)
            max_weight = arrage[vec[i]].m_month_weight;
        if(arrage[vec[i]].m_month_weight < min_weight)
            min_weight = arrage[vec[i]].m_month_weight;
    }

    vector<int>::iterator itr = vec.begin();
    while(itr!= vec.end())
    {
        if(min_weight == max_weight) break;
        else if ( max_weight == arrage[*itr].m_month_weight)
        {
            itr = vec.erase(itr);
        }
        else itr++;
    }
}
