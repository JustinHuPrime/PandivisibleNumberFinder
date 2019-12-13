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
#include "queue.h"

#include <cstring>
#include <iostream>
#include <thread>

using namespace std;
using namespace pandivisible;

namespace {}

int main(int argc, char *argv[]) {
  int base;
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

  cout << "Hello, world!\n";
  return 0;
}