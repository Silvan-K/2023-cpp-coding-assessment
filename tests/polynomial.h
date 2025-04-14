#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H


template <typename T> class Polynomial {
protected:
  std::vector<T> vals_;
public:
  Polynomial() {  }
  Polynomial(const Polynomial &p) : vals_(p.vals_) {
    remove_trailing_zeros();
  }
  Polynomial(const std::vector<T> &c) : vals_(c) {
    remove_trailing_zeros();
  }

  void remove_trailing_zeros() {
    auto rit = std::find_if(this->vals_.rbegin(), this->vals_.rend(),
                            [](const T &v) { return v != 0; });
    this->vals_.erase(rit.base(), end(this->vals_));
  }

  static Polynomial from_roots(const std::vector<T> &roots) {
    Polynomial p({T(1)});
    for (const T &r : roots) {
      p *= Polynomial({-T(r), T(1)});
    }
    return p;
  }

  bool is_zero() const { return this->vals_.empty(); }

  uint64_t abs() const { return this->vals_.size(); }

  Polynomial neg() const {
    std::vector<T> new_c;
    for (const T &c : this->vals_) {
      new_c.push_back(-c);
    }
    return Polynomial(new_c);
  }

  uint64_t degree() const { return this->abs() - 1; }

  const T &leading_coeff() const { return this->vals_.back(); }

  friend bool operator==(const Polynomial &p1, const Polynomial &p2) {
    if (p1.vals_.size() != p2.vals_.size()) {
      return false;
    }

    for (int i = 0; i < p1.vals_.size(); ++i) {
      if (p1.vals_[i] != p2.vals_[i]) {
        return false;
      }
    }
    return true;
  }

  void operator+=(const Polynomial &p1) {
    if (this->vals_.size() >= p1.vals_.size()) {
      for (int i = 0; i < p1.vals_.size(); ++i) {
        this->vals_[i] += p1.vals_[i];
      }
    } else {
      for (int i = 0; i < this->vals_.size(); ++i) {
        this->vals_[i] += p1.vals_[i];
      }
      for (int i = this->vals_.size(); i < p1.vals_.size(); ++i) {
        this->vals_.push_back(p1.vals_[i]);
      }
    }
    remove_trailing_zeros();
  }
  friend Polynomial operator+(const Polynomial &p1, const Polynomial &p2) {
    Polynomial res(p1);
    res += p2;
    return res;
  }

  void operator-=(const Polynomial &p1) {
    if (this->vals_.size() >= p1.vals_.size()) {
      for (int i = 0; i < p1.vals_.size(); ++i) {
        this->vals_[i] -= p1.vals_[i];
      }
    } else {
      for (int i = 0; i < this->vals_.size(); ++i) {
        this->vals_[i] -= p1.vals_[i];
      }
      for (int i = this->vals_.size(); i < p1.vals_.size(); ++i) {
        this->vals_.push_back(-p1.vals_[i]);
      }
    }
    remove_trailing_zeros();
  }
  friend Polynomial operator-(const Polynomial &p1, const Polynomial &p2) {
    Polynomial res(p1);
    res -= p2;
    return res;
  }

  // Multiply and save result in this
  void operator*=(const Polynomial &p1) {
    if (is_zero() || p1.is_zero()) {
      this->vals_.clear();
    }
    std::vector<T> new_c(this->vals_.size() + p1.vals_.size() - 1, 0);

    for (int i = 0; i < this->vals_.size(); ++i) {
      for (int j = 0; j < p1.vals_.size(); ++j) {
        new_c[i + j] += this->vals_[i] * p1.vals_[j];
      }
    }

    this->vals_.swap(new_c);
    remove_trailing_zeros();
  }

  friend Polynomial operator*(const Polynomial &p1, const Polynomial &p2) {
    Polynomial res(p1);
    res *= p2;
    return res;
  }

  Polynomial divmod(const Polynomial &divisor, Polynomial *rem) const {
    Polynomial q;
    *rem = *this;
    const T &div_lc = divisor.leading_coeff();
    uint64_t div_deg = divisor.degree();
    int64_t monomial_exp = rem->degree() - div_deg;
    
    for (int i = 0; i < 5000 && monomial_exp >= 0; ++i) {
      std::vector<T> monomial_coeffs(monomial_exp, 0);
      monomial_coeffs.push_back(rem->leading_coeff() / div_lc);
      Polynomial monomial_div(monomial_coeffs);
      q += monomial_div;
      *rem -= monomial_div * divisor;
      monomial_exp = rem->degree() - div_deg;
    }
    return q;
  }

  // TODO: put back in
  // std::string str() {
  //   std::string ret;
  //   for (int i = 0; i < this->vals_.size(); ++i) {
  //     ret += this->vals_[i].encode(BASE10) + "x^" + std::to_string(i) + " + ";
  //   }
  //   return ret.substr(0, ret.length() - 3);
  // }

  friend Polynomial operator/(const Polynomial &p1, const Polynomial &p2) {
    if (p2.is_zero()) {
      throw std::runtime_error("Polynomial division by zero.");
    }
    Polynomial rem;
    return p1.divmod(p2, &rem);
  }

  friend Polynomial operator%(const Polynomial &p1, const Polynomial &p2) {
    if (p2.is_zero()) {
      throw std::runtime_error("Polynomial mod by zero.");
    }
    Polynomial rem;
    p1.divmod(p2, &rem);
    return rem;
  }

  // TODO: test this
  Polynomial mul_sub(const Polynomial &b, const Polynomial &c) {
    uint64_t len = max(c.vals_.size(), this->vals_.size() + b.vals_.size() - 1);
    std::vector<T> new_c(len, 0);
    if (is_zero() || b.is_zero()) {
      return Polynomial() - c;
    }

    for (int i = 0; i < this->vals_.size(); ++i) {
      for (int j = 0; j < b.vals_.size(); ++j) {
        new_c[i + j] += this->vals_[i] * b.vals_[j];
      }
      new_c[i] -= c.vals_[i];
    }
    for (int i = this->vals_.size(); i < c.vals_.size(); ++i) {
      new_c.push_back(-c.vals_[i]);
    }

    return Polynomial(new_c);
  }
};

#endif
