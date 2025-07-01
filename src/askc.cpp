#include <fstream>
#include <gemini.h>
#include <iostream>
#include <lib/json.hpp>
#include <request.h>

using namespace std;

bool checkIfConfigFileExists() {
  ifstream file("config.json");
  bool res = file.is_open();
  return res;
}

pair<string, string> getConfig() {
  ifstream file("config.json");

  nlohmann::json config = nlohmann::json::parse(file);
  string provider = config["provider"].get<string>();
  if (provider == "gemini") {
    string apiKey = config["gemini_api_key"].get<string>();
    if (!apiKey.empty()) {
      return {provider, apiKey};
    }

    return {provider, ""};
  }

  return {"", ""};
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: askc <question>\n";
    return 1;
  }
  string question = argv[1];

  if (!checkIfConfigFileExists()) {
    nlohmann::json baseConfig = {
        {"provider", "gemini"},
        {"gemini_api_key", ""},
    };

    ofstream file("config.json");
    file << setw(4) << baseConfig << endl;
  }
  pair<string, string> config = getConfig();

  string provider = config.first;
  string apiKey = config.second;
  if (provider.empty()) {
    cerr << "Provider is not set or not available" << endl
         << endl
         << "    Please set the provider to one of the available once with:"
         << endl
         << "- gemini" << endl;
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
