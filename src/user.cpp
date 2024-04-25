#include <cstring>
#include <iostream>
#include <regex>
#include <string>
bool valid_username(const std::string &u) {
  if (u.length() == 0 || u.length() > 20 || u[0] > 'z' || u[0] < 'a')
    return false;
  for (char c : u) {
    if (c != '_' and !isalnum(c))
      return false;
  }
  return true;
}
bool valid_password(const std::string &p) {
  if (p.length() < 6 || p.length() > 30)
    return false;
  for (char c : p) {
    if (c != '_' and !isalnum(c))
      return false;
  }
  return true;
}
bool valid_name(const std::string &n) {
  std::regex pattern("^[\u4e00-\u9fa5]{2,5}$");
  return std::regex_match(n, pattern);
}
bool valid_privilege(int g) { return g >= 0 && g <= 10; }
struct User {
  char username[21] = {};
  char password[31] = {};
  char name[16] = {};
  char mail[31] = {};
  int privilege = 0;
};
User *add_user(const std::string &u, const std::string &p, const std::string &n,
               const std::string &m, int g) {
  if (!valid_username(u) || !valid_password(p) || !valid_name(n) ||
      !valid_privilege(g))
    return nullptr;
  User *user = new User();
  strcpy(user->username, u.c_str());
  strcpy(user->password, p.c_str());
  strcpy(user->name, n.c_str());
  strcpy(user->mail, m.c_str());
  user->privilege = g;
  return user;
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
  if (!valid_password(p) || !valid_name(n) || !valid_privilege(g))
    return false;
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