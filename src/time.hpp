#ifndef TIME_HPP
#define TIME_HPP
#include <iomanip>
#include <iostream>
struct Time {
  int month = 0, day = 0, hour = 0, minute = 0;
  //很愚蠢的做法，可能手动改的month为30，导致进位存在问题
  Time(int x) {
    minute = x % 60;
    x /= 60;
    hour = x % 24;
    x /= 24;
    day = x % 30;
    x /= 30;
    month = x;
  }
  Time() = default;
  Time operator+(const Time &other) const {
    Time result;
    result.minute = this->minute + other.minute;
    result.hour = this->hour + other.hour + result.minute / 60;
    result.minute %= 60;
    result.day = this->day + other.day + result.hour / 24;
    result.hour %= 24;
    result.month = this->month + other.month + result.day / 30;
    result.day %= 30;
    // 假设每个月都是30天，每年都是12个月
    result.month %= 12;
    return result;
  }
  Time &operator+=(const Time &other) {
    *this = *this + other;
    return *this;
  }
  int operator-(const Time &other) const {
    return (this->month - other.month) * 30 + this->day - other.day;
  }
  Time &operator++() {
    this->day++;
    if (day == 30) {
      day = 0;
      month++;
    }
    return *this;
  }
  bool operator<(const Time &other) const {
    if (this->month != other.month) {
      return this->month < other.month;
    }
    if (this->day != other.day) {
      return this->day < other.day;
    }
    if (this->hour != other.hour) {
      return this->hour < other.hour;
    }
    return this->minute < other.minute;
  }
  bool operator==(const Time &other) const {
    return this->month == other.month && this->day == other.day &&
           this->hour == other.hour && this->minute == other.minute;
  }
  bool operator<=(const Time &other) const {
    return *this < other || *this == other;
  }
  bool operator>(const Time &other) const { return !(*this <= other); }
  friend std::ostream &operator<<(std::ostream &os, const Time &time) {
    os << std::setw(2) << std::setfill('0') << time.month << "-" << std::setw(2)
       << std::setfill('0') << time.day << " " << std::setw(2)
       << std::setfill('0') << time.hour << ":" << std::setw(2)
       << std::setfill('0') << time.minute;
    return os;
  }
};
#endif