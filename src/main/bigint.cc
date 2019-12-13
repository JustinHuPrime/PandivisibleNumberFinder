// Copyright 2019 Justin Hu
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file is part of the Pandivisible Number Finder.

#include "bigint.h"

#include <algorithm>
#include <numeric>

using namespace std;

namespace pandivisible {
BigInt::BigInt(int _base, int init) noexcept : base(_base), digits(1, init) {}
BigInt::operator string() const noexcept {
  string s = "[" + to_string(digits.front());
  s += accumulate(digits.cbegin() + 1, digits.cend(), ""s,
                  [](string const &init, int digit) {
                    return init + ":" + to_string(digit);
                  });
  return s + "]";
}
size_t BigInt::size() const noexcept { return digits.size(); }
BigInt BigInt::extend(int digit) const noexcept {
  BigInt retval(*this);
  retval.digits.push_back(digit);
  return retval;
}
std::vector<int>::const_iterator BigInt::cbegin() const noexcept {
  return digits.cbegin();
}
std::vector<int>::const_iterator BigInt::cend() const noexcept {
  return digits.cend();
}

int operator%(BigInt const &number, int modulus) noexcept {
  int remainder = 0;
  for (int digit : number.digits)
    remainder = (digit + number.base * remainder) % modulus;
  return remainder;
}
}  // namespace pandivisible