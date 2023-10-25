#include <iostream>
using namespace std;

int main() {
  int a, b;
  char o;
  cin >> a >> b >> o;
  switch (o) {
    case '+':
      cout << a + b << endl;
      break;
    case '-':
      cout << a - b << endl;
      break;
    case '*':
      cout << a * b << endl;
      break;
    case '/':
      break;
  }
  return 0;
}