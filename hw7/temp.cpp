#include <iostream>
#include <string>
using namespace std;

int main ()
{
  string mystr;
  cout << "What's your name?" << endl;
  getline (cin, mystr);
  cout << "Hello " << mystr << endl;

  return 0;
}