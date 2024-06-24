#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <thread>
#include <execution>
#include <set>
using std::back_insert_iterator;
using std::copy;
using std::cout;
using std::endl;
using std::set;
using std::string;
using std::to_string;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;
using std::execution::par;
using std::execution::par_unseq;
using std::this_thread::get_id;

class Foo
{

public:
    Foo() {}

    Foo(const Foo &other) : code(other.code)
    {
        threads.insert(get_id());
    }

    Foo(const Foo &&other) : code(other.code)
    {
        Foo::threads.insert(get_id());
    }

    Foo &operator=(const Foo &other)
    {
        threads.insert(get_id());
        code = other.code;

        return *this;
    }

    Foo &operator=(Foo &&other)
    {
        threads.insert(get_id());
        code = other.code;
        other.code = "";

        return *this;
    }

    void setCode(const string &value)
    {
        code = value;
    }

    static set<std::thread::id> threads;

private:
    string code{};
};

set<std::thread::id> Foo::threads{};

int main()
{
    const int NUMBER_OF_ELEMENTS = 1024 * 1024 * 64;
    vector<Foo> fooVector(NUMBER_OF_ELEMENTS);
    vector<Foo> firstCopy = fooVector;

    for (int i = 0; i < NUMBER_OF_ELEMENTS; i++)
        fooVector[i].setCode("foo_" + to_string(i));

    vector<Foo> secondCopy{NUMBER_OF_ELEMENTS};

    auto start = high_resolution_clock::now();
    cout << "Standard copy\n";
    move(begin(fooVector), end(fooVector), begin(secondCopy));
    auto stop = high_resolution_clock::now();

    cout << "Elapsed time: " << duration_cast<microseconds>(stop - start).count() << " microseconds" << endl;

    fooVector = firstCopy;

    start = high_resolution_clock::now();
    cout << "Parallel copy\n";
    move(par, begin(fooVector), end(fooVector), begin(secondCopy));
    stop = high_resolution_clock::now();

    cout << "Elapsed time: " << duration_cast<microseconds>(stop - start).count() << " microseconds" << endl;

    fooVector = firstCopy;

    start = high_resolution_clock::now();
    cout << "Parallel unsequential copy\n";
    move(par_unseq, begin(fooVector), end(fooVector), begin(secondCopy));
    stop = high_resolution_clock::now();

    cout << "Elapsed time: " << duration_cast<microseconds>(stop - start).count() << " microseconds" << endl;

    cout << Foo::threads.size() << endl;

    firstCopy.clear();

    return 0;
}