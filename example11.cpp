#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <string>
#include <chrono>
using std::cout;
using std::endl;
using std::shared_mutex;
using std::string;
using std::thread;
using std::unique_lock;
using std::this_thread::sleep_for;
using namespace std::chrono_literals;

shared_mutex mutex{};
int countAccesses = 0;

void newAccess(const string &userName)
{
    std::unique_lock lock{mutex};
    sleep_for(200ms);
    countAccesses++;
    string ordinalSuffix = countAccesses == 1 ? "st" : (countAccesses == 2 ? "nd" : countAccesses == 3 ? "rd"
                                                                                                       : "th");

    cout << "Hello " << userName << "! You are the " << countAccesses << ordinalSuffix << " visitor" << endl;
}

int main()
{
    thread t1{newAccess, "Marco"};
    thread t2{newAccess, "Paolo"};
    thread t3{newAccess, "Roberto"};
    thread t4{newAccess, "Anna"};
    thread t5{newAccess, "Maria"};
    thread t6{newAccess, "Martina"};

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    return 0;
}