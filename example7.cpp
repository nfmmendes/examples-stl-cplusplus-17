#include <iostream>
#include <cmath>
using std::cout;
using std::endl;
using std::pow;

int main()
{
    auto quadraticFunction = [](double a, double b, double c)
    {
        return [=](double x)
        {
            return a * x * x + b * x + c;
        };
    };

    auto inversePower = [](double base)
    {
        return [=](double power)
        {
            return 1 / pow(base, power);
        };
    };

    cout << "Simple quadratic function \n";
    for (auto x = -10; x < 10; x++)
        cout << "For x = " << x << " y = " << quadraticFunction(2, 5, 3)(x) << endl;
    cout << endl;

    cout << "Simple pow two inverse function \n";
    for (auto x = -10; x < 10; x++)
        cout << "For x = " << x << " y = " << inversePower(2)(x) << endl;
    cout << endl;

    cout << "Compound function \n";
    for (auto x = -10; x < 10; x++)
        cout << "For x = " << x << " y = " << quadraticFunction(2, 5, 3)(inversePower(sqrt(2))(x)) << endl;
    cout << endl;

    return 0;
}