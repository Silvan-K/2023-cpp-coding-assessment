#ifndef PRIME_FIELD_ELEMENT_H
#define PRIME_FIELD_ELEMENT_H

#include "al_int.h"
#include "al_uint.h"
#include <cassert>

class BigPrimeField {

public:

  BigPrimeField(const al_int_t& input)  {
    if (input.sign() == +1){
      m_value = input.abs();
      wrapAround();
    }
    else {
      m_value = m_prime -(input.abs()/m_prime).second;
    }
    assert(m_value < m_prime);
  }

  BigPrimeField(const al_uint_t& input) :
    m_value(input) { wrapAround(); }

  BigPrimeField(const std::string& input) :
    m_value(input) { wrapAround(); }
  
  BigPrimeField(uint64_t input) :
    m_value(input) {wrapAround(); }

  BigPrimeField() : m_value() {}

  const al_uint_t::words_t& getData() { return m_value.getData(); }

  static void set_prime(al_uint_t prime) { m_prime = prime; }

  friend BigPrimeField operator+(const BigPrimeField& lhs,
                                 const BigPrimeField& rhs);
  friend BigPrimeField operator-(const BigPrimeField& lhs,
                                 const BigPrimeField& rhs);
  friend BigPrimeField operator*(const BigPrimeField& lhs,
                                 const BigPrimeField& rhs);
  friend BigPrimeField operator/(const BigPrimeField& lhs,
                                 const BigPrimeField& rhs);

  friend BigPrimeField& operator+=(BigPrimeField& lhs,
                                   const BigPrimeField& rhs);
  friend BigPrimeField& operator-=(BigPrimeField& lhs,
                                   const BigPrimeField& rhs);

  friend BigPrimeField operator-(const BigPrimeField& lhs);
  
  friend bool operator==(const BigPrimeField& lhs,
                         const BigPrimeField& rhs);
  friend bool operator<=(const BigPrimeField& lhs,
                         const BigPrimeField& rhs);
  friend bool operator>=(const BigPrimeField& lhs,
                         const BigPrimeField& rhs);
  friend bool operator< (const BigPrimeField& lhs,
                         const BigPrimeField& rhs);
  friend bool operator> (const BigPrimeField& lhs,
                         const BigPrimeField& rhs);

  friend bool operator!=(const BigPrimeField& lhs,
                         const int& rhs);
  friend bool operator==(const BigPrimeField& lhs,
                         const int& rhs);
  
private:

  static al_uint_t m_prime;
  al_uint_t m_value;

  void wrapAround() {
    if (m_value >= m_prime)
      m_value = (m_value/m_prime).second;
  }

  const al_uint_t& value() const { return m_value; }
};


#endif
