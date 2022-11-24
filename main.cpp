#include <iostream>
#include <string>
#include <complex>
#include <algorithm>
#include <vector>


#define PI 3.141592653589793238462643383279502884L


using std::vector;
using std::string;


typedef std::complex<double> Complex;


const int BASE = 10;


int *str_to_poly(string str) {
    int *p = new int[str.size()]();

    for (size_t i = 0; i < str.size(); i++)
        p[i] = str[i] - '0';

    std::reverse(p, p + str.size());

    return p;
}


string poly_to_str(vector<int> p) {
    string str(p.size(), '0');

    for(int i = 0; i < p.size(); i++)
        str[i] += p[i];

    reverse(str.begin(), str.end());

    return str;
}


template <typename T>
void print_poly(T *p, int size) {
    for (size_t i = 0; i < size; i++)
        std::cout << p[i] << " ";
    
    std::cout << std::endl;
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
Complex *fft(T *p, int size, Complex w) {
    if (size == 1) {
        Complex *res = new Complex[1] {p[0]};
        return res;
    }
    else {
        T *A = new T[size / 2](), *B = new T[size / 2]();

        for(int i = 0; i < size; i++) {
            if (i % 2) B[i / 2] = p[i];
            else       A[i / 2] = p[i];
        }

        Complex *a = fft(A, size / 2, w * w), *b = fft(B, size / 2, w * w), *res = new Complex[size]();

        delete []A;
        delete []B;

        Complex wt = 1;
        
        int k = size / 2;

        for (int i = 0; i < size; i++) {
            res[i] = a[i % k] + wt * b[i % k];
            wt *= w;
        }

        delete []a;
        delete []b;

        return res;
    }
}


void resize(int **p, int old_size, int new_size) {
    int *copy = new int[new_size]();
    for (int i = 0; i < old_size; i++) copy[i] = (*p)[i];
    delete [](*p);
    *p = copy;
}


vector<int> multiply_poly(int **a, int a_size, int **b, int b_size) {
    size_t new_size = 1;
    while (new_size < a_size || new_size < b_size) new_size = new_size << 1;
    new_size = new_size << 1;

    resize(a, a_size, new_size);
    resize(b, b_size, new_size);

    Complex *A = fft(*a, new_size, {cos(2 * PI / new_size), sin(2 * PI / new_size)});
    Complex *B = fft(*b, new_size, {cos(2 * PI / new_size), sin(2 * PI / new_size)});

    for (int i = 0; i < new_size; i++)
        A[i] *= B[i];
    
    delete []B;
    
    A = fft(A, new_size, {cos(-2 * PI / new_size), sin(-2 * PI / new_size)});
    
    vector<int> res(new_size);

    for(int i = 0; i < new_size; i++)
        res[i] = round(real(A[i]) / new_size);
    
    delete []A;

    normalize(&res);

    return res;
}



int main() {
    string a, b;
    std::cin >> a >> b;

    int *A = str_to_poly(a), *B = str_to_poly(b);

    vector<int> res = multiply_poly(&A, a.size(), &B, b.size());

    delete []A;
    delete []B;

    std::cout << "Result " << poly_to_str(res) << std::endl;

    return 0;
}
