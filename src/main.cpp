#include "file_storage.hpp"
#include "time.hpp"
#include "train.hpp"
#include "user.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
char max_password[31];
char max_trainid[21];
Time min_time, max_time;
int max_ind;
BPT<Index_user> user_table("_user");
BPT<Index_train> train_table("_train");
BPT<DateLocation_Train> date_location_train_table("_date_location_train");
BPT<DateLocation_Train> date_location_train_table2("_date_location_train2");
BPT<DemoOrder> order_table("_order");
BPT<DemoOrder2> waiting_list("_waiting_list");
sjtu::map<std::string, Index_user> user_pool;
MemoryRiver<Order> order_river("order_");
MemoryRiver<Train> train_river("train_");
MemoryRiver<User> user_river("user_");
sjtu::vector<DemoTrain> _query_train(string &s, Time &time, string &t,
                                     bool transfer) {
  DateLocation_Train query_dlt_min, query_dlt_max;
  strcpy(query_dlt_min.to, s.c_str());
  query_dlt_min.date = min_time;
  strcpy(query_dlt_max.to, s.c_str());
  if (!transfer)
    query_dlt_max.date = time;
  else
    query_dlt_max.date = max_time;
  sjtu::vector<DateLocation_Train> res =
      date_location_train_table.find(query_dlt_min, query_dlt_max);
  strcpy(query_dlt_min.to, t.c_str());
  strcpy(query_dlt_max.to, t.c_str());
  query_dlt_min.date = time - 24 * 60;
  query_dlt_max.date = max_time;
  sjtu::vector<DateLocation_Train> res2 =
      date_location_train_table2.find(query_dlt_min, query_dlt_max);
  sjtu::map<string, bool> mp;
  for (int i = 0; i < res2.size(); i++) {
    string s = res2[i].trainID;
    mp[s] = true;
  }
  for (int i = 0; i < res.size(); i++) {
    string s = res[i].trainID;
    if (mp.find(s) == mp.end()) {
      res.erase(i);
      i--;
    }
  }
  sjtu::vector<DemoTrain> res3;
  for (int i = 0; i < res.size(); i++) {
    Index_train query_train, query_train_max;
    strcpy(query_train.trainID, res[i].trainID);
    strcpy(query_train_max.trainID, res[i].trainID);
    query_train_max.pos = max_ind;
    sjtu::vector<Index_train> res2 =
        train_table.find(query_train, query_train_max);
    if (res2.empty() or !res2[0].release) {
      continue;
    }
    Train train;
    train_river.read(train, res2[0].pos);
    int price = 0;
    Time time2 = train.saleDate[0] + train.startTime;
    int begin = -1;
    for (int j = 0; j < train.stationNum; j++) {
      if (strcmp(train.stations[j], s.c_str()) == 0) {
        begin = j;
        break;
      }
      time2 += train.travelTimes[j];
      time2 += train.stopoverTimes[j];
    }
    Time tmp = time;
    tmp.hour = time2.hour;
    tmp.minute = time2.minute;
    while (transfer and time > tmp) {
      ++tmp;
    }
    while (transfer and time2 > tmp) {
      ++tmp;
    }
    int index = tmp - time2;
    if (index < 0 or index > train.saleDate[1] - train.saleDate[0]) {
      continue;
    }
    int valseatNum = train.seatTotal - train.seatNum[index][begin];
    Time time3;
    for (int j = begin; j < train.stationNum; j++) {
      if (strcmp(train.stations[j], t.c_str()) == 0) {
        DemoTrain dt;
        strcpy(dt.trainID, train.trainID);
        dt.startTime = tmp;
        dt.endTime = dt.startTime + time3;
        dt.num = valseatNum;
        dt.prices = price;
        res3.push_back(dt);
        break;
      }
      valseatNum = min(valseatNum, train.seatTotal - train.seatNum[index][j]);
      price += train.prices[j];
      time3 += train.travelTimes[j];
      if (j > begin) {
        time3 += train.stopoverTimes[j - 1];
      }
    }
  }
  return res3;
}
sjtu::vector<DemoTrain2> _query_transfer(string &s, Time &time, string &t) {
  DateLocation_Train query_dlt_min, query_dlt_max;
  strcpy(query_dlt_min.to, s.c_str());
  query_dlt_min.date = min_time;
  strcpy(query_dlt_max.to, s.c_str());
  query_dlt_max.date = time;
  sjtu::vector<DateLocation_Train> res =
      date_location_train_table.find(query_dlt_min, query_dlt_max);
  sjtu::vector<DemoTrain2> res3;
  for (int i = 0; i < res.size(); i++) {
    Index_train query_train, query_train_max;
    strcpy(query_train.trainID, res[i].trainID);
    strcpy(query_train_max.trainID, res[i].trainID);
    query_train_max.pos = max_ind;
    sjtu::vector<Index_train> res2 =
        train_table.find(query_train, query_train_max);
    if (res2.empty() or !res2[0].release) {
      continue;
    }
    Train train;
    train_river.read(train, res2[0].pos);
    int price = 0;
    int begin = -1;
    Time time2 = train.saleDate[0] + train.startTime;
    for (int j = 0; j < train.stationNum; j++) {
      if (strcmp(train.stations[j], s.c_str()) == 0) {
        begin = j;
        break;
      }
      time2 += train.travelTimes[j];
      time2 += train.stopoverTimes[j];
    }
    int index = time - time2;
    if (index > train.saleDate[1] - train.saleDate[0]) {
      continue;
    }
    int valseatNum = train.seatTotal - train.seatNum[index][begin];
    // assert(begin != -1);
    Time time3 = time;
    time3.hour = time2.hour;
    time3.minute = time2.minute;
    Time tmp = time3;
    Time time4;
    for (int j = begin; j < train.stationNum; j++) {
      if (j != begin and strcmp(train.stations[j], t.c_str()) != 0) {
        string s2 = train.stations[j];
        sjtu::vector<DemoTrain> dt;
        dt = _query_train(s2, time3, t, true);
        for (int k = 0; k < dt.size(); k++) {
          if (strcmp(dt[k].trainID, train.trainID) == 0) {
            dt.erase(k);
            k--;
          }
        }
        for (int k = 0; k < dt.size(); k++) {
          DemoTrain2 dt2;
          strcpy(dt2.trainID, train.trainID);
          strcpy(dt2.trainID2, dt[k].trainID);
          dt2.startTime = time;
          dt2.startTime.hour = tmp.hour;
          dt2.startTime.minute = tmp.minute;
          dt2.endTime = dt2.startTime + time4;
          dt2.startTime2 = dt[k].startTime;
          dt2.endTime2 = dt[k].endTime;
          dt2.num = min(valseatNum, dt[k].num);
          dt2.prices = price + dt[k].prices;
          dt2.price1 = price;
          dt2.price2 = dt[k].prices;
          dt2.num1 = valseatNum;
          dt2.num2 = dt[k].num;
          strcpy(dt2.transfer_locate, train.stations[j]);
          res3.push_back(dt2);
        }
      }
      valseatNum = min(valseatNum, train.seatTotal - train.seatNum[index][j]);
      price += train.prices[j];
      time3 += train.travelTimes[j];
      time4 += train.travelTimes[j];
      if (j > begin) {
        time3 += train.stopoverTimes[j - 1];
        time4 += train.stopoverTimes[j - 1];
      }
    }
  }
  return res3;
}
int Time::month_day[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int Time::month_days[13] = {0,   0,   31,  60,  91,  121, 152,
                            182, 213, 244, 274, 305, 335};
int main() {
  // 虐死萌萌哒评测机~
  cin.tie(0);
  cout.tie(0);
  ios::sync_with_stdio(false);
  //  freopen("../testcases/basic_1/1.in", "r", stdin);
  //  freopen("../self.out", "w", stdout);
  memset(max_password, 127, sizeof(max_password));
  max_password[30] = 0;
  memset(max_trainid, 127, sizeof(max_trainid));
  max_trainid[20] = 0;
  min_time.month = 6;
  min_time.day = 1;
  max_time.month = 9;
  max_time.day = 4;
  max_time.hour = 23;
  max_time.minute = 60;
  max_ind = 0x7fffffff;
  std::string in;
  int n, m;
  string index, command;
  int ind = -1;
  cin >> index;
  while (true) {
    cin >> command;
    cout << index << " ";
    if (command[0] == 'e') { // exit
      cout << "bye" << '\n';
      return 0;
    } else if (command[0] == 'c') { // clear
      user_table.clear();
      train_table.clear();
      date_location_train_table.clear();
      order_table.clear();
      waiting_list.clear();
      order_river.clear();
      user_pool.clear();
    } else if (command[0] == 'a' and command[4] == 'u') { // add_user
      std::string c, u, p, n, m;
      int g;
      do {
        cin >> in;
        if (in == "-c") {
          cin >> c;
        } else if (in == "-u") {
          cin >> u;
        } else if (in == "-p") {
          cin >> p;
        } else if (in == "-n") {
          cin >> n;
        } else if (in == "-m") {
          cin >> m;
        } else if (in == "-g") {
          cin >> g;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      int o = -1;
      order_river.get_info(o, 1);
      if (o == 0) {
        g = 10;
      } else {
        if (user_pool.find(c) == user_pool.end()) {
          std::cout << "-1\n";
          continue;
        }
        if (user_pool[c].privilege <= g) {
          std::cout << "-1\n";
          continue;
        }
        Index_user user_min, user_max;
        strcpy(user_min.username, u.c_str());
        strcpy(user_max.username, u.c_str());
        user_max.pos = max_ind;
        if (!user_table.find(user_min, user_max).empty()) {
          std::cout << "-1\n";
          continue;
        }
      }
      User new_user;
      bool bl = add_user(u, p, n, m, g, new_user);
      if (!bl) {
        std::cout << "-1\n";
        continue;
      }
      Index_user index_user;
      strcpy(index_user.username, u.c_str());
      index_user.privilege = g;
      index_user.pos = user_river.write(new_user);
      user_table.insert(index_user);
      o++;
      order_river.write_info(o, 1);
      std::cout << "0\n";
    } else if (command[0] == 'l' and command[3] == 'i') { // login
      std::string u, p;
      do {
        cin >> in;
        if (in == "-u") {
          cin >> u;
        } else if (in == "-p") {
          cin >> p;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      if (user_pool.find(u) != user_pool.end()) {
        std::cout << "-1\n";
        continue;
      }
      Index_user user_min, user_max;
      strcpy(user_min.username, u.c_str());
      strcpy(user_max.username, u.c_str());
      user_max.pos = max_ind;
      sjtu::vector<Index_user> res = user_table.find(user_min, user_max);
      if (res.empty()) {
        std::cout << "-1\n";
        continue;
      }
      User user;
      user_river.read(user, res[0].pos);
      if (strcmp(user.password, p.c_str()) != 0) {
        std::cout << "-1\n";
        continue;
      }
      user_pool[u] = res[0];
      std::cout << "0\n";
    } else if (command[0] == 'l' and command[3] == 'o') { // logout
      std::string u;
      do {
        cin >> in;
        if (in == "-u") {
          cin >> u;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      if (user_pool.find(u) == user_pool.end()) {
        std::cout << "-1\n";
        continue;
      }
      user_pool.erase(user_pool.find(u));
      std::cout << "0\n";
    } else if (command[0] == 'q' and command[6] == 'p') { // query_profile
      std::string u, c;
      do {
        cin >> in;
        if (in == "-u") {
          cin >> u;
        } else if (in == "-c") {
          cin >> c;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      if (user_pool.find(c) == user_pool.end()) {
        std::cout << "-1\n";
        continue;
      }
      int index = -1;
      if (user_pool.find(u) == user_pool.end()) {
        Index_user query_user, query_user_max;
        strcpy(query_user.username, u.c_str());
        strcpy(query_user_max.username, u.c_str());
        query_user_max.pos = max_ind;
        sjtu::vector<Index_user> res =
            user_table.find(query_user, query_user_max);
        if (res.empty() or
            (res[0].privilege >= user_pool[c].privilege and u != c)) {
          std::cout << "-1\n";
          continue;
        }
        index = res[0].pos;
      } else {
        index = user_pool[u].pos;
        if (user_pool[u].privilege >= user_pool[c].privilege and u != c) {
          std::cout << "-1\n";
          continue;
        }
      }
      User user;
      user_river.read(user, index);
      query_profile(&user);
    } else if (command[0] == 'm') { // modify_profile
      std::string u, p, n, m, c;
      int g = -1;
      do {
        cin >> in;
        if (in == "-u") {
          cin >> u;
        } else if (in == "-p") {
          cin >> p;
        } else if (in == "-n") {
          cin >> n;
        } else if (in == "-m") {
          cin >> m;
        } else if (in == "-g") {
          cin >> g;
        } else if (in == "-c") {
          cin >> c;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      if (user_pool.find(c) == user_pool.end()) {
        std::cout << "-1\n";
        continue;
      }
      if (user_pool[c].privilege <= g) {
        std::cout << "-1\n";
        continue;
      }
      Index_user query_user, query_user_max;
      strcpy(query_user.username, u.c_str());
      strcpy(query_user_max.username, u.c_str());
      query_user_max.pos = max_ind;
      sjtu::vector<Index_user> res =
          user_table.find(query_user, query_user_max);
      if (res.empty() or
          (res[0].privilege >= user_pool[c].privilege and u != c)) {
        std::cout << "-1\n";
        continue;
      }
      User user;
      user_river.read(user, res[0].pos);
      if (!modify_profile(&user, p, n, m, g)) {
        std::cout << "-1\n";
        continue;
      }
      res[0].privilege = user.privilege;
      if (user_pool.find(u) != user_pool.end()) {
        user_pool[u] = res[0];
      }
      user_river.update(user, res[0].pos);
    } else if (command[0] == 'a' and command[4] == 't') { // add_train
      std::string i, p, x, t, o, d, y, s;
      int n, m;
      do {
        cin >> in;
        if (in == "-i") {
          cin >> i;
        } else if (in == "-n") {
          cin >> n;
        } else if (in == "-m") {
          cin >> m;
        } else if (in == "-s") {
          cin >> s;
        } else if (in == "-p") {
          cin >> p;
        } else if (in == "-x") {
          cin >> x;
        } else if (in == "-t") {
          cin >> t;
        } else if (in == "-o") {
          cin >> o;
        } else if (in == "-d") {
          cin >> d;
        } else if (in == "-y") {
          cin >> y;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      Train new_train;
      bool bl = add_train(i, n, m, s, p, x, t, o, d, y, new_train);
      if (!bl) {
        std::cout << "-1\n";
        continue;
      }
      Index_train train_min, train_max;
      strcpy(train_min.trainID, i.c_str());
      strcpy(train_max.trainID, i.c_str());
      train_max.pos = max_ind;
      if (!train_table.find(train_min, train_max).empty()) {
        std::cout << "-1\n";
        continue;
      }
      int pos = train_river.write(new_train);
      Index_train index_train;
      strcpy(index_train.trainID, i.c_str());
      index_train.pos = pos;
      train_table.insert(index_train);
      cout << "0\n";
    } else if (command[0] == 'r' and command[2] == 'l') { // release_train
      std::string i;
      do {
        cin >> in;
        if (in == "-i") {
          cin >> i;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      Index_train query_train, query_train_max;
      strcpy(query_train.trainID, i.c_str());
      strcpy(query_train_max.trainID, i.c_str());
      query_train_max.pos = max_ind;
      sjtu::vector<Index_train> res =
          train_table.find(query_train, query_train_max);
      if (res.empty() or res[0].release) {
        std::cout << "-1\n";
        continue;
      }
      Train train;
      train_river.read(train, res[0].pos);
      train_table.erase(res[0]);
      res[0].release = true;
      train_table.insert(res[0]);
      Time t2 = train.saleDate[0] + train.startTime;
      for (int i = 0; i < train.stationNum; i++) {
        DateLocation_Train dlt;
        dlt.date = t2;
        strcpy(dlt.to, train.stations[i]);
        strcpy(dlt.trainID, train.trainID);
        date_location_train_table.insert(dlt);
        t2 += train.travelTimes[i];
        t2 += train.stopoverTimes[i];
      }
      t2 = train.saleDate[1] + train.startTime;
      for (int i = 0; i < train.stationNum; i++) {
        DateLocation_Train dlt;
        dlt.date = t2;
        strcpy(dlt.to, train.stations[i]);
        strcpy(dlt.trainID, train.trainID);
        date_location_train_table2.insert(dlt);
        t2 += train.travelTimes[i];
        t2 += train.stopoverTimes[i];
      }
      cout << "0\n";
    } else if (command[0] == 'q' and command[8] == 'a') { // query_train
      std::string i, c;
      do {
        cin >> in;
        if (in == "-i") {
          cin >> i;
        } else if (in == "-d") {
          cin >> c;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      Time t;
      t.month = stoi(c.substr(0, 2));
      t.day = stoi(c.substr(3, 2));
      Index_train query_train, query_train_max;
      strcpy(query_train.trainID, i.c_str());
      strcpy(query_train_max.trainID, i.c_str());
      query_train_max.pos = max_ind;
      sjtu::vector<Index_train> res =
          train_table.find(query_train, query_train_max);
      if (res.empty()) {
        std::cout << "-1\n";
        continue;
      }
      Train train;
      train_river.read(train, res[0].pos);
      if (t < train.saleDate[0] or train.saleDate[1] < t) {
        std::cout << "-1\n";
        continue;
      }
      query_trains(&train, &t);
    } else if (command[0] == 'd') { // delete_train
      std::string i;
      do {
        cin >> in;
        if (in == "-i") {
          cin >> i;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      Index_train query_train, query_train_max;
      strcpy(query_train.trainID, i.c_str());
      strcpy(query_train_max.trainID, i.c_str());
      query_train_max.pos = max_ind;
      sjtu::vector<Index_train> res =
          train_table.find(query_train, query_train_max);
      if (res.empty() or res[0].release) {
        std::cout << "-1\n";
        continue;
      }
      Train train;
      train_river.read(train, res[0].pos);
      train_table.erase(res[0]);
      std::cout << "0\n";
      continue;
    } else if (command[0] == 'q' and command[7] == 'i') { // query_ticket
      std::string s, t, d, p = "time";
      do {
        cin >> in;
        if (in == "-s") {
          cin >> s;
        } else if (in == "-t") {
          cin >> t;
        } else if (in == "-d") {
          cin >> d;
        } else if (in == "-p") {
          cin >> p;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      Time time;
      time.month = stoi(d.substr(0, 2));
      time.day = stoi(d.substr(3, 2));
      time.hour = 23;
      time.minute = 60;
      sjtu::vector<DemoTrain> res3 = _query_train(s, time, t, false);
      cout << res3.size() << '\n';
      if (p == "time") {
        res3.sort(timecmp);
      } else if (p == "cost") {
        res3.sort(costcmp);
      }
      for (int i = 0; i < res3.size(); i++) {
        cout << res3[i].trainID << ' ' << s << ' ' << res3[i].startTime
             << " -> " << t << ' ' << res3[i].endTime << ' ' << res3[i].prices
             << ' ' << res3[i].num << '\n';
      }
    } else if (command[0] == 'q' and command[8] == 'n') { // query_transfer
      std::string s, t, d, p = "time";
      do {
        cin >> in;
        if (in == "-s") {
          cin >> s;
        } else if (in == "-t") {
          cin >> t;
        } else if (in == "-d") {
          cin >> d;
        } else if (in == "-p") {
          cin >> p;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      Time time;
      time.month = stoi(d.substr(0, 2));
      time.day = stoi(d.substr(3, 2));
      time.hour = 23;
      time.minute = 60;
      sjtu::vector<DemoTrain2> res3 = _query_transfer(s, time, t);
      if (res3.empty()) {
        cout << "0\n";
        continue;
      }
      if (p == "time") {
        res3.sort(timecmp2);
      } else if (p == "cost") {
        res3.sort(costcmp2);
      }
      cout << res3[0].trainID << ' ' << s << ' ' << res3[0].startTime << " -> "
           << res3[0].transfer_locate << ' ' << res3[0].endTime << ' '
           << res3[0].price1 << ' ' << res3[0].num1 << '\n';
      cout << res3[0].trainID2 << ' ' << res3[0].transfer_locate << ' '
           << res3[0].startTime2 << " -> " << t << ' ' << res3[0].endTime2
           << ' ' << res3[0].price2 << ' ' << res3[0].num2 << '\n';
    } else if (command[0] == 'b') { // buy_ticket
      std::string u, i, d, s, f, t;
      bool q = false;
      int n = -1;
      do {
        cin >> in;
        if (in == "-u") {
          cin >> u;
        } else if (in == "-i") {
          cin >> i;
        } else if (in == "-d") {
          cin >> d;
        } else if (in == "-n") {
          cin >> n;
        } else if (in == "-q") {
          cin >> s;
          if (s == "true")
            q = true;
        } else if (in == "-f") {
          cin >> f;
        } else if (in == "-t") {
          cin >> t;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      if (user_pool.find(u) == user_pool.end()) {
        std::cout << "-1\n";
        continue;
      }
      Index_train query_train, query_train_max;
      strcpy(query_train.trainID, i.c_str());
      strcpy(query_train_max.trainID, i.c_str());
      query_train_max.pos = max_ind;
      sjtu::vector<Index_train> res =
          train_table.find(query_train, query_train_max);
      if (res.empty() or !res[0].release) {
        std::cout << "-1\n";
        continue;
      }
      Train train;
      train_river.read(train, res[0].pos);
      Order order;
      bool bl = buy_ticket(train, u, i, d, f, t, n, q, order);
      train_river.update(train, res[0].pos);
      if (!bl) {
        std::cout << "-1\n";
        continue;
      }
      order.index = ind;
      if (order.status == 1) {
        cout << order.price << '\n';
      } else if (order.status == 2) {
        cout << "queue\n";
      } else {
        cout << "-1\n";
      }
      DemoOrder demo_order;
      strcpy(demo_order.username, order.username);
      demo_order.pos = order_river.write(order);
      order_table.insert(demo_order);
      if (order.status == 2) {
        DemoOrder2 demo_order2;
        strcpy(demo_order2.trainID, order.trainID);
        demo_order2.pos = demo_order.pos;
        demo_order2.date = order.dat;
        demo_order2.loc = order.loc;
        demo_order2.end_loc = order.end_loc;
        demo_order2.n = n;
        waiting_list.insert(demo_order2);
      }
    } else if (command[0] == 'q' and command[6] == 'o') { // query_order
      std::string u;
      do {
        cin >> in;
        if (in == "-u") {
          cin >> u;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      if (user_pool.find(u) == user_pool.end()) {
        std::cout << "-1\n";
        continue;
      }
      DemoOrder query_order, query_order_max;
      strcpy(query_order.username, u.c_str());
      strcpy(query_order_max.username, u.c_str());
      query_order_max.pos = max_ind;
      sjtu::vector<DemoOrder> res =
          order_table.find(query_order, query_order_max);
      cout << res.size() << '\n';
      for (int i = res.size() - 1; i >= 0; i--) {
        Order order;
        order_river.read(order, res[i].pos);
        cout << order;
      }
    } else if (command[0] == 'r' and command[2] == 'f') { // refund_ticket
      std::string u;
      int n = 1;
      do {
        cin >> in;
        if (in == "-u") {
          cin >> u;
        } else if (in == "-n") {
          cin >> n;
        } else if (in.length() > 0 and in[0] == '[') {
          index = in;
        }
      } while (in.length() > 0 and in[0] != '[');
      if (user_pool.find(u) == user_pool.end()) {
        std::cout << "-1\n";
        continue;
      }
      DemoOrder query_order, query_order_max;
      strcpy(query_order.username, u.c_str());
      strcpy(query_order_max.username, u.c_str());
      query_order_max.pos = max_ind;
      sjtu::vector<DemoOrder> res =
          order_table.find(query_order, query_order_max);
      if (n > res.size()) {
        std::cout << "-1\n";
        continue;
      }
      Order order;
      order_river.read(order, res[res.size() - (n - 1) - 1].pos);
      if (order.status == 3) {
        std::cout << "-1\n";
        continue;
      }
      int order_status = order.status;
      order.status = 3;
      order_river.update(order, res[res.size() - (n - 1) - 1].pos);
      DemoOrder2 query_order2, query_order2_max;
      strcpy(query_order2.trainID, order.trainID);
      strcpy(query_order2_max.trainID, order.trainID);
      query_order2.date = order.dat;
      query_order2_max.date = order.dat;
      query_order2.loc = order.loc;
      query_order2_max.loc = order.loc;
      query_order2_max.pos = max_ind;
      Index_train query_train, query_train_max;
      strcpy(query_train.trainID, order.trainID);
      strcpy(query_train_max.trainID, order.trainID);
      query_train_max.pos = max_ind;
      sjtu::vector<Index_train> res2 =
          train_table.find(query_train, query_train_max);
      if (res2.empty()) {
        std::cout << "-1\n";
        continue;
      }
      Train train;
      train_river.read(train, res2[0].pos);
      if (order_status == 1) {
        for (int i = order.loc; i < order.end_loc; i++) {
          train.seatNum[order.dat][i] -= order.num;
        }
      } else {
        query_order2.pos = res[res.size() - (n - 1) - 1].pos;
        query_order2_max.pos = res[res.size() - (n - 1) - 1].pos;
        sjtu::vector<DemoOrder2> res3 =
            waiting_list.find(query_order2, query_order2_max);
        waiting_list.erase(res3[0]);
        cout << "0\n";
        continue;
      }
      sjtu::vector<DemoOrder2> res4;
      query_order2.loc = 0;
      query_order2_max.loc = 0;
      for (int i = 0; i < train.stationNum; i++) {
        sjtu::vector<DemoOrder2> res3 =
            waiting_list.find(query_order2, query_order2_max);
        for (int j = 0; j < res3.size(); j++) {
          res4.push_back(res3[j]);
        }
        query_order2.loc++;
        query_order2_max.loc++;
      }
      res4.sort(waitingcmp);
      for (int j = 0; j < res4.size(); j++) {
        Order order2;
        order_river.read(order2, res4[j].pos);
        bool bl = buy_ticket(train, order2.dat, order2.loc, order2.end_loc,
                             order2.num);
        if (!bl) {
          continue;
        }
        order2.status = 1;
        order_river.update(order2, res4[j].pos);
        waiting_list.erase(res4[j]);
      }
      train_river.update(train, res2[0].pos);
      cout << "0\n";
    }
  }
}
