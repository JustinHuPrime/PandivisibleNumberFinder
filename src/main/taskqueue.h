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

#ifndef PANDIVISIBLE_TASKQUEUE_H_
#define PANDIVISIBLE_TASKQUEUE_H_

#include "bigint.h"

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace pandivisible {
// no tasks left exception
class NoTaskException : public std::exception {
 public:
  NoTaskException() noexcept = default;
  NoTaskException(NoTaskException const &) noexcept = default;
  NoTaskException(NoTaskException &&) noexcept = default;

  ~NoTaskException() noexcept = default;

  NoTaskException &operator=(NoTaskException const &) noexcept = default;
  NoTaskException &operator=(NoTaskException &&) noexcept = default;

  char const *what() const noexcept override;

 private:
};
// queue of BigInts, synchronized, with flags to indicate tasks will be added
class TaskQueue {
 public:
  TaskQueue() noexcept;
  TaskQueue(TaskQueue const &) = delete;
  TaskQueue(TaskQueue &&) = delete;

  ~TaskQueue() noexcept = default;

  TaskQueue &operator=(TaskQueue const &) = delete;
  TaskQueue &operator=(TaskQueue &&) = delete;

  // add a task
  void push(std::shared_ptr<BigInt> &&) noexcept;
  void push(std::shared_ptr<BigInt> const &) noexcept;

  // waits for a task, gets it, sets flag for more tasks maybe incoming (from
  // this pop)
  // throws NoTaskException if task queue is empty and no more tasks will come
  std::shared_ptr<BigInt> pop();

  // marks no more tasks incoming (from this pop)
  void done() noexcept;

 private:
  std::mutex lock;
  std::condition_variable newTaskFlag;
  std::queue<std::shared_ptr<BigInt>> tasks;
  size_t maybeMoreCount;  // number of threads that may add tasks
};
}  // namespace pandivisible

#endif  // PANDIVISIBLE_QUEUE_H_