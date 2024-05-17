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
  return true;
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

bool add_train(const std::string &i, int n, int m, const std::string &s,
               const std::string p, const std::string &x, const std::string &t,
               const std::string &o, const std::string &d, const std::string &y,
               Train &train) {
  // if (!valid_trainID(i) || !valid_stationNum(n) || !valid_seatNum(m) ||
  //    !valid_startTime(x) || !valid_type(y))
  //  return false;
  strcpy(train.trainID, i.c_str());
  train.stationNum = n;
  train.seatTotal = m;
  std::istringstream iss(s);
  std::string token;
  int pos = 0;
  while (std::getline(iss, token, '|')) {
    // if (!valid_stations(token)) {
    //   return false;
    // }
    strcpy(train.stations[pos++], token.c_str());
  }
  std::istringstream isss(p);
  pos = 0;
  while (std::getline(isss, token, '|')) {
    //  if (!valid_prices(std::stoi(token))) {
    //   return false;
    // }
    train.prices[pos++] = std::stoi(token);
  }
  train.startTime.hour = std::stoi(x.substr(0, 2));
  train.startTime.minute = std::stoi(x.substr(3, 2));
  std::istringstream isssss(o);
  pos = 0;
  while (std::getline(isssss, token, '|')) {
    // if (!valid_stopoverTimes(std::stoi(token))) {
    //   return false;
    // }
    train.stopoverTimes[pos++] = std::stoi(token);
  }
  pos = 0;
  std::istringstream issssss(t);
  while (std::getline(issssss, token, '|')) {
    // if (!valid_travelTimes(std::stoi(token))) {
    //   return false;
    // }
    train.travelTimes[pos++] = std::stoi(token);
  }
  train.saleDate[0].month = std::stoi(d.substr(0, 2));
  train.saleDate[0].day = std::stoi(d.substr(3, 2));
  train.saleDate[1].month = std::stoi(d.substr(6, 2));
  train.saleDate[1].day = std::stoi(d.substr(9, 2));
  strcpy(train.type, y.c_str());
  return true;
}
void query_trains(Train *train, Time *date) {
  std::cout << train->trainID << ' ' << train->type << '\n';
  int index = *date - train->saleDate[0];
  Time time = train->startTime + *date;
  int price = 0;
  for (int i = 0; i < train->stationNum; i++) {
    std::cout << train->stations[i] << ' ';
    if (i == 0) {
      std::cout << "xx-xx xx:xx "
                << "-> " << time << ' ';
    } else {
      std::cout << time << ' ';
      std::cout << "-> ";
      if (i == train->stationNum - 1) {
        std::cout << "xx-xx xx:xx ";
      } else {
        time += train->stopoverTimes[i - 1];
        std::cout << time << ' ';
      }
    }
    std::cout << price << ' ';
    price += train->prices[i];
    if (i == train->stationNum - 1) {
      std::cout << 'x';
    } else {
      std::cout << train->seatTotal - train->seatNum[index][i];
    }
    time += train->travelTimes[i];
    std::cout << '\n';
  }
}
bool buy_ticket(Train &train, const std::string &u, const std::string &i,
                const std::string &d, const std::string &f,
                const std::string &t, int n, bool p, Order &order) {
  if (train.release == false or train.seatTotal < n) {
    return false;
  }
  int pos = -1;
  Time st = train.startTime + train.saleDate[0];
  for (int i = 0; i < 100; i++) {
    if (strcmp(train.stations[i], f.c_str()) == 0) {
      pos = i;
      break;
    }
    st += train.travelTimes[i];
    st += train.stopoverTimes[i];
  }
  if (pos == -1)
    return false;
  bool flag = false;
  Time ti;
  ti.month = std::stoi(d.substr(0, 2));
  ti.day = std::stoi(d.substr(3, 2));
  int date = ti - st;
  if (date < 0)
    return false;
  int date2 = train.saleDate[1] - train.saleDate[0];
  if (date > date2)
    return false;
  int j;
  for (j = pos;
       j < train.stationNum and strcmp(train.stations[j], t.c_str()) != 0; j++)
    ;
  if (j == train.stationNum)
    return false;
  for (j = pos;
       j < train.stationNum and strcmp(train.stations[j], t.c_str()) != 0;
       j++) {
    if (train.seatNum[date][j] + n > train.seatTotal) {
      flag = true;
      break;
    }
  }
  if (flag and !p)
    return false;
  int price = 0;
  for (int i = pos; strcmp(train.stations[i], t.c_str()) != 0; i++) {
    price += train.prices[i];
  }
  if (!flag)
    for (int i = pos; strcmp(train.stations[i], t.c_str()) != 0; i++) {
      train.seatNum[date][i] += n;
    }
  strcpy(order.username, u.c_str());
  strcpy(order.trainID, i.c_str());
  order.startTime = ti;
  order.startTime.minute = st.minute;
  order.startTime.hour = st.hour;
  strcpy(order.from, f.c_str());
  strcpy(order.to, t.c_str());
  order.arriveTime = order.startTime;
  for (int i = pos; strcmp(train.stations[i], t.c_str()) != 0; i++) {
    order.end_loc = i;
    order.arriveTime += train.travelTimes[i];
    if (i > pos)
      order.arriveTime += train.stopoverTimes[i - 1];
  }
  order.end_loc++;
  order.price = price * n;
  order.num = n;
  order.dat = date;
  order.loc = pos;
  if (!flag)
    order.status = 1;
  else
    order.status = 2;
  return true;
}
bool buy_ticket(Train &train, int dat, int loc, int end_loc, int n) {
  for (int i = loc; i < end_loc; i++) {
    if (train.seatNum[dat][i] + n > train.seatTotal) {
      return false;
    }
  }
  for (int i = loc; i < end_loc; i++) {
    train.seatNum[dat][i] += n;
  }
  return true;
}
bool timecmp(const DemoTrain &a, const DemoTrain &b) {
  int t1 = a.startTime.sum() + b.endTime.sum();
  int t2 = a.endTime.sum() + b.startTime.sum();
  if (t1 > t2) {
    return true;
  }
  if (t1 < t2) {
    return false;
  }
  return strcmp(a.trainID, b.trainID) < 0;
}
bool costcmp(const DemoTrain &a, const DemoTrain &b) {
  if (a.prices < b.prices) {
    return true;
  }
  if (a.prices > b.prices) {
    return false;
  }
  return strcmp(a.trainID, b.trainID) < 0;
}
bool timecmp2(const DemoTrain2 &a, const DemoTrain2 &b) {
  int t1 = a.startTime.sum() + b.endTime2.sum();
  int t2 = a.endTime2.sum() + b.startTime.sum();
  if (t1 > t2) {
    return true;
  }
  if (t1 < t2) {
    return false;
  }
  if (a.prices != b.prices) {
    return a.prices < b.prices;
  }
  int cmp1 = strcmp(a.trainID, b.trainID);
  if (cmp1 != 0) {
    return cmp1 < 0;
  }
  int cmp2 = strcmp(a.trainID2, b.trainID2);
  if (cmp2 != 0) {
    return cmp2 < 0;
  }
  return false;
}
bool costcmp2(const DemoTrain2 &a, const DemoTrain2 &b) {
  if (a.prices < b.prices) {
    return true;
  }
  if (a.prices > b.prices) {
    return false;
  }
  int t1 = a.startTime.sum() + b.endTime2.sum();
  int t2 = a.endTime2.sum() + b.startTime.sum();
  if (t1 > t2) {
    return true;
  }
  if (t1 < t2) {
    return false;
  }
  int cmp1 = strcmp(a.trainID, b.trainID);
  if (cmp1 != 0) {
    return cmp1 < 0;
  }
  int cmp2 = strcmp(a.trainID2, b.trainID2);
  if (cmp2 != 0) {
    return cmp2 < 0;
  }
  return false;
}
bool waitingcmp(const DemoOrder2 &a, const DemoOrder2 &b) {
  return a.pos < b.pos;
}