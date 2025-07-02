#include "settings.h"
#include <cerrno>
#include <fstream>
#include <iostream>
#include <lib/json.hpp>

using namespace std;

bool checkIfConfigFileExists(string path) {
  ifstream file(path);
  bool res = file.is_open();
  return res;
}

void createDefultConfig(string path) {
  ofstream file(path);
  file << setw(4) << DEFAULTCONFIG << endl;
}

pair<string, string> getConfig(string path) {
  ifstream file(path);

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

void setSetting(string key, string value, string path) {
  ifstream in(path);
  nlohmann::json data = nlohmann::json::parse(in);
  data[key] = value;
  ofstream out(path);
  out << setw(4) << data << endl;
  out.close();
}

void printManue() {
  cout << "1. Set Provider" << endl;
  cout << "2. Config Provider" << endl;
  cout << "3. exit" << endl;
  cout << endl;
}

void printProviders() {
  for (int i = 0; i < PROVIDERS_COUNT; i++) {
    cout << i << ". " << PROVIDERS[i] << endl;
  }
}

void setProvider() {
  cout << "\n Select one of the avaliable providers \n";
  printProviders();
  int input = -1;
  cin >> input;

  setSetting("provider", PROVIDERS[input], PATH);
}

void configProviders() {
  cout << "\n Select one of the avaliable providers \n";

  printProviders();
  int input = -1;
  cin >> input;
  string provider = PROVIDERS[input];
  cout << "\n \n Configer " << provider << endl;

  cout << "1. Set " << provider << "API Key" << endl;

  cin >> input;
  cin >> input;
  if (input == 1) {
    string apikey = "";
    cout << "\nEnter your API key: ";
    getline(cin, apikey);
    setSetting(provider + "_api_key", apikey, PATH);
    return;
  }
}

void enterSettingsMenue() {
  while (true) {
    printManue();
    int input = 0;
    cin >> input;

    switch (input) {
    case 1:
      setProvider();
      break;
    case 2:
      configProviders();
      break;
    case 3:
      return;
      break;
    default:
      cout << "Not a option";
    }
  }
}
