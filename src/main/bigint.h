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

#ifndef PANDIVISIBLE_BIGINT_H_
#define PANDIVISIBLE_BIGINT_H_

#include <cstdint>
#include <string>
#include <vector>

namespace pandivisible {
// a big integer with some base
// assumes no leading zeroes, may not be empty
class BigInt {
 public:
  BigInt(int base, int digit) noexcept;
  BigInt(BigInt const &) = default;
  BigInt(BigInt &&) noexcept = default;

  ~BigInt() noexcept = default;

  BigInt &operator=(BigInt const &) = default;
  BigInt &operator=(BigInt &&) noexcept = default;

  friend int operator%(BigInt const &, int modulus) noexcept;

  operator std::string() const noexcept;

  size_t size() const noexcept;

  // get new number with digit appended to the end
  BigInt extend(int digit) const noexcept;

  std::vector<int>::const_iterator cbegin() const noexcept;
  std::vector<int>::const_iterator cend() const noexcept;

 private:
  int const base;
  std::vector<int> digits;
};

int operator%(BigInt const &number, int modulus) noexcept;
}  // namespace pandivisible

#endif  // PANDIVISIBLE_BIGINT_H_