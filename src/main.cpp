#include "file_storage.hpp"
#include "time.hpp"
#include "train.hpp"
#include "user.hpp"
#include <iostream>
using namespace std;
using Userpair = std::pair<std::string, User>;
using Trainpair = std::pair<std::string, Train>;
int main() {
  BPT<Userpair> user_table("user");
  BPT<Trainpair> train_table("train");
  while (true) {
    string command;
    cin >> command;
    if (command == "exit") {
      cout << "bye" << '\n';
      break;
    }
  }
}