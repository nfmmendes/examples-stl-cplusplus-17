#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <execution>
#include <thread>
#include <set>
using std::back_insert_iterator;
using std::copy;
using std::cout;
using std::endl;
using std::set;
using std::string;
using std::thread;
using std::to_string;
using std::vector;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

class Foo
{

public:
    Foo() {}

    Foo(const Foo &other) : code(other.code)
    {
    }

    Foo(const Foo &&other) : code(other.code)
    {
    }

    Foo &operator=(const Foo &other)
    {
        code = other.code;

        return *this;
    }

    Foo &operator=(Foo &&other)
    {
        code = other.code;
        other.code = "";

        return *this;
    }

    void setCode(const string &value)
    {
        code = value;
    }

private:
    string code{};
};

int main()
{
    const int NUMBER_OF_ELEMENTS = 1024 * 1024 * 64;
    const int NUMBER_OF_THREADS = 4;
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
    vector<thread> threads;
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        threads.push_back(thread([i, &fooVector, &secondCopy]()
                                 {
            auto first = begin(fooVector) + i * fooVector.size() / NUMBER_OF_THREADS;
            auto last = first + fooVector.size() / NUMBER_OF_THREADS;
            move(first, last, begin(secondCopy)); }));
    }

    for (auto &t : threads)
        t.join();

    stop = high_resolution_clock::now();

    cout << "Elapsed time: " << duration_cast<microseconds>(stop - start).count() << " microseconds" << endl;

    firstCopy.clear();

    return 0;
}