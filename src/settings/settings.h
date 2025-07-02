#pragma once

#include <lib/json.hpp>
#include <string>
#include <utility>

bool checkIfConfigFileExists(std::string path);
void createDefultConfig(std::string path);
std::pair<std::string, std::string> getConfig(std::string path);

const std::string PATH = "config.json";
const int PROVIDERS_COUNT = 2;
const std::string PROVIDERS[] = {"gemini", "chatgpt"};
const nlohmann::json DEFAULTCONFIG = {
    {"provider", "gemini"},
    {"gemini_api_key", ""},
};

void enterSettingsMenue();
