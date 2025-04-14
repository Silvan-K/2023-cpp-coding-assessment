
#include "src/al_uint.h"
#include "src/prime_field_element.h"
#include "gtest/gtest.h"

namespace {

TEST(BigPrimeField, Add) {
  al_uint_t p("1849764852316");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1267650600228229401496703205376"); // 2^100
  BigPrimeField res("1037610352464");

  EXPECT_EQ(n + n, res);
}


TEST(BigPrimeField, SubtractPositive) {
  al_uint_t p("1849764852316");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1267650600228229401496703205376"); // 2^100
  BigPrimeField n2("123456789123456789");
  BigPrimeField res("1114309858401");

  EXPECT_EQ(n2 - n, res);
}


TEST(BigPrimeField, SubtractNegative) {
  al_uint_t p("1849764852316");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1267650600228229401496703205376"); // 2^100
  BigPrimeField n2("123456789123456789");
  BigPrimeField res("735454993915");

  EXPECT_EQ(n - n2, res);
}

TEST(BigPrimeField, Multiply) {
  al_uint_t p("1849764852316");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1267650600228229401496703205376"); // 2^100
  BigPrimeField res("464726607316");

  EXPECT_EQ(n * n, res);
}


TEST(BigPrimeField, Divide) {
  al_uint_t p("1849764852316");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1267650600228229401496703205376"); // 2^100
  BigPrimeField d("123456789");
  BigPrimeField res("1072048663712");

  BigPrimeField got = n / d;

  EXPECT_EQ(got, res);
}

TEST(BigPrimeField, DivideSimple) {
  al_uint_t p("37");
  BigPrimeField::set_prime(p);
  BigPrimeField n("1");
  BigPrimeField d("2");
  BigPrimeField res("19");

  BigPrimeField got = n / d;

  EXPECT_EQ(got, res);
}


} // namespace
