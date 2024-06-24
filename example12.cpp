#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <map>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <cassert>
using std::cout;
using std::endl;
using std::find;
using std::map;
using std::mutex;
using std::quoted;
using std::shared_lock;
using std::shared_mutex;
using std::string;
using std::thread;
using std::unique_lock;
using std::vector;
using std::this_thread::sleep_for;
using namespace std::chrono_literals;

int main()
{
    const int NUMBER_OF_CYCLES = 50;

    map<string, bool> bookAvailabilityMap{{"Divina Commedia", true},
                                          {"Uno, nessuno e centomilla", true},
                                          {"Il nome della rosa", true},
                                          {"Il principe", true},
                                          {"Lo hobbit", true},
                                          {"La certosa di Parma", true},
                                          {"Il simbolo perduto", true},
                                          {"It", true},
                                          {"Romeo e Giulietta", true},
                                          {"Il piccolo principe", true},
                                          {"Design patterns", true},
                                          {"Il codice da Vinci", true},
                                          {"L'alchimista", true},
                                          {"Notre Dame di Paris", true}};

    vector<string> requests{
        "Divina Commedia",
        "Il simbolo perduto",
        "Il simbolo perduto",
        "Romeo e Giulietta",
        "L'alchimista",
        "Design patterns",
        "Il principe",
        "L'alchimista",
        "Il nome della rosa",
        "It",
        "Il piccolo principe",
        "Harry Potter e la camera dei segreti",
        "The woman in me",
        "Design patterns",
        "Il codice da Vinci",
        "La certosa di Parma",
        "Notre Dame di Paris",
        "Uno, nessuno e centomilla",
        "Il piccolo principe",
        "Il codice da Vinci",
        "Lo hobbit",
        "La certosa di Parma",
    };

    shared_mutex searchMutex;
    shared_mutex borrowMutex;
    shared_mutex returnMutex;

    auto handleRequests = [&bookAvailabilityMap, &searchMutex, &borrowMutex, &returnMutex](const string &book)
    {
        auto isBookAvailable = bookAvailabilityMap.find(book);
        if (isBookAvailable == end(bookAvailabilityMap))
        {
            cout << "The book " << std::quoted(book) << " is not present in this library" << endl;
            sleep_for(5ms);
            return;
        }

        { // Borrow block
            shared_lock check{searchMutex};
            if (isBookAvailable->second)
            {
                {
                    unique_lock bookLock{borrowMutex};
                    assert(isBookAvailable->second);
                    isBookAvailable->second = false;
                    cout << "The book: " << quoted(book) << " has been lent out" << endl;
                }
                sleep_for(300ms + 30ms * (rand() % 20));

                {
                    unique_lock return_book{returnMutex};
                    assert(!isBookAvailable->second);
                    isBookAvailable->second = true;
                    cout << "The book: " << quoted(book) << " has been returned" << endl;
                }
            }
        }
    };

    vector<thread> requestThreads;
    for (auto i = 0; i < NUMBER_OF_CYCLES * requests.size(); i++)
    {
        requestThreads.push_back(thread(handleRequests, requests[i % requests.size()]));
    }

    for (auto &item : requestThreads)
        item.join();

    return 0;
}