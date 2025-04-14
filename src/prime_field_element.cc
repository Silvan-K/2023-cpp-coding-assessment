#include "src/prime_field_element.h"

namespace {

BigPrimeField findInverse(const al_uint_t& input,
                          const al_uint_t& prime) {
  auto r2 = al_int_t(input); auto r1 = al_int_t(prime);
  auto s2 = al_int_t(1); auto s1 = al_int_t(0);
  auto t2 = al_int_t(0); auto t1 = al_int_t(1);
  while(!r1.zero()) {
    const auto divRes = r2/r1;
    const auto& q = divRes.first;
      // al_int_t((r2.abs()/r1.abs()).first,
      // r2.sign()!=r1.sign() ? -1 : 1);
    r2 = r1; r1 = divRes.second; // r2-q*r1;
    auto s1tmp = s2-q*s1;
    s2 = s1; s1 = s1tmp;
    auto t1tmp = t2-q*t1;
    t2 = t1; t1 - t1tmp;
  }
  return BigPrimeField(s2);
}

}

al_uint_t BigPrimeField::m_prime = al_uint_t("82719359238349327729181123");

BigPrimeField operator+(const BigPrimeField& lhs,
                        const BigPrimeField& rhs) {
  return BigPrimeField(lhs.m_value+rhs.m_value);
}

BigPrimeField operator-(const BigPrimeField& lhs,
                        const BigPrimeField& rhs) {
  if(rhs <= lhs)
    return BigPrimeField(lhs.m_value-rhs.m_value);
  const auto diff = rhs.m_value - lhs.m_value;
  return BigPrimeField(BigPrimeField::m_prime -
                       (diff/BigPrimeField::m_prime).second);
}

BigPrimeField operator*(const BigPrimeField& lhs,
                        const BigPrimeField& rhs) {
  return BigPrimeField(lhs.m_value*rhs.m_value);
}

BigPrimeField operator/(const BigPrimeField& lhs,
                        const BigPrimeField& rhs) {
  return findInverse(rhs.m_value,
                     BigPrimeField::m_prime)*lhs;
}

BigPrimeField& operator+=(BigPrimeField& lhs,
                          const BigPrimeField& rhs) {
  lhs.m_value += rhs.m_value;
  lhs.wrapAround();
  return lhs;
}

BigPrimeField& operator-=(BigPrimeField& lhs,
                          const BigPrimeField& rhs) {
  lhs = BigPrimeField(lhs-rhs);
  return lhs;
}

BigPrimeField operator-(const BigPrimeField& lhs) {
  return BigPrimeField(BigPrimeField::m_prime -
                       lhs.m_value);
}

bool operator==(const BigPrimeField& lhs,
                const BigPrimeField& rhs) {
  return (lhs.value().getData()==
          rhs.value().getData());
}

bool operator<=(const BigPrimeField& lhs,
                const BigPrimeField& rhs) {
  return (lhs.value()<=rhs.value());
}

bool operator>=(const BigPrimeField& lhs,
                const BigPrimeField& rhs) {
  return (lhs.value()>=rhs.value());
}

bool operator<(const BigPrimeField& lhs,
                const BigPrimeField& rhs) {
  return (lhs.value()<rhs.value());
}

bool operator>(const BigPrimeField& lhs,
                const BigPrimeField& rhs) {
  return (lhs.value()>rhs.value());
}

bool operator!=(const BigPrimeField& lhs,
                const int& rhs) {
  return lhs.m_value!=rhs;
}

bool operator==(const BigPrimeField& lhs,
                const int& rhs) {
  return lhs.m_value==rhs;
  
}
