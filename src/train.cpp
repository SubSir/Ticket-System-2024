#include <cstring>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
bool valid_trainID(const std::string &u) {
  if (u.length() == 0 || u.length() > 20 || u[0] > 'z' || u[0] < 'a')
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
struct Train {
  char trainID[21] = {};
  int stationNum = 0;
  char stations[101][31] = {};
  int seatNum = 0;
  int prices[101] = {};
  char startTime[6] = {};
  int travelTimes[101] = {};
  int stopoverTimes[101] = {};
  char saleDate[2][6] = {};
  char type[2] = {};
};
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
  train->seatNum = m;
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
  strcpy(train->startTime, x.c_str());
  std::istringstream issss(t);
  pos = 0;
  while (std::getline(issss, token, '|')) {
    if (!valid_travelTimes(std::stoi(token))) {
      delete train;
      return nullptr;
    }
    train->travelTimes[pos++] = std::stoi(token);
  }
  std::istringstream isssss(o);
  pos = 0;
  while (std::getline(isssss, token, '|')) {
    if (!valid_stopoverTimes(std::stoi(token))) {
      delete train;
      return nullptr;
    }
    train->stopoverTimes[pos++] = std::stoi(token);
  }
  std::istringstream issssss(d);
  pos = 0;
  while (std::getline(issssss, token, '|')) {
    if (!valid_saleDate(token)) {
      delete train;
      return nullptr;
    }
    strcpy(train->saleDate[pos++], token.c_str());
  }
  strcpy(train->type, t.c_str());
  return train;
}
void query_train(Train *train) {
  std::cout << train->trainID << ' ' << train->type << '\n';
  for (int i = 0; i < train->stationNum; i++) {
    std::cout << train->stations[i] << ' ' << train->;
  }
}