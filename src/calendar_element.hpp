#ifndef _FLIGHT_ELEMENT_HPP_
#define _FLIGHT_ELEMENT_HPP_

#include <iostream>
#include <vector>

using namespace std;

namespace calendar_base{

    typedef enum WeekEnd{
        MONDAY = 1,
        TUESDAY,
        WENDAY,
        THURSDAY,
        FRIDAY,
        SATUREDAY,
        SUNDAY
    }WeekEnd;

    typedef struct DATE{
        int t_year;
        int t_month;
        int t_date;
        WeekEnd t_week;        
    }DATE;

    class month_calendar{
        public:
            month_calendar(){}
            month_calendar(int year, int month)
            {
                this->m_year = year;
                this->m_month = month;
            }
            ~month_calendar(){}
            void update_calendar();
            void create_calendar();
            void print();
            int size(){return m_month_calendar.size();}
            DATE match_date(int i){return m_month_calendar[i];}
            vector<DATE> get_month_date_vec(){return m_month_calendar;}
        private:
            vector<DATE> m_month_calendar;
            int m_year;
            int m_month;
    };
}
#endif