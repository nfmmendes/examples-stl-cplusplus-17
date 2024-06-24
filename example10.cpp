#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
using namespace std::chrono_literals;
using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::this_thread::sleep_for;

int main()
{
    const string phrase[] = {"We", "wish", "you", "a", "merry", "christmas"};

    auto autoFunction = [=](int threadId)
    {
        sleep_for(10ms * (rand() % 10));
        cout << phrase[threadId] << " ";
        sleep_for(500ms);
        cout << "\nEnd of thread " << threadId;
    };

    thread t1{autoFunction, 0};
    thread t2{autoFunction, 1};
    thread t3{autoFunction, 2};
    thread t4{autoFunction, 3};
    thread t5{autoFunction, 4};
    thread t6{autoFunction, 5};

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    cout << "\n";

    return 0;
}