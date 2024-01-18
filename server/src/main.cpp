#include "../include/loginHandler.hpp"
int main() {
  LoginHandler loginHandler(8989);
  loginHandler.listen();
  return 0;
}
