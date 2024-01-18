#include "../include/loginHandler.hpp"
int main() {
  LoginHandler loginHandler(8989);
  while(true)
    loginHandler.listen();
  return 0;
}
