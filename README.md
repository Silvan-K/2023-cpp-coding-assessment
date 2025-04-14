# 2023 C++ coding challenge

The tasks for this challenge were the following:

## 1) Implement a datatype for arbitrarily large integers

Can be found in `./src/al_uint.{h,cc}` (unsigned) and `./src/al_int.h` (signed). Includes support for  :

- Addition
- Subtraction
- Multiplication
- Division (returns quotient and remainder)

## 2) Based on the above, implement a class for representing prime field elements of arbitrary size

Operations on prime field elements are understood to be modulo a certain prime number. The prime number has to be changeable at runtime. Can be found in `./src/prime_field_element.{h,cc}`. Includes support for:

- Addition
- Subtraction
- Multiplication
- Division (finding a multiplicative inverse)
  
# Build instructions

Decided to use this project to test out the Meson build system. To compile and test, please run the commands below. Dependencies (gtest) are taken care of automatically. Compilation artifacts will appear under `./build/`.
 
```
meson setup build .
meson compile -C build
meson test -C build
```

