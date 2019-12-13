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
#include "scopeguard.h"
#include "taskqueue.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
using namespace pandivisible;

namespace {}

int main(int argc, char *argv[]) {
  int base = -1;
  size_t nthreads = 1;

  for (int idx = 1; idx < argc; idx++) {
    if (argv[idx] == "-h"s || argv[idx] == "-?"s || argv[idx] == "--help"s) {
      cout << "Usage: finder BASE [-t NTHREADS]\n"
              "Finds all pandivisible numbers with the given BASE.\n"
              "\n"
              "-t NTHREADS  use specified number of threads to search.\n";
      return EXIT_SUCCESS;
    } else if (argv[idx] == "-v"s || argv[idx] == "--version"s) {
      cout << "finder version 0.1.0\n"
              "Copyright 2019 Justin Hu\n"
              "This software is licensed under the Apache License, Version "
              "2.0.\n"
              "See the \"LICENSE\" file for copying conditions.\n"
              "This software is distributed on an \"AS IS\" BASIS, WITHOUT "
              "WARRANTIES OR\n"
              "CONDITIONS OF ANY KIND\n";
      return EXIT_SUCCESS;
    }
  }

  bool encounteredBase = false;
  bool encounteredNThreads = false;
  for (int idx = 1; idx < argc; idx++) {
    if (argv[idx] == "-t"s) {
      if (encounteredNThreads) {
        // error - already saw -t
        cerr << argv[0]
             << ": encountered thread count specifier multiple times\n";
        return EXIT_FAILURE;
      } else if (idx + 1 >= argc) {
        // error = no nthreads to read
        cerr << argv[0] << ": `-t` expects number of threads as argument\n";
        return EXIT_FAILURE;
      } else {
        encounteredNThreads = true;
      }
    } else {
      if (encounteredBase && !encounteredNThreads) {
        // error - already saw base
        cerr << argv[0] << ": encountered base multiple times\n";
      } else {
        // was there a -t before this?
        if (encounteredNThreads) {
          // set nthreads
          char *endPtr;
          unsigned long val = strtoul(argv[idx], &endPtr, 0);
          if (endPtr != argv[idx] + strlen(argv[idx])) {
            cerr << argv[0] << ": could not parse number of threads\n";
            return EXIT_FAILURE;
          }
          if (val > numeric_limits<size_t>().max()) {
            cerr << argv[0] << ": number of threads out of range\n";
            return EXIT_FAILURE;
          }
          nthreads = val;
        } else {
          // set base
          char *endPtr;
          unsigned long val = strtoul(argv[idx], &endPtr, 0);
          if (endPtr != argv[idx] + strlen(argv[idx])) {
            cerr << argv[0] << ": could not parse base\n";
            return EXIT_FAILURE;
          }
          if (val > numeric_limits<int>().max() || val < 2) {
            cerr << argv[0] << ": base out of range\n";
            return EXIT_FAILURE;
          }
          base = static_cast<int>(val);
          encounteredBase = true;
        }
      }
    }
  }
  if (!encounteredBase) {
    cerr << argv[0] << ": missing base\n";
    return EXIT_FAILURE;
  }

  cout << "Searching for pandivisible number of base " << base << " with "
       << nthreads << " threads\n";

  if (base % 2 != 0) {
    cout << "None found, trivially!\n";
    return EXIT_SUCCESS;
  }

  vector<thread> workers;
  workers.reserve(nthreads);

  TaskQueue tasks;

  mutex outputLock;
  bool foundOne = false;

  vector<int> evenDigits;
  vector<int> oddDigits;

  evenDigits.reserve(static_cast<size_t>(base / 2));
  oddDigits.reserve(static_cast<size_t>(base / 2));
  for (int digit = 1; digit < base; digit++) {
    (digit % 2 == 0 ? evenDigits : oddDigits).push_back(digit);
  }

  // adding the 1st digit, one is odd
  for_each(oddDigits.cbegin(), oddDigits.cend(), [&tasks, base](int digit) {
    tasks.push(make_shared<BigIntRoot>(base, digit));
  });

  for (size_t idx = 0; idx < nthreads; idx++) {
    workers.push_back(thread([&, base ]() noexcept {
      try {
        while (true) {
          shared_ptr<BigInt> i = tasks.pop();
          ScopeGuard doneGuard([&tasks]() noexcept { tasks.done(); });

          if (*i % static_cast<int>(i->size()) != 0) {
            // candidate failed
            continue;
          } else if (static_cast<int>(i->size()) == base - 1) {
            // long enough - output
            lock_guard synchronize(outputLock);
            cout << "Found " << string(*i) << "\n";
            foundOne = true;
          } else {
            // get usable set - if adding to even length, get odd set, else
            // even set
            vector<int> usableDigits =
                (i->size() % 2 == 0 ? oddDigits : evenDigits);
            for_each(i->cbegin(), i->cend(), [&usableDigits](int digit) {
              auto toRemove =
                  equal_range(usableDigits.begin(), usableDigits.end(), digit);
              usableDigits.erase(toRemove.first, toRemove.second);
            });
            for_each(usableDigits.cbegin(), usableDigits.cend(),
                     [&tasks, &i](int digit) {
                       tasks.push(make_unique<BigIntDelta>(i, digit));
                     });
          }
        }
      } catch (NoTaskException const &) {
        return;  // worker done! no work left
      }
    }));
  }

  for_each(workers.begin(), workers.end(),
           [](thread &worker) { worker.join(); });

  cout << (foundOne ? "Done!\n" : "None found!\n");

  return 0;
}