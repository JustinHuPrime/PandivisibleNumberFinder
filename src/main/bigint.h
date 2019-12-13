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
#include <memory>
#include <string>

namespace pandivisible {
class BigIntRoot;
class BigIntDelta;

// a big integer stored as a parent or a delta plus pointer to parent
// assumes no leading zeroes, may not be empty
class BigInt {
  friend class BigIntDelta;
  friend class BigIntRoot;

 public:
  // constant iterator through the digits of a BigInt
  class BigIntConstIterator {
   public:
    using difference_type = ptrdiff_t;
    using value_type = int;
    using pointer = int const *;
    using reference = int const &;
    using iterator_category = std::input_iterator_tag;

    BigIntConstIterator(BigInt const *) noexcept;
    BigIntConstIterator(BigIntConstIterator const &) noexcept = default;
    BigIntConstIterator(BigIntConstIterator &&) noexcept = default;

    ~BigIntConstIterator() noexcept = default;

    BigIntConstIterator &operator=(BigIntConstIterator const &) noexcept =
        default;
    BigIntConstIterator &operator=(BigIntConstIterator &&) noexcept = default;

    friend bool operator!=(BigIntConstIterator const &,
                           BigIntConstIterator const &) noexcept;
    friend bool operator==(BigIntConstIterator const &,
                           BigIntConstIterator const &) noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    BigIntConstIterator &operator++() noexcept;
    BigIntConstIterator operator++(int) noexcept;

   private:
    BigInt const *curr;
  };

  BigInt(BigInt const &) noexcept = default;
  BigInt(BigInt &&) noexcept = default;

  virtual ~BigInt() noexcept = default;

  BigInt &operator=(BigInt const &) noexcept = default;
  BigInt &operator=(BigInt &&) noexcept = default;

  friend int operator%(BigInt const &, int modulus) noexcept;

  virtual operator std::string() const noexcept = 0;

  virtual size_t size() const noexcept = 0;

  BigIntConstIterator cbegin() const noexcept;
  BigIntConstIterator cend() const noexcept;

 protected:
  int digit;

  BigInt(int digit) noexcept;

  // returns pair of remainder, base
  virtual std::pair<int, int> moduloBase(int modulus) const noexcept = 0;
  // get parent digit, if any
  virtual BigInt const *parent() const noexcept = 0;
};
class BigIntRoot : public BigInt {
 public:
  BigIntRoot(int base, int digit) noexcept;
  BigIntRoot(BigIntRoot const &) noexcept = default;
  BigIntRoot(BigIntRoot &&) noexcept = default;

  ~BigIntRoot() noexcept = default;

  BigIntRoot &operator=(BigIntRoot const &) noexcept = default;
  BigIntRoot &operator=(BigIntRoot &&) noexcept = default;

  operator std::string() const noexcept override;

  size_t size() const noexcept override;

 protected:
  int base;

  std::pair<int, int> moduloBase(int modulus) const noexcept override;
  BigInt const *parent() const noexcept override;
};
class BigIntDelta : public BigInt {
 public:
  BigIntDelta(std::shared_ptr<BigInt> const &from, int digit) noexcept;
  BigIntDelta(BigIntDelta const &) noexcept = default;
  BigIntDelta(BigIntDelta &&) noexcept = default;

  ~BigIntDelta() noexcept = default;

  BigIntDelta &operator=(BigIntDelta const &) noexcept = default;
  BigIntDelta &operator=(BigIntDelta &&) noexcept = default;

  operator std::string() const noexcept override;

  size_t size() const noexcept override;

 protected:
  std::shared_ptr<BigInt> from;

  std::pair<int, int> moduloBase(int modulus) const noexcept override;
  BigInt const *parent() const noexcept override;
};

bool operator!=(BigInt::BigIntConstIterator const &,
                BigInt::BigIntConstIterator const &) noexcept;
bool operator==(BigInt::BigIntConstIterator const &,
                BigInt::BigIntConstIterator const &) noexcept;
int operator%(BigInt const &number, int modulus) noexcept;

std::shared_ptr<BigInt> extend(std::shared_ptr<BigInt> const &parent,
                               int digit) noexcept;
}  // namespace pandivisible

#endif  // PANDIVISIBLE_BIGINT_H_