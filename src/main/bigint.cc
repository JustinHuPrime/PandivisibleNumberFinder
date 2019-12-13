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

namespace pandivisible {
BigInt::BigInt(int _base) noexcept : base(_base), digits() {}
BigInt::BigInt(int _base, int init) noexcept : base(_base), digits(1, init) {}
size_t BigInt::size() const noexcept { return digits.size(); }

int operator%(BigInt const &number, int modulus) noexcept {
  int remainder = 0;
  for (int digit : number.digits)
    remainder = (digit + number.base * remainder) % modulus;
  return remainder;
}
}  // namespace pandivisible