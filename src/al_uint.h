#ifndef AL_UINT_H
#define AL_UINT_H

#include <vector>
#include <climits>
#include <string>
#include <limits>

template<typename word_t, typename accum_t>
class al_uint_t_  {

public:

  using words_t = std::vector<word_t>;
  typedef accum_t accum_t_;

  al_uint_t_() : m_words{} {}

  al_uint_t_(accum_t value);
  
  al_uint_t_(const std::string& input) :
    m_words{decimalToBinary(input.c_str())} {}

  al_uint_t_(const words_t& data);

  static std::string divByTwo(const std::string& input);
  static words_t decimalToBinary(std::string input);

  const words_t& getData() const { return m_words; }
  
  bool operator< (const al_uint_t_<word_t, accum_t>& rhs) const;
  bool operator> (const al_uint_t_<word_t, accum_t>& rhs) const;
  bool operator<=(const al_uint_t_<word_t, accum_t>& rhs) const;
  bool operator>=(const al_uint_t_<word_t, accum_t>& rhs) const;
  bool operator==(const al_uint_t_<word_t, accum_t>& rhs) const;
  bool operator!=(const int& rhs) const;
  bool operator!=(const accum_t& rhs) const;
  bool operator==(const int& rhs) const;
  bool operator==(const accum_t& rhs) const;

private:
  
  words_t m_words;

  static constexpr int wordSize = sizeof(word_t)*CHAR_BIT;
  static constexpr int wordMax = std::numeric_limits<word_t>::max();
  static constexpr int accumSize = sizeof(accum_t)*CHAR_BIT;

  template<typename word_t_, typename accum_t_>
  friend al_uint_t_<word_t_, accum_t_>
  operator+(const al_uint_t_<word_t_, accum_t_>&,
            const al_uint_t_<word_t_, accum_t_>&);
  
  template<typename word_t_, typename accum_t_>
  friend al_uint_t_<word_t_, accum_t_>
  operator-(const al_uint_t_<word_t_, accum_t_>&,
            const al_uint_t_<word_t_, accum_t_>&);

  template<typename word_t_, typename accum_t_>
  friend al_uint_t_<word_t_, accum_t_>
  operator*(const al_uint_t_<word_t_, accum_t_>&,
            const al_uint_t_<word_t_, accum_t_>&);

  template<typename word_t_, typename accum_t_>
  friend std::pair<al_uint_t_<word_t_, accum_t_>,
                   al_uint_t_<word_t_, accum_t_>>
  operator/(const al_uint_t_<word_t_, accum_t_>&,
            const al_uint_t_<word_t_, accum_t_>&);

  template<typename word_t_, typename accum_t_>
  friend al_uint_t_<word_t_, accum_t_>&
  operator+=(al_uint_t_<word_t_, accum_t_>&,
             const al_uint_t_<word_t_, accum_t_>&);

  template<typename word_t_, typename accum_t_>
  friend al_uint_t_<word_t_, accum_t_>&
  operator-=(al_uint_t_<word_t_, accum_t_>&,
             const al_uint_t_<word_t_, accum_t_>&);

  template<typename word_t_, typename accum_t_>
  friend al_uint_t_<word_t_, accum_t_>
  quickDiv(const al_uint_t_<word_t_, accum_t_>& lhs,
           const al_uint_t_<word_t_, accum_t_>& rhs);

};

// template<typename word_t, typename accum_t>
// al_uint_t_<word_t, accum_t>
// operator+(const al_uint_t_<word_t, accum_t>&,
//           const al_uint_t_<word_t, accum_t>&);

// template<typename word_t, typename accum_t>
// al_uint_t_<word_t, accum_t>
// operator-(const al_uint_t_<word_t, accum_t>&,
//           const al_uint_t_<word_t, accum_t>&);

// template<typename word_t, typename accum_t>
// al_uint_t_<word_t, accum_t>
// operator*(const al_uint_t_<word_t, accum_t>&,
//           const al_uint_t_<word_t, accum_t>&);

// template<typename word_t_, typename accum_t_>
// std::pair<al_uint_t_<word_t_, accum_t_>,
//           al_uint_t_<word_t_, accum_t_>>
// operator/(const al_uint_t_<word_t_, accum_t_>&,
//           const al_uint_t_<word_t_, accum_t_>&);

// template<typename word_t, typename accum_t>
// al_uint_t_<word_t, accum_t>
// quickDiv(const al_uint_t_<word_t, accum_t>& lhs,
//          const al_uint_t_<word_t, accum_t>& rhs);

typedef al_uint_t_<uint32_t, uint64_t> al_uint_t;

#endif
