#include <iostream>
#include "calendar_element.hpp"
#include "flight_base.hpp"

using namespace std;
using namespace calendar_base;
using namespace flight_base;

// test the csv read and format 
void test(){
    month_calendar calendar(2019,11);
    calendar.update_calendar();
    calendar.create_calendar();
    calendar.print();

    excel_handler hander;
    hander.init(calendar);

    if(!hander.read_plan_csv())
    {
        cout << "read file filed" << endl;
    }

    hander.print_plan();

    month_arrange_table month_table;
    month_table.init(calendar);
    month_table.init_month_arrange();
    month_table.create_month_arrange(hander.get_month_arrange());
    month_table.month_plan_print();

}

int main(){
    // step 1 : create the calendar
    // month_calendar calendar(2019,11);
    // calendar.update_calendar();
    // calendar.create_calendar();
    // calendar.print();
    test();
    return 1;
}

