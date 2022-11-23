#include <iostream>
#include <vector>


using namespace std;


const int BASE = 10;


vector<int> int_to_poly(int n) {
    vector<int> polynom;

    while (n != 0) {
        polynom.push_back(n % BASE);
        n /= BASE;
    }

    return polynom;
}



int main() {
    int a = 0;
    cin >> a;

    vector<int> poly = int_to_poly(a);

    for(int i = 0; i < poly.size(); i++)
        cout << poly[i] << " ";

    return 0;
}
