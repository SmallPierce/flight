#include "flight_base.hpp"

using namespace std;
using namespace calendar_base;
using namespace flight_base;

// string split
vector<string> split(string str, string pattern)
{
    string::size_type pos;
    vector<string> result;

    str += pattern;//扩展字符串以方便操作
    int size = str.size();

    for (int i = 0; i<size; i++) {
        pos = str.find(pattern, i);
        if (pos<size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

bool air_line::find(WeekEnd weekday)
{
    vector<WeekEnd>::iterator it = f_execute_date.begin();
    while(it!= f_execute_date.end())
    {
        if((*it)== weekday) return true;
        else it++;
    }
    if(it == f_execute_date.end()) return false;
    return false; 
}

void month_arrange_table::init_month_arrange()
{
    for(int i = 0 ; i < m_month_calendar.size();i++)
    {
        DATE day1 = m_month_calendar.match_date(i);
        air_line_set set;
        match_pair temp = make_pair(day1,set );
        m_month_arrage_map.push_back(temp);
    }
}


void month_arrange_table::create_month_arrange(vector<air_line> month_arrage_vec)
{
    for(auto it : month_arrage_vec) // AIR_LINE
    {
        for(int i = 0 ; i < m_month_calendar.size(); i++)
        {
        }
    }
}

void month_arrange_table::month_plan_print()
{
    auto it = m_month_arrage_map.begin();
    while(it != m_month_arrage_map.end())
    {
        cout << it->first.t_date << ":" ;
        for(auto flight : it->second)
        {
            cout << flight.flight_no << " ";
        }
        cout << endl;
    }
}

/** 
 * @brief:read the air plan from the plan csv
 * @return: if succss return 1 
 *          else return -1;
 **/
bool excel_handler::read_plan_csv()
{
    fstream file("../data/flight_plan.csv", ios::in);
    if(!file)
    {
        cout << "open plan.csv failed" << endl;
        return false;
    }

    string record_line;
    while(getline(file, record_line))
    {
        air_line line_temp;
        vector<string>  temp = split(record_line, ",");
        line_temp.flight_no = atoi(temp[0].c_str());
        line_temp.flight_type = atoi(temp[2].c_str());
        if(line_temp.flight_type == 1) line_temp.flight_weight = 1.0;
        else line_temp.flight_weight = 0.0;
        if(temp.size() > 3)
        {
            line_temp.flag = atoi(temp[4].c_str());
        }
        string::iterator it = temp[1].begin();
        while(it!= temp[1].end())
        {
            int weekday = int(*it - '0');
            WeekEnd week_temp = (WeekEnd)weekday;
            line_temp.f_execute_date.push_back(week_temp);
            it++;
        }
        this->m_month_arrage_vec.push_back(line_temp);
    }
    return true;
}

void excel_handler::print_plan(){
    for(int i = 0; i < m_month_arrage_vec.size(); i++)
    {
        cout << m_month_arrage_vec[i].flight_no << ":" << m_month_arrage_vec[i].flight_type << endl;
    }
}
/**
 * @brief:read the arrage file which has already set the flight plan
 **/

bool excel_handler::read_arrage_csv()
{
    fstream file("../data/arrage.csv", ios::in);
    if(!file)
    {
        cout << "open the arrage file error " << endl;
        return false; 
    }

    string record_line;
    while(getline(file, record_line))
    {
        group_need_flight vec;
        vector<string> temp_vec = split(record_line, ",");
        for(int j = 0 ; j < temp_vec.size(); j = j+2)
        {
            air_line flight_temp;
            DATE day = m_month_calendar.match_date(atoi(temp_vec[j].c_str()));            
            try{
                flight_temp = get_air_line(atoi(temp_vec[j+1].c_str()));
            }
            catch(runtime_error)
            {
                cout << "flight [" << temp_vec[j+1]  
                     << "] not match in plan file"<< endl;
                cout << "please comfirm it" << endl;
                flight_temp.flight_no = atoi(temp_vec[j+1].c_str());
                flight_temp.flag = 0;
                flight_temp.flight_type = 1;
                flight_temp.flight_weight = 1.0;
            }
            vec.push_back(pair<DATE,air_line>(day,flight_temp));
        }
        m_group_arrage.push_back(vec);
    }
    return true;
}

air_line excel_handler::get_air_line(int flight_no)
{
    for(auto it : m_month_arrage_vec)
    {
        if(it.flight_no == flight_no) 
            return it;
    }
    throw runtime_error("not find this flight no in month plan");
}
