#include <iostream>
using std::cout;
using std::endl;

struct Date
{
    unsigned int day;
    unsigned int month;
    int year;
};

int main()
{
    Date laborDay{1, 5, 2024};

    auto [day, month, year] = laborDay;

    cout << day << " " << month << " " << year << endl;

    return 0;
}