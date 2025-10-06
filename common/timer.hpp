#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

namespace timer {
class Timer {
 private:
  using Clock = std::chrono::high_resolution_clock;

  const double kNanosecondsToSecond = 1e-9;
  const double kNanosecondsToMilliseconds = 1e-6;

 public:
  Timer() = default;

  void start();

  long long elapsed_nanoseconds();
  double elapsed_seconds();
  double elapsed_milliseconds();

 private:
  Clock::time_point start_time_;
};

inline void Timer::start() {
  start_time_ = Clock::now();
}

inline long long Timer::elapsed_nanoseconds() {
  auto end_time = Clock::now(); 
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time_);
  return duration.count();
}

inline double Timer::elapsed_seconds() {
  return static_cast<double>(elapsed_nanoseconds()) * kNanosecondsToSecond;
}

inline double Timer::elapsed_milliseconds() {
  return static_cast<double>(elapsed_nanoseconds()) * kNanosecondsToMilliseconds;
}

} // namespace timer


#endif // TIMER_HPP_
