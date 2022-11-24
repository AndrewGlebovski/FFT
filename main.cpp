#include <iostream>
#include <complex>
#include <cmath>
#include <vector>


#define PI 3.141592653589793238462643383279502884L


using namespace std;


typedef complex<double> Complex;


const int BASE = 10;


vector<int> int_to_poly(int n) {
    vector<int> p;

    while (n != 0) {
        p.push_back(n % BASE);
        n /= BASE;
    }

    return p;
}


int poly_to_int(vector<int> p) {
    int n = 0, x = 1;

    for(int i = 0; i < p.size(); i++) {
        n += x * p[i];
        x *= BASE;
    }

    return n;
}


template <typename T>
void print_poly(vector<T> p) {
    for (size_t i = 0; i < p.size(); i++)
        cout << p[i] << " ";
    cout << endl;
}


void normalize(vector<int> *p) {
    int extra = 0;

    for (size_t i = 0; i < p -> size(); i++) {
        (*p)[i] += extra;
        extra = (*p)[i] / BASE;
        (*p)[i] %= BASE;
    }

    while (extra > 0) { 
        p -> push_back(extra % BASE);
        extra /= BASE;
    }
}


template <typename T>
vector<Complex> fft(vector<T> p, Complex w) {
    int size = p.size();

    if (size == 1) {
        return {p[0]};
    }
    else {
        vector<T> A, B;

        for(int i = 0; i < size; i++) {
            if (i % 2) B.push_back(p[i]);
            else A.push_back(p[i]);
        }

        vector<Complex> a = fft(A, w * w), b = fft(B, w * w), res(size);

        Complex wt = 1;
        
        int k = size / 2;

        for (int i = 0; i < size; i++) {
            res[i] = a[i % k] + wt * b[i % k];
            wt *= w;
        }

        return res;
    }
}


void multiply_poly(vector<int> *a, vector<int> *b) {
    size_t new_size = 1;
    while (new_size < a -> size() || new_size < b -> size()) new_size = new_size << 1;
    new_size = new_size << 1;

    cout << "New size " << new_size << endl;

    a -> resize(new_size);
    b -> resize(new_size);

    vector<Complex> A = fft(*a, {cos(2 * PI / new_size), sin(2 * PI / new_size)});
    vector<Complex> B = fft(*b, {cos(2 * PI / new_size), sin(2 * PI / new_size)});

    cout << "FFT Polynoms check" << endl; 
    print_poly(A);
    print_poly(B);

    for (int i = 0; i < new_size; i++)
        A[i] *= B[i];

    cout << "Multiply check" << endl; 
    print_poly(A);

    A = fft(A, {cos(-2 * PI / new_size), sin(-2 * PI / new_size)});

    cout << "Inverse FFT check" << endl; 
    print_poly(A);

    for(int i = 0; i < new_size; i++)
        (*a)[i] = round(real(A[i]) / new_size);
    
    normalize(a);

    cout << "Result polynom check" << endl; 

    print_poly(*a);
}



int main() {
    int a = 0, b = 0;
    cin >> a >> b;

    vector<int> A = int_to_poly(a), B = int_to_poly(b);

    cout << "Polynoms created!" << endl;

    cout << "Polynoms check a = " << poly_to_int(A) << " b = " << poly_to_int(B) << endl; 
    print_poly(A);
    print_poly(B);

    multiply_poly(&A, &B);

    cout << "Result " << poly_to_int(A) << endl;

    return 0;
}
