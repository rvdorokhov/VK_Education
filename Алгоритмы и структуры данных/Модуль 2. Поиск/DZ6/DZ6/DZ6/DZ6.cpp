// Есть список товаров в магазине отсортированный по возрастанию, необходимо понять, есть ли в этом списке товар с заданной ценой. В случае если цена найдена возвращайте true

#include <iostream>
#include <vector>

using namespace std;

template<typename type>
bool bin_search(type search_what, vector <type> mass) {
    int left = 0, right = mass.size() - 1, index = mass.size() / 2;
    bool result = false;
    while ((right >= left)&&(!result)) {
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
    int search_what;
    unsigned int n; cin >> n; vector<int> mass(n);
    for (unsigned int i = 0; i < n; ++i) {
        cin >> mass[i];
    }
    cin >> search_what;

    if (bin_search(search_what, mass) == 1) { cout << "true"; }
    else { cout << "false"; }
}