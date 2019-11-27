#ifndef _FLIGHT_BASE_HPP_
#define _FLIGHT_BASE_HPP_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "calendar_element.hpp"

using namespace std;
using namespace calendar_base;

#define GROUP_NO 6

namespace flight_base{

    typedef struct air_line{
        bool find(WeekEnd weekday);
        int flight_no;
        float flight_weight;
        vector<WeekEnd> f_execute_date;
        int flight_type;
        int flag;
    }air_line;

struct cmp_key{
  bool operator()(const DATE & day1, const DATE &day2)const
  {
      if(day1.t_date != day2.t_date)
        return day1.t_date < day2.t_date;
      else
      {
        return day1.t_date > day1.t_date;
      }
      
  }
};
    typedef vector<air_line> air_line_set;
    typedef map<DATE, air_line> air_line_fix;
    typedef vector<pair<DATE, air_line> > group_need_flight;
    typedef pair<DATE, air_line_set> match_pair;

    class month_arrange_table{
        public:
            month_arrange_table(){}
            ~month_arrange_table(){}

            void init(month_calendar var){
                this->m_month_calendar = var;
            }
            void create_month_arrange(vector<air_line> month_arrage_vec);
            void init_month_arrange();
            void month_plan_print();
        private:
            month_calendar m_month_calendar;
            vector<match_pair> m_month_arrage_map;

    };

    class group_month_arrage{
        public:
            group_month_arrage(){}
            ~group_month_arrage(){}

            void init(month_calendar var){
                this->m_month_calendar = var;
            }
            void create_group_arranage();
        private:
            month_calendar m_month_calendar;
            map<DATE, vector<air_line>, cmp_key > group_arrange_vec;
    };

/**
 * @brief: response create the month air_line plan table 
 *         response create the month must have to air_line arrange
 **/
    class excel_handler{
        public:
            excel_handler(){}
            ~excel_handler(){}
            void init(month_calendar var)
            {
                this->m_month_calendar = var;
            }

            air_line get_air_line(int flight_no);
            bool read_plan_csv();
            bool read_arrage_csv();
            void print_plan();
            vector<air_line> get_month_arrange(){
                return this->m_month_arrage_vec;
            }
        private:
            vector<air_line> m_month_arrage_vec;
            vector<group_need_flight> m_group_arrage;           
            month_calendar m_month_calendar;
    };
}




#endif