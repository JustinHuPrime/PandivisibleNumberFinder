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

#include "taskqueue.h"

using namespace std;

namespace pandivisible {
char const *NoTaskException::what() const noexcept {
  return "no tasks left in queue";
}
TaskQueue::TaskQueue() noexcept
    : lock(), newTaskFlag(), tasks(), maybeMoreCount(0) {}
void TaskQueue::push(shared_ptr<BigInt> &&task) noexcept {
  lock_guard synchronize(lock);
  tasks.push(move(task));
  newTaskFlag.notify_one();
}
void TaskQueue::push(shared_ptr<BigInt> const &task) noexcept {
  lock_guard synchronize(lock);
  tasks.push(task);
  newTaskFlag.notify_one();
}
shared_ptr<BigInt> TaskQueue::pop() {
  unique_lock synchronize(lock);
  while (tasks.empty() && maybeMoreCount != 0) {
    // wait until tasks isn't empty or until maybeMoreCount is zero
    newTaskFlag.wait(synchronize);
  }

  if (tasks.empty()) {
    // no more tasks!
    throw NoTaskException();
  } else {
    shared_ptr<BigInt> retval = tasks.front();
    tasks.pop();
    maybeMoreCount++;  // this task might generate more tasks
    return retval;
  }
}
void TaskQueue::done() noexcept {
  lock_guard synchronize(lock);
  maybeMoreCount--;
  if (maybeMoreCount == 0) newTaskFlag.notify_all();
}
}  // namespace pandivisible