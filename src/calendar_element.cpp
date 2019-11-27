#include "calendar_element.hpp"

using namespace std;
using namespace calendar_base;
void month_calendar::update_calendar()
{
    time_t time_now = time(0);
    cout << "1970 到现在的时间是：" << time_now << endl;
    tm *ltm = localtime(&time_now);

    // update the next 
    m_year = 1900 + ltm->tm_year;
    m_month = 2+ ltm->tm_mon;
    
    cout << m_year << m_month << endl;
}

int nyear(int year)
{
    if(year%400==0||(year%4==0&&year%100!=0))
	   return 366;
    else
	   return 365;
}

// according the year and month, calc the month days 
int nmonth(int m,int y) 
{
    if (m==1 || m==3 || m==5 || m==7 || m==8 || m==10 || m==12)
	   return 31;
    else if (nyear(y)==366&&m==2)
	   return 29;
    else if (nyear(y)==365&&m==2)
	   return 28;
    else
	   return 30;
}

// according the year and month ,calc the total days begin from 1900
int getDays(int year, int month)
{
    int i;
    int sum=0;
    if (year>1900)
        for(i=1900; i<year; i++)
    	    sum+=nyear(i);
    if (month>1)
        for(i=1;i<month;i++)
    	    sum+=nmonth(i,year);
 
    return sum;
}

void month_calendar::create_calendar(){
    int sum = getDays(m_year, m_month);
    int day_count = nmonth(m_year, m_month);
    for(int i = 1 ; i <= day_count; i++)
    {
        DATE temp;
        temp.t_date = i;
        temp.t_month = m_month;
        temp.t_year = m_year;
        int week_day = (sum + i)%7 +1;
        temp.t_week = (WeekEnd) week_day;
        m_month_calendar.push_back(temp);
   }
}

void month_calendar::print(){
    for(int i = 0; i < m_month_calendar.size(); i++)
    {
        DATE day = m_month_calendar[i];
        cout << day.t_date << " : " << day.t_week << endl; 
    }
    return;
}

