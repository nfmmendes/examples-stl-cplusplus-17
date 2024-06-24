#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <numeric>
#include <optional>
#include <string>
#include <utility>
using std::back_inserter;
using std::copy;
using std::cout;
using std::enable_if;
using std::endl;
using std::find;
using std::forward;
using std::function;
using std::iota;
using std::is_arithmetic;
using std::is_same;
using std::make_pair;
using std::map;
using std::min;
using std::pair;
using std::result_of;
using std::string;
using std::vector;

template <class T>
class LinqContainer
{

public:
    LinqContainer() {}
    explicit LinqContainer(const T &input) : container(input) {}

    template <class U = T>
    LinqContainer<T> where(function<bool(typename U::value_type)> predicate) const
    {
        T result;

        copy_if(begin(), end(), back_inserter(result), predicate);

        return LinqContainer<T>{result};
    }

    LinqContainer<T> take(size_t count) const
    {
        T result;

        if (count > 0)
            copy(begin(), begin() + min(count, container.size()), back_inserter(result));

        return LinqContainer<T>{result};
    }

    LinqContainer<T> takeLast(size_t count) const
    {
        T result;

        if (count > 0)
            copy(end() - min(count, container.size()), end(), back_inserter(result));

        return LinqContainer<T>{result};
    }

    LinqContainer<T> order() const
    {
        T result;

        copy(begin(), end(), back_inserter(result));
        sort(std::begin(result), std::end(result));

        return LinqContainer<T>{result};
    }

    LinqContainer<T> orderDescending() const
    {
        T result;

        copy(begin(), end(), back_inserter(result));
        sort(std::begin(result), std::end(result));
        reverse(std::begin(result), std::end(result));

        return LinqContainer<T>{result};
    }

    static constexpr LinqContainer<vector<int>> range(int start, int count)
    {
        vector<int> result(count);
        if (count > 0)
            iota(std::begin(result), std::end(result), start);

        return LinqContainer<vector<int>>{result};
    }

    template <class U = T>
    typename std::enable_if<std::is_arithmetic<typename U::value_type>::value &&
                                !std::is_same<typename U::value_type, char>::value &&
                                !std::is_same<typename U::value_type, bool>::value,
                            int>::type
    sum()
    {
        return accumulate(begin(), end(), 0);
    }

    template <class U = T>
    typename std::enable_if<is_same<typename U::value_type, string>::value || is_same<typename U::value_type, char>::value,
                            string>::type
    concatenation()
    {
        string empty{};
        return accumulate(begin(), end(), empty);
    }

    template <class U = T>
    std::optional<typename U::value_type> firstOrDefault(function<bool(typename U::value_type)> &&predicate)
    {
        auto it = find_if(begin(), end(), predicate);

        if (it != end())
            return *it;
        else
            return {};
    }

    template <class U = T>
    typename U::value_type first(function<bool(typename U::value_type)> &&predicate)
    {
        auto result = firstOrDefault(move(predicate));

        if (!result.has_value())
            throw;

        return result.value();
    }

    template <class U = T>
    std::optional<typename U::value_type> lastOrDefault(function<bool(typename U::value_type)> &&predicate)
    {
        auto it = find_if(rbegin(), rend(), predicate);

        if (it != rend())
            return *it;
        else
            return {};
    }

    template <class U = T>
    typename U::value_type last(function<bool(typename U::value_type)> &&predicate)
    {
        auto result = lastOrDefault(move(predicate));

        if (!result.has_value())
            throw;

        return result.value();
    }

    template <typename F, class U = T>
    auto select(F &&selectionBuilder)
    {
        using ItemType = typename U::value_type;
        using ReturnType = typename result_of<F(ItemType &)>::type;
        vector<ReturnType> result;

        transform(begin(), end(), back_inserter(result), selectionBuilder);

        return LinqContainer<vector<ReturnType>>(result);
    }

    auto begin() const
    {
        return std::begin(container);
    }

    auto end() const
    {
        return std::end(container);
    }

    auto rbegin() const
    {
        return std::rbegin(container);
    }

    auto rend() const
    {
        return std::rend(container);
    }

private:
    T container{};
};

int main()
{
    LinqContainer<vector<int>> container{vector<int>{3, 9, 2, 4, 6, 19, 7, 4, 18, 11, 21}};

    // Get first two divisors of three.
    auto result = container.where([](int x)
                                  { return x % 3 == 0; })
                      .take(2);

    for (auto value : result)
        cout << value << " ";
    cout << endl;

    //  Get the last three divisors of two
    result = container.where([](int x)
                             { return x % 2 == 0; })
                 .takeLast(3);

    for (auto value : result)
        cout << value << " ";
    cout << endl;

    //  Sort divisors of two.
    result = container.where([](int x)
                             { return x % 2 == 0; })
                 .order();

    for (auto value : result)
        cout << value << " ";
    cout << endl;

    //  Sort descending the divisors of two.
    result = container.where([](int x)
                             { return x % 2 == 0; })
                 .orderDescending();

    for (auto value : result)
        cout << value << " ";
    cout << endl;

    // Show that the original container is unchanged.
    for (auto value : container)
        cout << value << " ";
    cout << endl;

    // Print range (Template parameter is ignored)
    for (auto value : LinqContainer<int>::range(-2, 15))
        cout << value << " ";
    cout << endl;

    LinqContainer<vector<string>> linqString{vector<string>{"abc", "cde", "efg"}};
    // Does not build, because the vector type is not numeric.
    // linqString.sum();
    cout << "String concatenation = " << linqString.concatenation() << endl;

    LinqContainer<vector<char>> linqVectorChar(vector<char>{'a', 'e', 'i', 'o', 'u'});
    // Does not build, because the vector type is not numeric.
    // linqVectorChar.sum();
    cout << "Char concatenation = " << linqVectorChar.concatenation() << endl; // builds

    cout << "Elements sum: " << container.sum() << endl;

    auto numericResult = container.firstOrDefault([](int x)
                                                  { return x % 2 == 0 && x % 3 == 0; })
                             .value_or(0);
    cout << "First common divisor of  2 and 3 is " << numericResult << endl;
    cout << "Again: " << container.first([](int x)
                                         { return x % 2 == 0 && x % 3 == 0; })
         << endl;

    numericResult = container.lastOrDefault([](int x)
                                            { return x % 2 == 0 && x % 3 == 0; })
                        .value_or(0);
    cout << "Last common divisor of  2 and 3 is " << numericResult << endl;
    cout << "Again: " << container.last([](int x)
                                        { return x % 2 == 0 && x % 3 == 0; })
         << endl;

    auto pairListResult = container.select([](int x)
                                           { return make_pair(x * 2, x * x); });

    cout << "Selection function results " << endl;
    for (auto [twice, square] : pairListResult)
        cout << "The double is: " << twice << " and the square is: " << square << endl;

    cout << "Selection function results with annonymous return type " << endl;
    auto annonymousResult = container.select([](int x)
                                             { struct { int twice; int square; string name; }obj{ 2*x,  x*x , "name_"+std::to_string(x)};
                        return  obj; });

    for (auto value : annonymousResult)
        cout << value.name << " " << value.twice << " " << value.square << endl;
    return 0;
}