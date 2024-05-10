#include "file_storage.hpp"
#include "time.hpp"
#include "train.hpp"
#include "user.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
char max_password[31];
int main() {
  memset(max_password, 127, sizeof(max_password));
  BPT<User> *user_table = new BPT<User>("user");
  BPT<Train> *train_table = new BPT<Train>("train");
  BPT<DateLocation_Train> *date_location_train_table =
      new BPT<DateLocation_Train>("date_location_train");
  sjtu::map<std::string, User> user_pool;
  std::string i, s, p, x, t, o, d, y, in;
  fstream fin("root", ios::in | ios::out);
  int n, m;
  while (true) {
    string command;
    cin >> command;
    if (command == "exit") {
      cout << "bye" << '\n';
      delete user_table;
      delete train_table;
      delete date_location_train_table;
      break;
    } else if (command == "clear") {
      delete user_table;
      delete train_table;
      delete date_location_train_table;
      std::ofstream fout("BPTuser", std::ios::trunc);
      fout.close();
      fout.open("BPTtrain", std::ios::trunc);
      fout.close();
      fout.open("BPTdate_location_train", std::ios::trunc);
      fout.close();
      fout.open("binuser", std::ios::trunc);
      fout.close();
      fout.open("bintrain", std::ios::trunc);
      fout.close();
      fout.open("bindate_location_train", std::ios::trunc);
      fout.close();
      fin.close();
      fout.open("root", std::ios::trunc);
      fout.close();
      fstream fin("root", ios::in | ios::out);
      user_table = new BPT<User>("user");
      train_table = new BPT<Train>("train");
      date_location_train_table =
          new BPT<DateLocation_Train>("date_location_train");
      user_pool.clear();
    } else if (command == "add_user") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string c, u, p, n, m;
      int cnt = 0, cnt2 = 0;
      int g;
      while (getline(iss, in, ' ')) {
        if (in == "-c") {
          cnt++;
          getline(iss, c, ' ');
        } else if (in == "-u") {
          cnt2++;
          getline(iss, u, ' ');
        } else if (in == "-p") {
          cnt2++;
          getline(iss, p, ' ');
        } else if (in == "-n") {
          cnt2++;
          getline(iss, n, ' ');
        } else if (in == "-m") {
          cnt2++;
          getline(iss, m, ' ');
        } else if (in == "-g") {
          cnt++;
          getline(iss, i, ' ');
          g = stoi(i);
        }
      }
      if (fin.seekg(0, ios::end).tellg() == 0) {
        if (cnt2 != 4) {
          std::cout << "-1\n";
          continue;
        }
        g = 10;
      } else {
        if (user_pool.find(c) == user_pool.end()) {
          std::cout << "-1\n";
          continue;
        }
        if (user_pool[c].privilege < g) {
          std::cout << "-1\n";
          continue;
        }
        if (cnt != 6) {
          std::cout << "-1\n";
          continue;
        }
        User user_min, user_max;
        strcpy(user_min.username, u.c_str());
        strcpy(user_max.username, u.c_str());
        strcpy(user_max.password, max_password);
        if (!user_table->find(user_min, user_max).empty()) {
          std::cout << "-1\n";
          continue;
        }
      }
      User *new_user = add_user(u, p, n, m, g);
      if (new_user == nullptr) {
        std::cout << "-1\n";
        continue;
      }
      user_table->insert(*new_user);
      fin.write(reinterpret_cast<char *>(new_user), sizeof(User));
      std::cout << "0\n";
      delete new_user;
    } else if (command == "login") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string u, p;
      int cnt = 0;
      while (getline(iss, in, ' ')) {
        if (in == "-u") {
          cnt++;
          getline(iss, u, ' ');
        } else if (in == "-p") {
          cnt++;
          getline(iss, p, ' ');
        }
      }
      if (cnt != 2) {
        std::cout << "-1\n";
        continue;
      }
      User *user = new User();
      strcpy(user->username, u.c_str());
      strcpy(user->password, p.c_str());
      sjtu::vector<User> res = user_table->find(*user, *user);
      if (res.empty()) {
        std::cout << "-1\n";
        continue;
      }
      user_pool[u] = res[0];
      std::cout << "0\n";
    } else if (command == "logout") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string u;
      int cnt = 0;
      while (getline(iss, in, ' ')) {
        if (in == "-u") {
          cnt++;
          getline(iss, u, ' ');
        }
      }
      if (cnt != 1) {
        std::cout << "-1\n";
        continue;
      }
      if (user_pool.find(u) == user_pool.end()) {
        std::cout << "-1\n";
        continue;
      }
      user_pool.erase(user_pool.find(u));
      std::cout << "0\n";
    } else if (command == "query_profile") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string u, c;
      int cnt = 0;
      while (getline(iss, in, ' ')) {
        if (in == "-u") {
          cnt++;
          getline(iss, u, ' ');
        } else if (in == "-c") {
          cnt++;
          getline(iss, c, ' ');
        }
      }
      if (cnt != 2) {
        std::cout << "-1\n";
        continue;
      }
      if (user_pool.find(c) == user_pool.end()) {
        std::cout << "-1\n";
        continue;
      }
      User query_user, query_user_max;
      strcpy(query_user.username, u.c_str());
      strcpy(query_user_max.username, u.c_str());
      strcpy(query_user_max.password, max_password);
      sjtu::vector<User> res = user_table->find(query_user, query_user_max);
      if (res.empty() or
          (res[0].privilege >= user_pool[c].privilege and u != c)) {
        std::cout << "-1\n";
        continue;
      }
      assert(res.size() == 1);
      query_profile(&res[0]);
    } else if (command == "modify_profile") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string u, p, n, m, c;
      int g = -1;
      while (getline(iss, in, ' ')) {
        if (in == "-u") {
          getline(iss, u, ' ');
        } else if (in == "-p") {
          getline(iss, p, ' ');
        } else if (in == "-n") {
          getline(iss, n, ' ');
        } else if (in == "-m") {
          getline(iss, m, ' ');
        } else if (in == "-c") {
          getline(iss, c, ' ');
        } else if (in == "-g") {
          getline(iss, i, ' ');
          g = stoi(i);
        }
      }
      if (user_pool.find(c) == user_pool.end()) {
        std::cout << "-1\n";
        continue;
      }
      if (user_pool[c].privilege < g) {
        std::cout << "-1\n";
        continue;
      }
      User query_user, query_user_max;
      strcpy(query_user.username, u.c_str());
      strcpy(query_user_max.username, u.c_str());
      strcpy(query_user_max.password, max_password);
      sjtu::vector<User> res = user_table->find(query_user, query_user_max);
      if (res.empty() or
          (res[0].privilege >= user_pool[c].privilege and u != c)) {
        std::cout << "-1\n";
        continue;
      }
      assert(res.size() == 1);
      user_table->erase(res[0]);
      if (!modify_profile(&res[0], p, n, m, g)) {
        std::cout << "-1\n";
        continue;
      }
      user_table->insert(res[0]);
      if (user_pool.find(u) != user_pool.end()) {
        user_pool[u] = res[0];
      }
    } else if (command == "add_train") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string c;
      int cnt = 0;
      while (getline(iss, in, ' ')) {
        if (in == "-i") {
          cnt++;
          getline(iss, c, ' ');
        } else if (in == "-n") {
          cnt++;
          getline(iss, i, ' ');
          n = stoi(i);
        } else if (in == "-m") {
          cnt++;
          getline(iss, i, ' ');
          m = stoi(i);
        } else if (in == "-s") {
          cnt++;
          getline(iss, s, ' ');
        } else if (in == "-p") {
          cnt++;
          getline(iss, p, ' ');
        } else if (in == "-x") {
          cnt++;
          getline(iss, x, ' ');
        } else if (in == "-t") {
          cnt++;
          getline(iss, t, ' ');
        } else if (in == "-o") {
          cnt++;
          getline(iss, o, ' ');
        } else if (in == "-d") {
          cnt++;
          getline(iss, d, ' ');
        } else if (in == "-y") {
          cnt++;
          getline(iss, y, ' ');
        }
      }
      i = c;
      if (cnt != 10) {
        std::cout << "-1\n";
        continue;
      }
      Train *new_train = add_train(i, n, m, s, p, x, t, o, d, y);
      if (new_train == nullptr) {
        std::cout << "-1\n";
        continue;
      }
      Train train_min, train_max;
      strcpy(train_min.trainID, i.c_str());
      strcpy(train_max.trainID, i.c_str());
      strcpy(train_max.type, max_password);
      if (!train_table->find(train_min, train_max).empty()) {
        std::cout << "-1\n";
        continue;
      }
      train_table->insert(*new_train);
      cout << "0\n";
      delete new_train;
    } else if (command == "relese_train") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string i, c;
      int cnt = 0;
      while (getline(iss, in, ' ')) {
        if (in == "-i") {
          cnt++;
          getline(iss, i, ' ');
        }
      }
      if (cnt != 1) {
        std::cout << "-1\n";
        continue;
      }
      Train query_train, query_train_max;
      strcpy(query_train.trainID, i.c_str());
      strcpy(query_train_max.trainID, i.c_str());
      strcpy(query_train_max.type, max_password);
      sjtu::vector<Train> res = train_table->find(query_train, query_train_max);
      if (res.empty()) {
        std::cout << "-1\n";
        continue;
      }
      assert(res.size() == 1);
      train_table->erase(res[0]);
      if (res[0].release) {
        std::cout << "-1\n";
        continue;
      }
      res[0].release = true;
      train_table->insert(res[0]);
      for (Time t = res[0].saleDate[0]; t <= res[0].saleDate[1]; ++t) {
        Time t2 = t + res[0].startTime;
        for (int i = 0; i < res[0].stationNum; i++) {
          DateLocation_Train dlt;
          dlt.date = t2;
          strcpy(dlt.to, res[0].stations[i]);
          strcpy(dlt.trainID, res[0].trainID);
          date_location_train_table->insert(dlt);
          t2 += res[0].travelTimes[i];
          if (i > 0)
            t2 += res[0].stopoverTimes[i - 1];
        }
      }
    } else if (command == "query_train") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string i, c;
      int cnt = 0;
      while (getline(iss, in, ' ')) {
        if (in == "-i") {
          cnt++;
          getline(iss, i, ' ');
        }
        if (in == "-d") {
          cnt++;
          getline(iss, c, ' ');
        }
      }
      if (cnt != 2) {
        std::cout << "-1\n";
        continue;
      }
      Time t;
      t.month = stoi(c.substr(0, 2));
      t.day = stoi(c.substr(3, 2));
      Train query_train, query_train_max;
      strcpy(query_train.trainID, i.c_str());
      strcpy(query_train_max.trainID, i.c_str());
      strcpy(query_train_max.type, max_password);
      sjtu::vector<Train> res = train_table->find(query_train, query_train_max);
      if (res.empty()) {
        std::cout << "-1\n";
        continue;
      }
      assert(res.size() == 1);
      if (t < res[0].saleDate[0] or res[0].saleDate[1] < t) {
        std::cout << "-1\n";
        continue;
      }
      query_trains(&res[0], &t);
    } else if (command == "delete_train") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string i;
      int cnt = 0;
      while (getline(iss, in, ' ')) {
        if (in == "-i") {
          cnt++;
          getline(iss, i, ' ');
        }
      }
      if (cnt != 1) {
        std::cout << "-1\n";
        continue;
      }
      Train query_train, query_train_max;
      strcpy(query_train.trainID, i.c_str());
      strcpy(query_train_max.trainID, i.c_str());
      strcpy(query_train_max.type, max_password);
      sjtu::vector<Train> res = train_table->find(query_train, query_train_max);
      if (res.empty()) {
        std::cout << "-1\n";
        continue;
      }
      assert(res.size() == 1);
      if (res[0].release) {
        std::cout << "-1\n";
        continue;
      }
      train_table->erase(res[0]);
      std::cout << "0\n";
      continue;
    } else if (command == "query_ticket") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string s, t, d, p;
      int cnt = 0;
      while (getline(iss, in, ' ')) {
        if (in == "-s") {
          cnt++;
          getline(iss, s, ' ');
        }
        if (in == "-t") {
          cnt++;
          getline(iss, t, ' ');
        }
        if (in == "-d") {
          cnt++;
          getline(iss, d, ' ');
        }
        if (in == "-p") {
          cnt++;
          getline(iss, p, ' ');
        }
      }
      // 未检查含有P但是cnt为3
      if (cnt != 3 and cnt != 4) {
        std::cout << "-1\n";
        continue;
      }
      DateLocation_Train query_dlt, query_dlt_max;
      Time time;
      time.month = stoi(d.substr(0, 2));
      time.day = stoi(d.substr(3, 2));
      strcpy(query_dlt.to, t.c_str());
      query_dlt.date = time;
      strcpy(query_dlt_max.to, t.c_str());
      query_dlt_max.date = time;
      sjtu::vector<DateLocation_Train> res =
          date_location_train_table->find(query_dlt, query_dlt_max);
      sjtu::vector<DemoTrain> res3;

      for (int i = 0; i < res.size(); i++) {
        Train query_train, query_train_max;
        strcpy(query_train.trainID, res[i].trainID);
        strcpy(query_train_max.trainID, res[i].trainID);
        strcpy(query_train_max.type, max_password);
        sjtu::vector<Train> res2 =
            train_table->find(query_train, query_train_max);
        if (res2.empty()) {
          continue;
        }
        assert(res2.size() == 1);
        if (res2[0].release) {
          continue;
        }
        int price = 0;
        int index = time - res2[0].saleDate[0];
        int begin = -1;
        Time time2;
        for (int j = 0; j < res2[0].stationNum; j++) {
          if (strcmp(res2[0].stations[j], s.c_str()) == 0) {
            begin = j;
            break;
          }
          time2 += res2[0].travelTimes[j];
          if (j > 0) {
            time2 += res2[0].stopoverTimes[j - 1];
          }
        }
        int valseatNum = res2[0].seatTotal - res2[0].seatNum[index][begin];
        assert(begin != -1);
        for (int j = begin; j < res2[0].stationNum; j++) {
          if (strcmp(res2[0].stations[j], t.c_str()) == 0) {
            DemoTrain dt;
            strcpy(dt.trainID, res2[0].trainID);
            dt.startTime = res2[0].startTime + time;
            dt.endTime = dt.startTime + time2;
            dt.num = valseatNum;
            dt.prices = price;
            res3.push_back(dt);
            break;
          }
          valseatNum =
              min(valseatNum, res2[0].seatTotal - res2[0].seatNum[index][j]);
          price += res2[0].prices[j];
          time2 += res2[0].travelTimes[j];
          if (j > 0) {
            time2 += res2[0].stopoverTimes[j - 1];
          }
        }
      }
      cout << res3.size() << '\n';
      if (p == "time") {
        res3.sort(timecmp);
      } else if (p == "cost") {
        res3.sort(costcmp);
      }
      for (int i = 0; i < res3.size(); i++) {
        cout << res3[i].trainID << ' ' << s << ' ' << res3[0].startTime
             << " -> " << t << res3[0].endTime << ' ' << res3[0].prices << ' '
             << res3[0].num << '\n';
      }
    }
  }
}