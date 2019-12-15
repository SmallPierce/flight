//
// Created by smallwang911 on 12/12/19.
//

#ifndef FLIGHT_FLIGHTBASE_HPP
#define FLIGHT_FLIGHTBASE_HPP
#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <fstream>
#define GROUP_SIZE 6

using namespace std;

typedef enum WeekEND{
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
}WeekEND;  // WeekEND

class DATE{
public:
    DATE(){};
    DATE(int day, WeekEND weeks){
        this->m_day = day;
        this->m_week = weeks;
    }
    ~DATE()= default;
    void init(const int &day, const WeekEND &week);
public:
    int m_day;
    WeekEND m_week;
};  // DATE

class Month_Calendar{
public:
    Month_Calendar()= default;
    Month_Calendar(int year, int month){
        this->m_year = year;
        this->m_month = month;
    }
    ~Month_Calendar()= default;
    void calendar_init();
    int size()const {return m_date_size;}
    int get_date(const DATE &day);
    DATE get_Date(int i){return m_calendar[i];}
    WeekEND get_week(const DATE &day);
    void cprint() const;
private:
    int m_month;
    int m_year;
    int m_date_size;
    vector<DATE> m_calendar;
};

class Flight_Info{
public:
    Flight_Info(){}
    Flight_Info(int no, float wei, int duty[]);
    int m_flight_no;
    float m_flight_weight;
    int m_on_duty[7]{};
};

class Flight_Schedule{
public:
    Flight_Schedule()= default;
    ~Flight_Schedule()= default;
    void init(const Month_Calendar & calendar);
    void create_schedule(const char * path);
    void cprint();
    Month_Calendar calendar()const{ return this->m_calendar;}
    vector<Flight_Info> get_schedule(){return this->m_flight_schedule;}
    Flight_Info get_by_no(int flight_no);
private:
    vector<Flight_Info> m_flight_schedule;
    Month_Calendar m_calendar;
};

typedef tuple<int , DATE, vector<Flight_Info>> Flight_Base;
typedef tuple<int , int, int> Force_Arrage;
class Month_Arrange {
public:
    Month_Arrange(){}
    ~Month_Arrange(){}
    void Month_init(const Month_Calendar &calendar);
    void Create_Arragement(Flight_Schedule &schedule);
    Month_Calendar calendar()const {return this->m_calendar;}
    void cprint();
    bool check(int flight_no, int day);
    void printarrange();
    void init_force(const char * path);
    vector<Force_Arrage> get_force(){return m_force_arrange;}
public:
    Month_Calendar m_calendar;
    int m_size;
    vector<Flight_Base> m_arragement;
    vector<Force_Arrage> m_force_arrange;
};

typedef tuple<int, DATE, float, vector<Flight_Info>> Group_Base;

class Group_Arrage{
public:
    Group_Arrage(){}
    ~Group_Arrage(){}
    void init(Month_Calendar & calendar);
    void insert(int day, Flight_Info flight_info);
    int get_size(){return m_size;}
public:
    float m_month_weight;
    float m_flight_weight;
    int m_size;
    vector<Group_Base> m_group_arrage;
    Month_Calendar m_calendar;
};

#endif //FLIGHT_FLIGHTBASE_HPP
