#include "train.hpp"
#include "time.hpp"
#include <cstring>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
bool valid_trainID(const std::string &u) {
  if (u.length() == 0 || u.length() > 20 || u[0] > 'z' ||
      (u[0] < 'a' && (u[0] < 'A' || u[0] > 'Z')))
    return false;
  for (char c : u) {
    if (c != '_' and !isalnum(c))
      return false;
  }
  return true;
}
bool valid_stationNum(int g) { return g >= 2 && g <= 100; }
bool valid_stations(const std::string &s) {
  std::regex pattern("^[\u4e00-\u9fa5]{1,10}$");
  return std::regex_match(s, pattern);
}
bool valid_seatNum(int g) { return g <= 100000; }
bool valid_prices(int g) { return g <= 100000; }
bool valid_startTime(const std::string &t) {
  std::regex pattern("^[0-9]{2}:[0-9]{2}$");
  return std::regex_match(t, pattern);
}
bool valid_travelTimes(int g) { return g <= 10000; }
bool valid_stopoverTimes(int g) { return g <= 10000; }
bool valid_saleDate(const std::string &d) {
  std::regex pattern("^[0-9]{2}-[0-9]{2}$");
  return std::regex_match(d, pattern);
}
bool valid_type(const std::string &t) { return t >= "A" && t <= "Z"; }
Time saleDate[2];
char type[2] = {};

Train *add_train(const std::string &i, int n, int m, const std::string &s,
                 const std::string p, const std::string &x,
                 const std::string &t, const std::string &o,
                 const std::string &d, const std::string &y) {
  if (!valid_trainID(i) || !valid_stationNum(n) || !valid_seatNum(m) ||
      !valid_startTime(x) || !valid_type(t))
    return nullptr;
  Train *train = new Train();
  strcpy(train->trainID, i.c_str());
  train->stationNum = n;
  train->seatTotal = m;
  std::istringstream iss(s);
  std::string token;
  int pos = 0;
  while (std::getline(iss, token, '|')) {
    if (!valid_stations(token)) {
      delete train;
      return nullptr;
    }
    strcpy(train->stations[pos++], token.c_str());
  }
  std::istringstream isss(p);
  pos = 0;
  while (std::getline(isss, token, '|')) {
    if (!valid_prices(std::stoi(token))) {
      delete train;
      return nullptr;
    }
    train->prices[pos++] = std::stoi(token);
  }
  train->startTime.hour = std::stoi(x.substr(0, 2));
  train->startTime.minute = std::stoi(x.substr(3, 2));
  std::istringstream isssss(o);
  pos = 0;
  while (std::getline(isssss, token, '|')) {
    if (!valid_stopoverTimes(std::stoi(token))) {
      delete train;
      return nullptr;
    }
    train->stopoverTimes[pos++] = std::stoi(token);
  }
  train->saleDate[0].month = std::stoi(d.substr(0, 2));
  train->saleDate[0].day = std::stoi(d.substr(3, 2));
  train->saleDate[1].month = std::stoi(d.substr(6, 2));
  train->saleDate[1].day = std::stoi(d.substr(9, 2));
  strcpy(train->type, t.c_str());
  return train;
}
void query_trains(Train *train, Time *date) {
  std::cout << train->trainID << ' ' << train->type << '\n';
  Time time = train->startTime + *date;
  int price = 0;
  for (int i = 0; i < train->stationNum; i++) {
    std::cout << train->stations[i] << ' ';
    if (i == 0) {
      std::cout << "xx-xx xx:xx ";
    } else {
      time += train->stopoverTimes[i - 1];
      std::cout << time << ' ';
    }
    std::cout << "-> ";
    time += train->travelTimes[i];
    if (i == train->stationNum - 1) {
      std::cout << "xx-xx xx:xx ";
    } else {
      std::cout << time << ' ';
    }
    std::cout << price << ' ';
    price += train->prices[i];
    if (i == train->stationNum - 1) {
      std::cout << 'x';
    } else {
      std::cout << train->seatNum[i];
    }
    std::cout << '\n';
  }
}
Order *buy_ticket(Train &train, const std::string &u, const std::string &i,
                  const std::string &d, const std::string &f,
                  const std::string &t, int n, bool p) {
  if (train.release == false) {
    return nullptr;
  }
  int pos = 0;
  for (int i = 0; i < 100; i++) {
    if (strcmp(train.stations[i], f.c_str()) == 0) {
      pos = i;
      break;
    }
  }
  bool flag = false;
  // 31天修正未进行
  int date = (std::stoi(d.substr(0, 2)) - train.startTime.month) * 30 +
             std::stoi(d.substr(3, 2)) - train.startTime.day;
  for (int i = pos; strcmp(train.stations[i], t.c_str()) != 0; i++) {
    if (train.seatNum[date][i] + n > train.seatTotal) {
      flag = true;
      break;
    }
  }
  if (flag and !p)
    return nullptr;
  int price = 0;
  for (int i = pos; strcmp(train.stations[i], t.c_str()) != 0; i++) {
    price += train.prices[i];
    train.seatNum[date][i] += n;
  }
  Order *order = new Order();
  strcpy(order->username, u.c_str());
  strcpy(order->trainID, i.c_str());
  order->startTime = train.startTime;
  strcpy(order->from, f.c_str());
  strcpy(order->to, t.c_str());
  order->arriveTime = train.startTime;
  for (int i = pos; strcmp(train.stations[i], t.c_str()) != 0; i++) {
    order->arriveTime += train.travelTimes[i];
  }
  order->price = price;
  order->num = n;
  if (!flag)
    order->status = 1;
  else
    order->status = 2;
  return order;
}
bool timecmp(const DemoTrain &a, const DemoTrain &b) {
  Time t1 = a.startTime + b.endTime;
  Time t2 = a.endTime + b.startTime;
  if (t1 < t2) {
    return true;
  }
  if (t1 > t2) {
    return false;
  }
  return a.prices < b.prices;
}
bool costcmp(const DemoTrain &a, const DemoTrain &b) {
  if (a.prices < b.prices) {
    return true;
  }
  if (a.prices > b.prices) {
    return false;
  }
  Time t1 = a.startTime + a.endTime;
  Time t2 = b.startTime + b.endTime;
  if (t1 < t2) {
    return true;
  }
  if (t1 > t2) {
    return false;
  }
  return a.num > b.num;
}
bool timecmp2(const DemoTrain2 &a, const DemoTrain2 &b) {
  Time t1 = a.startTime + b.endTime2;
  Time t2 = a.endTime + b.startTime2;
  if (t1 < t2) {
    return true;
  }
  if (t1 > t2) {
    return false;
  }
  return a.prices < b.prices;
}
bool costcmp2(const DemoTrain2 &a, const DemoTrain2 &b) {
  if (a.prices < b.prices) {
    return true;
  }
  if (a.prices > b.prices) {
    return false;
  }
  Time t1 = a.startTime + a.endTime2;
  Time t2 = b.startTime + b.endTime2;
  if (t1 < t2) {
    return true;
  }
  if (t1 > t2) {
    return false;
  }
  return a.num > b.num;
}