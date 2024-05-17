#include "user.hpp"
#include "time.hpp"
#include <cstring>
#include <iostream>
#include <ostream>
#include <regex>
#include <string>
bool valid_username(const std::string &u) {
  if (u.length() == 0 || u.length() > 20 || u[0] > 'z' ||
      (u[0] < 'a' && (u[0] < 'A' || u[0] > 'Z')))
    return false;
  for (char c : u) {
    if (c != '_' and !isalnum(c))
      return false;
  }
  return true;
}
bool valid_password(const std::string &p) {
  if (p.empty() || p.length() > 30)
    return false;
  for (char c : p) {
    if (!isprint(c))
      return false;
  }
  return true;
}
bool valid_name(const std::string &n) {
  return n.length() >= 6 && n.length() <= 15;
}
bool valid_privilege(int g) { return g >= 0 && g <= 10; }

bool valid_mail(const std::string &m) { return m.length() <= 30; }

bool add_user(const std::string &u, const std::string &p, const std::string &n,
              const std::string &m, int g, User &user) {
  // if (!valid_username(u) || !valid_password(p) || !valid_name(n) ||
  //     !valid_privilege(g) || !valid_mail(m))
  //   return false;
  strcpy(user.username, u.c_str());
  strcpy(user.password, p.c_str());
  strcpy(user.name, n.c_str());
  strcpy(user.mail, m.c_str());
  user.privilege = g;
  return true;
}
bool login(const std::string &u, const std::string &p, User *user) {
  return strcmp(u.c_str(), user->username) == 0 &&
         strcmp(p.c_str(), user->password) == 0;
}
void query_profile(User *user) {
  std::cout << user->username << ' ' << user->name << ' ' << user->mail << ' '
            << user->privilege << '\n';
}
bool modify_profile(User *user, const std::string &p, const std::string &n,
                    const std::string &m, int g) {
  // if ((!valid_password(p) and p.length() > 0) ||
  //    (!valid_name(n) and n.length() > 0) || (!valid_privilege(g) and g >= 0))
  // return false;
  if (p.length() > 0)
    strcpy(user->password, p.c_str());
  if (n.length() > 0)
    strcpy(user->name, n.c_str());
  if (m.length() > 0)
    strcpy(user->mail, m.c_str());
  if (g >= 0)
    user->privilege = g;
  std::cout << user->username << ' ' << user->name << ' ' << user->mail << ' '
            << user->privilege << '\n';
  return true;
}