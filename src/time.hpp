#ifndef TIME_HPP
#define TIME_HPP
#include <iomanip>
#include <iostream>

struct Time {
  int month = 0, day = 0, hour = 0, minute = 0;
  static int month_day[13];
  static int month_days[13];
  Time(int x) {
    minute = x % 60;
    x /= 60;
    hour = x % 24;
    x /= 24;
    month_date(x, month, day);
  }
  Time() = default;
  Time(const Time &other) {
    month = other.month;
    day = other.day;
    hour = other.hour;
    minute = other.minute;
  }
  Time operator+(const Time &other) const {
    Time result;
    result.minute = this->minute + other.minute;
    result.hour = this->hour + other.hour + result.minute / 60;
    result.minute %= 60;
    result.day = result.hour / 24;
    result.hour %= 24;
    int x1, x2;
    date_month(this->month, this->day, x1);
    date_month(other.month, other.day, x2);
    x1 += x2;
    x1 += result.day;
    month_date(x1, result.month, result.day);
    return result;
  }
  Time &operator+=(const Time &other) {
    *this = *this + other;
    return *this;
  }
  int operator-(const Time &other) const {
    int x1, x2;
    date_month(this->month, this->day, x1);
    date_month(other.month, other.day, x2);
    return x1 - x2;
  }
  Time &operator++() {
    this->day++;
    if (day == month_day[month] + 1) {
      day = 1;
      month++;
    }
    return *this;
  }

  static void month_date(int x, int &month, int &day) {
    for (int i = 1; i < 13; i++) {
      if (x <= month_days[i]) {
        month = i - 1;
        break;
      }
    }
    day = x - month_days[month];
  }
  static void date_month(int month, int day, int &x) {
    x = month_days[month] + day;
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