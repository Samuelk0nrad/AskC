#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: greet <name>\n";
    return 1;
  }

  string name = argv[1];
  cout << "Hello, " << name << "!" << std::endl;

  return 0;
}
