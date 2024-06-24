#include <iostream>
#include <map>
#include <string>
#include <utility>
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;

int main()
{
    map<string, int> countryCodes{{"Italy", 39}, {"France", 33}, {"Germany", 49}, {"Spain", 34}, {"United Kingdom", 44}};

    auto lastCountryPair = *countryCodes.rbegin();
    auto [lastCountryName, lastCountryCode] = lastCountryPair;

    cout << "The last country name is: " << lastCountryName << ", and its code is " << lastCountryCode << endl;

    cout << "Available countries with its respective codes: \n";

    for (auto &[country, code] : countryCodes)
        cout << "Country: " << country << " --- Code: " << code << endl;

    return 0;
}