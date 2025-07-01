#include <gemini.h>
#include <iostream>
#include <request.h>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: askc <question>\n";
    return 1;
  }

  string question = argv[1];
  Gemini gem("some API key");
  Request res = gem;

  cout << res.question(question) << endl;
  return 0;
}
