#ifndef LESLIE_THREAD_MANAGER_H_
#define LESLIE_THREAD_MANAGER_H_

#include <memory>
#include <vector>

namespace Leslie {

class Thread {
 public:
  Thread() = default;
  ~Thread() = default;
};

class ThreadManager {
 public:
  ThreadManager() = default;
  ~ThreadManager() = default;

  void StartSearching() {}
  void StopSearching() {}

 private:
  std::vector<std::unique_ptr<Thread>> threads;
};

}  // namespace Leslie

#endif  // LESLIE_THREAD_MANAGER_H_
