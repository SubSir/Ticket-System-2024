#ifndef TRAIN_HPP
#define TRAIN_HPP

#include "time.hpp"
#include <cstring>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

bool valid_trainID(const std::string &u);
bool valid_stationNum(int g);
bool valid_stations(const std::string &s);
bool valid_seatNum(int g);
bool valid_prices(int g);
bool valid_startTime(const std::string &t);
bool valid_travelTimes(int g);
bool valid_stopoverTimes(int g);
bool valid_saleDate(const std::string &d);
bool valid_type(const std::string &t);

struct Train {
  bool release = false;
  char trainID[21] = {};
  int stationNum = 0;
  char stations[101][31] = {};
  int seatTotal = 0;
  int seatNum[100][101] = {}; // 你知道这很浪费
  int prices[101] = {};
  Time startTime;
  Time travelTimes[101];
  Time stopoverTimes[101];
  Time saleDate[2];
  char type[2];
  bool operator<(const Train &rhs) const {
    int trainID_cmp = std::strcmp(trainID, rhs.trainID);
    if (trainID_cmp < 0) {
      return true;
    } else if (trainID_cmp > 0) {
      return false;
    }
    if (stationNum < rhs.stationNum) {
      return true;
    } else if (stationNum > rhs.stationNum) {
      return false;
    }
    for (int i = 0; i < stationNum; i++) {
      int stations_cmp = std::strcmp(stations[i], rhs.stations[i]);
      if (stations_cmp < 0) {
        return true;
      } else if (stations_cmp > 0) {
        return false;
      }
    }
    if (seatTotal < rhs.seatTotal) {
      return true;
    } else if (seatTotal > rhs.seatTotal) {
      return false;
    }
    for (int i = 0; i < stationNum; i++) {
      if (prices[i] < rhs.prices[i]) {
        return true;
      } else if (prices[i] > rhs.prices[i]) {
        return false;
      }
    }
    if (startTime < rhs.startTime) {
      return true;
    } else if (startTime == rhs.startTime) {
      for (int i = 0; i < stationNum; i++) {
        if (travelTimes[i] < rhs.travelTimes[i]) {
          return true;
        } else if (travelTimes[i] > rhs.travelTimes[i]) {
          return false;
        }
      }
      for (int i = 0; i < stationNum; i++) {
        if (stopoverTimes[i] < rhs.stopoverTimes[i]) {
          return true;
        } else if (stopoverTimes[i] > rhs.stopoverTimes[i]) {
          return false;
        }
      }
      for (int i = 0; i < 2; i++) {
        if (saleDate[i] < rhs.saleDate[i]) {
          return true;
        } else if (saleDate[i] > rhs.saleDate[i]) {
          return false;
        }
      }
      int type_cmp = std::strcmp(type, rhs.type);
      if (type_cmp < 0) {
        return true;
      } else if (type_cmp > 0) {
        return false;
      }
    }
    return false;
  }
  bool operator>(const Train &rhs) const { return rhs < *this; }
  bool operator>=(const Train &rhs) const { return !(*this < rhs); }
  bool operator<=(const Train &rhs) const { return !(rhs < *this); }
  bool operator!=(const Train &rhs) const {
    return (*this < rhs) or (rhs < *this);
  }
};
struct Order {
  char username[21] = {};
  char trainID[21] = {};
  char from[31] = {};
  Time startTime;
  char to[31] = {};
  Time arriveTime;
  int price = 0;
  int num = 0;
  int status = 0;
  friend std::ostream &operator<<(std::ostream &os, const Order &order) {
    if (order.status == 1) {
      os << "[success] ";
    } else if (order.status == 2) {
      os << "[pending] ";
    } else {
      os << "[refunded] ";
    }
    os << order.trainID << ' ' << order.from << ' ' << order.startTime << " -> "
       << order.to << ' ' << order.arriveTime << ' ' << order.price << ' '
       << order.num << '\n';
    return os;
  }
  bool operator<(const Order &rhs) const {
    int username_cmp = std::strcmp(username, rhs.username);
    if (username_cmp < 0) {
      return true;
    } else if (username_cmp > 0) {
      return false;
    }
    int trainID_cmp = std::strcmp(trainID, rhs.trainID);
    if (trainID_cmp < 0) {
      return true;
    } else if (trainID_cmp > 0) {
      return false;
    }
    int from_cmp = std::strcmp(from, rhs.from);
    if (from_cmp < 0) {
      return true;
    } else if (from_cmp > 0) {
      return false;
    }
    if (startTime < rhs.startTime) {
      return true;
    } else if (startTime == rhs.startTime) {
      int to_cmp = std::strcmp(to, rhs.to);
      if (to_cmp < 0) {
        return true;
      } else if (to_cmp > 0) {
        return false;
      }
      if (arriveTime < rhs.arriveTime) {
        return true;
      } else if (arriveTime == rhs.arriveTime) {
        if (price < rhs.price) {
          return true;
        } else if (price == rhs.price) {
          if (num < rhs.num) {
            return true;
          } else if (num == rhs.num) {
            return status < rhs.status;
          }
        }
      }
    }
    return false;
  }
  bool operator>(const Order &rhs) const { return rhs < *this; }
  bool operator>=(const Order &rhs) const { return !(*this < rhs); }
  bool operator<=(const Order &rhs) const { return !(rhs < *this); }
  bool operator!=(const Order &rhs) const {
    return (*this < rhs) or (rhs < *this);
  }
};
struct DemoTrain {
  char trainID[21] = {};
  int prices = 0;
  int num = 0;
  Time startTime;
  Time endTime;
  bool operator<(const DemoTrain &rhs) const {
    if (startTime < rhs.startTime) {
      return true;
    } else if (startTime == rhs.startTime) {
      if (endTime < rhs.endTime) {
        return true;
      } else if (endTime == rhs.endTime) {
        return prices < rhs.prices;
      }
    }
    return false;
  }
  bool operator>(const DemoTrain &rhs) const { return rhs < *this; }
  bool operator>=(const DemoTrain &rhs) const { return !(*this < rhs); }
  bool operator<=(const DemoTrain &rhs) const { return !(rhs < *this); }
  bool operator!=(const DemoTrain &rhs) const {
    return (*this < rhs) or (rhs < *this);
  }
};
struct DateLocation_Train {
  Time date;
  char to[31];
  char trainID[21];
  bool operator<(const DateLocation_Train &rhs) const {
    if (date.month < rhs.date.month) {
      return true;
    } else if (date.month == rhs.date.month) {
      if (date.day < rhs.date.day) {
        return true;
      } else if (date.day == rhs.date.day) {
        if (std::strcmp(to, rhs.to) < 0) {
          return true;
        } else if (std::strcmp(to, rhs.to) == 0) {
          return std::strcmp(trainID, rhs.trainID) < 0;
        }
      }
    }
    return false;
  }
  bool operator>(const DateLocation_Train &rhs) const { return rhs < *this; }
  bool operator>=(const DateLocation_Train &rhs) const {
    return !(*this < rhs);
  }
  bool operator<=(const DateLocation_Train &rhs) const {
    return !(rhs < *this);
  }
  bool operator!=(const DateLocation_Train &rhs) const {
    return (*this < rhs) or (rhs < *this);
  }
};
Train *add_train(const std::string &i, int n, int m, const std::string &s,
                 const std::string p, const std::string &x,
                 const std::string &t, const std::string &o,
                 const std::string &d, const std::string &y);

void query_trains(Train *train, Time *date);

Order *buy_ticket(Train &train, const std::string &u, const std::string &i,
                  const std::string &d, const std::string &f,
                  const std::string &t, int n, bool p);
bool timecmp(const DemoTrain &a, const DemoTrain &b);
bool costcmp(const DemoTrain &a, const DemoTrain &b);
#endif
