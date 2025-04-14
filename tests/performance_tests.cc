#include <chrono>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "src/al_uint.h"
#include "src/prime_field_element.h"
#include "tests/polynomial.h"


namespace {

std::string gen_random(int len) {
    static const char alphanum[] =
        "0123456789";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}


TEST(BigPrimeField, TestPolyArith) {
    int N = 400;
    int trials = 10;
    int warmup = 3;
    al_uint_t p("5678423945204805371863426");
    BigPrimeField::set_prime(p);
    std::vector<BigPrimeField> a_coeffs(N), b_coeffs(N), c_coeffs(2*N - 1, 0);

    for (int i = 0; i < N; ++i) {
        a_coeffs[i] = BigPrimeField(gen_random(10 + rand() % 20));
        b_coeffs[i] = BigPrimeField(gen_random(10 + rand() % 20));
    }

    Polynomial a(a_coeffs), b(b_coeffs), c(c_coeffs);

    for (int i = 0; i < warmup; ++i) {
        c += a * b;
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < trials; ++i) {
        c += a * b;
    }
    
    auto elapsed = std::chrono::high_resolution_clock::now() - start;

    long long mult_time = std::chrono::duration_cast<std::chrono::microseconds>(
            elapsed).count() / trials;

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < trials; ++i) {
        c = a / b;
    }
    elapsed = std::chrono::high_resolution_clock::now() - start;
    long long div_time = std::chrono::duration_cast<std::chrono::microseconds>(
            elapsed).count() / trials;


    std::cout << "Time (us): " << mult_time + div_time << std::endl;

}


}
