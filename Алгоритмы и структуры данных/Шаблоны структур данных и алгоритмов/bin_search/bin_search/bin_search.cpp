#include <iostream>
#include <vector>

using namespace std;

template<typename type>
bool bin_search(type search_what, vector <type> mass) {
    int left = 0, right = mass.size() - 1, index = mass.size() / 2;
    bool result = false;
    while ((right >= left) && (!result)) {
        if (search_what == mass[index]) {
            result = true;
        }
        else if (search_what > mass[index]) {
            left = index + 1;
        }
        else {
            right = index - 1;
        }
        index = left + (right - left) / 2;
    }

    return result;
}

int main()
{
}