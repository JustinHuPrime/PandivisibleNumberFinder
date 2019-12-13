// Copyright 2019 Justin Hu
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

#ifndef PANDIVISIBLE_SCOPEGUARD_H_
#define PANDIVISIBLE_SCOPEGUARD_H_

#include <functional>

namespace pandivisible {
// scope guard implementation
// not movable nor copyable, calls a function on scope exit
class ScopeGuard {
 public:
  ScopeGuard(std::function<void()> const &) noexcept;
  ScopeGuard(ScopeGuard const &) = delete;
  ScopeGuard(ScopeGuard &&) = delete;

  ~ScopeGuard();

  ScopeGuard &operator=(ScopeGuard const &) = delete;
  ScopeGuard &operator=(ScopeGuard &&) = delete;

 private:
  std::function<void()> toCall;
};
}  // namespace pandivisible

#endif  // PANDIVISIBLE_SCOPEGUARD_H_