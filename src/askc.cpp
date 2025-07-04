#include "settings/settings.h"
#include <cpr/cpr.h>
#include <gemini.h>
#include <iostream>
#include <lib/json.hpp>
#include <request.h>

using namespace std;

void testAPI(string apiKey) {

  nlohmann::json body = {
      {"contents",
       {{{"parts", {{{"text", "Explain how AI works in a few words"}}}}}}}};

  cpr::Response r =
      cpr::Post(cpr::Url{"https://generativelanguage.googleapis.com/v1beta/"
                         "models/gemini-2.5-flash:generateContent"},
                cpr::Header{{"x-goog-api-key", apiKey},
                            {"Content-Type", "application/json"}},
                cpr::Body{body.dump()});

  // Print the response
  std::cout << "Status code: " << r.status_code << '\n';
  std::cout << "Response: " << r.text << '\n';
}

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

  // cout << apiKey << endl;

  Gemini gem(apiKey);
  Request res = gem;

  // testAPI(apiKey);

  gem.question(question).execute();

  return 0;
}
