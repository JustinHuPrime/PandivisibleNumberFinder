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
namespace {
using BigIntConstIterator = BigInt::BigIntConstIterator;
}

BigIntConstIterator::BigIntConstIterator(BigInt const *_curr) noexcept
    : curr(_curr) {}
bool operator!=(BigIntConstIterator const &a,
                BigIntConstIterator const &b) noexcept {
  return a.curr != b.curr;
}
bool operator==(BigIntConstIterator const &a,
                BigIntConstIterator const &b) noexcept {
  return a.curr == b.curr;
}
BigIntConstIterator::reference BigIntConstIterator::operator*() const noexcept {
  return curr->digit;
}
BigIntConstIterator::pointer BigIntConstIterator::operator->() const noexcept {
  return &curr->digit;
}
BigIntConstIterator &BigIntConstIterator::operator++() noexcept {
  curr = curr->parent();
  return *this;
}
BigIntConstIterator BigIntConstIterator::operator++(int) noexcept {
  BigIntConstIterator retval = *this;
  operator++();
  return retval;
}

int operator%(BigInt const &i, int modulus) noexcept {
  return i.moduloBase(modulus).first;
}
BigIntConstIterator BigInt::cbegin() const noexcept {
  return BigIntConstIterator(this);
}
BigIntConstIterator BigInt::cend() const noexcept {
  return BigIntConstIterator(nullptr);
}
BigInt::BigInt(int _digit) noexcept : digit(_digit) {}

BigIntRoot::BigIntRoot(int _base, int digit) noexcept
    : BigInt(digit), base(_base) {}
BigIntRoot::operator string() const noexcept {
  return "[" + to_string(digit) + "]";
}
size_t BigIntRoot::size() const noexcept { return 1; }
pair<int, int> BigIntRoot::moduloBase(int modulus) const noexcept {
  return {digit % modulus, base};
}
BigInt const *BigIntRoot::parent() const noexcept { return nullptr; }

BigIntDelta::BigIntDelta(shared_ptr<BigInt> const &_from, int digit) noexcept
    : BigInt(digit), from(_from) {}
BigIntDelta::operator string() const noexcept {
  string s = string(*from);
  s.insert(s.size() - 1, ":" + to_string(digit));
  return s;
}
size_t BigIntDelta::size() const noexcept { return from->size() + 1; }
pair<int, int> BigIntDelta::moduloBase(int modulus) const noexcept {
  auto parent = from->moduloBase(modulus);
  parent.first = (digit + parent.second * parent.first) % modulus;
  return parent;
}
BigInt const *BigIntDelta::parent() const noexcept { return from.get(); }
}  // namespace pandivisible