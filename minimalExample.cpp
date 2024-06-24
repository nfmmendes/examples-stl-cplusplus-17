#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;

int main()
{
    vector<int> elements{3, 5, 2, 1, 4};

    for (auto item : elements)
        cout << item << " ";

    cout << endl;

    return 0;
}