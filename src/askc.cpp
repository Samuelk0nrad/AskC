#include "settings/settings.h"
#include <gemini.h>
#include <iostream>
#include <lib/json.hpp>
#include <request.h>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc >= 2) {
    string tag = argv[1];
    if (tag == "config") {
      enterSettingsMenue();
      return 0;
    }
  }
  if (argc < 2) {
    cerr << "Usage: askc <question>\n";
    return 1;
  }

  string question = argv[1];

  if (!checkIfConfigFileExists(PATH)) {
    createDefultConfig(PATH);
  }

  pair<string, string> config = getConfig(PATH);

  string provider = config.first;
  string apiKey = config.second;
  if (provider.empty()) {
    cerr << "Provider is not set or not available" << endl
         << endl
         << "    Please set the provider to one of the available once with:"
         << endl
         << "    - gemini" << endl;
    return 1;
  } else if (apiKey.empty()) {
    cerr
        << "No API key set / Please set you'er api key for the choosen provider"
        << endl;
    return 1;
  }

  Gemini gem("some API key");
  Request res = gem;

  cout << res.question(question) << endl << provider << endl << apiKey << endl;
  return 0;
}
