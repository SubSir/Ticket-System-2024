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
  sjtu::map<std::string, User> user_pool;
  std::string i, s, p, x, t, o, d, y;
  fstream fin("root", ios::in | ios::out);
  int n, m;
  while (true) {
    string command;
    cin >> command;
    if (command == "exit") {
      cout << "bye" << '\n';
      break;
    } else if (command == "clear") {
      delete user_table;
      delete train_table;
      std::ofstream fout("BPTuser", std::ios::trunc);
      fout.close();
      fout.open("BPTtrain", std::ios::trunc);
      fout.close();
      fout.open("binuser", std::ios::trunc);
      fout.close();
      fout.open("bintrain", std::ios::trunc);
      fout.close();
      fin.close();
      fout.open("root", std::ios::trunc);
      fout.close();
      fstream fin("root", ios::in | ios::out);
      user_table = new BPT<User>("user");
      train_table = new BPT<Train>("train");
      user_pool.clear();
    } else if (command == "add_user") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string c, u, p, n, m;
      int cnt = 0, cnt2 = 0;
      int g;
      while (getline(iss, i, ' ')) {
        if (i == "-c") {
          cnt++;
          getline(iss, c, ' ');
        } else if (i == "-u") {
          cnt2++;
          getline(iss, u, ' ');
        } else if (i == "-p") {
          cnt2++;
          getline(iss, p, ' ');
        } else if (i == "-n") {
          cnt2++;
          getline(iss, n, ' ');
        } else if (i == "-m") {
          cnt2++;
          getline(iss, m, ' ');
        } else if (i == "-g") {
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
    } else if (command == "login") {
      std::string line;
      std::getline(std::cin, line);
      std::istringstream iss(line);
      std::string u, p;
      int cnt = 0;
      while (getline(iss, i, ' ')) {
        if (i == "-u") {
          cnt++;
          getline(iss, u, ' ');
        } else if (i == "-p") {
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
      while (getline(iss, i, ' ')) {
        if (i == "-u") {
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
      while (getline(iss, i, ' ')) {
        if (i == "-u") {
          cnt++;
          getline(iss, u, ' ');
        } else if (i == "-c") {
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
      while (getline(iss, i, ' ')) {
        if (i == "-u") {
          getline(iss, u, ' ');
        } else if (i == "-p") {
          getline(iss, p, ' ');
        } else if (i == "-n") {
          getline(iss, n, ' ');
        } else if (i == "-m") {
          getline(iss, m, ' ');
        } else if (i == "-c") {
          getline(iss, c, ' ');
        } else if (i == "-g") {
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
      std::string c, u, p, n, m;
    }
  }
}