#include "settings.h"
#include <cerrno>
#include <fstream>
#include <ios>
#include <iostream>
#include <lib/json.hpp>
#include <limits>

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
  string apiKey = config[provider + "_api_key"].get<string>();
  return {provider, apiKey};
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
  cout << "##########################" << endl;
  cout << "#        Settings        #" << endl;
  cout << "+========================+" << endl;
  cout << "| -> 1. Set Provider     |" << endl;
  cout << "| -> 2. Config Provider  |" << endl;
  cout << "| -> 3. exit             |" << endl;
  cout << "+------------------------+" << endl;
  cout << endl;

  cout << " > ";
}

void printProviders() {
  cout << "Select one of the avaliable providers: " << endl;
  int maxChar = 0;
  for (int i = 0; i < PROVIDERS_COUNT; i++) {
    if (PROVIDERS[i].size() > maxChar)
      maxChar = PROVIDERS[i].size();
  }
  string dashes(maxChar + 8, '-');
  cout << "+" << dashes << "+" << endl;
  for (int i = 0; i < PROVIDERS_COUNT; i++) {
    string spaces(maxChar - PROVIDERS[i].size(), ' ');
    cout << "| -> " << i << ". " << PROVIDERS[i] << spaces << " |" << endl;
  }
  cout << "+" << dashes << "+" << endl;
}

void setProvider() {
  printProviders();
  int input = -1;
  cout << endl << " > ";
  cin >> input;
  cout << endl;

  setSetting("provider", PROVIDERS[input], PATH);
}

void configProviders() {
  printProviders();
  int input = -1;
  cout << endl << " > ";
  if (!(cin >> input)) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << endl << "not a valide input" << endl;
    return;
  }
  string provider = PROVIDERS[input];
  cout << endl << "----------------------------" << endl;
  cout << "Configer " << provider << ": " << endl;

  string dashes(PROVIDERS[input].size() + 20, '-');

  cout << "+" << dashes << "+" << endl;
  cout << "| -> 1. Set " << provider << " API Key |" << endl;
  cout << "+" << dashes << "+" << endl;

  cout << endl << " > ";
  if (!(cin >> input)) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << endl << "not a valide input" << endl;
    return;
  }
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cout << endl;
  if (input == 1) {
    string apikey = "";
    cout << "Enter your API key: ";
    getline(cin, apikey);
    setSetting(provider + "_api_key", apikey, PATH);
    cout << endl;
    return;
  }
}

void enterSettingsMenue() {
  while (true) {
    printManue();
    int input = 0;
    cin >> input;

    cout << endl;
    cout << "----------------------------" << endl;

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
      cout << "Not a option" << endl;
    }
  }
}
