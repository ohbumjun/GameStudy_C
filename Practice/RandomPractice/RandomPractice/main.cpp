#include <functional>
#include <iostream>
#include <vector>

using namespace std;

template<typename T, typename Pred>
std::void_t<decltype(std::declval<T>().begin(), std::declval<T>().end())>
deleteEven(T& t, Pred pred)
{
    auto iter = t.begin();
    auto iterEnd = t.end();

    for (; iter != iterEnd;)
    {
        if (pred(iter->first))
        {
            iter = t.erase(iter);
            iterEnd = t.end();
            continue;
        }
        ++iter;
    }


}

int main() {
    std::unordered_map<int, int> m = { {1, 2}, {2, 3}, {3, 4} };

    deleteEven(m, [](int elem) {return (elem & 1) == 0; });
    
    auto iter = m.begin();
    auto iterEnd = m.end();

    for (; iter != iterEnd; ++iter)
    {
        cout << iter->first << endl;
    }

    cout << endl;
}