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
  bool release;
  char trainID[21];
  int stationNum;
  char stations[101][31];
  int seatNum[100][101];
  int seatTotal;
  int prices[101];
  Time startTime;
  Time travelTimes[101];
  Time stopoverTimes[101];
  Time saleDate[2];
  char type[2];
  bool operator<(const Train &rhs) const;
};

struct Order {
  char username[21];
  char trainID[21];
  char from[31];
  Time startTime;
  char to[31];
  Time arriveTime;
  int price;
  int num;
  int status;
  friend std::ostream &operator<<(std::ostream &os, const Order &order);
  bool operator<(const Order &rhs);
};
struct DemoTrain {
  char trainID[21] = {};
  int prices = 0;
  int num = 0;
  Time startTime;
  Time endTime;
};
struct DateLocation_Train {
  Time date;
  char to[31];
  char trainID[21];
  bool operator<(const DateLocation_Train &rhs) const;
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
