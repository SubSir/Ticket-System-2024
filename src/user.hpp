#ifndef USER_HPP
#define USER_HPP

#include <string>

bool valid_username(const std::string &u);
bool valid_password(const std::string &p);
bool valid_name(const std::string &n);
bool valid_privilege(int g);

struct User {
  char username[21] = {};
  char password[31] = {};
  char name[16] = {};
  char mail[31] = {};
  int privilege = 0;
};

User *add_user(const std::string &u, const std::string &p, const std::string &n,
               const std::string &m, int g);

bool login(const std::string &u, const std::string &p, User *user);

void query_profile(User *user);

bool modify_profile(User *user, const std::string &p, const std::string &n,
                    const std::string &m, int g);

#endif // USER_HPP
