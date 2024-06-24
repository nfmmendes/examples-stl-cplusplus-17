#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main()
{
    // C++17 new feature: Generic lambdas with auto parameters.
    auto printValue = [](auto &value)
    {
        cout << value << " ";
    };

    vector<int> intVector{3, 5, 4, 2, 1, 3};
    vector<string> stringVector{"axf", "dfd", "pwo", "dkc", "wwp"};

    for (auto v : intVector)
        printValue(v);
    cout << endl;

    for (auto &v : stringVector)
        printValue(v);
    cout << endl;

    return 0;
}