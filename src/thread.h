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

class TaskManager {
 public:
  TaskManager() = default;
  ~TaskManager() = default;

  void StartSearching();
  void StopSearching();

 private:
  std::vector<std::unique_ptr<Thread>> threads_;
  float alpha_limit_ = 0.f;
  float beta_limit_ = 0.f;
};

}  // namespace Leslie

#endif  // LESLIE_THREAD_MANAGER_H_
