#pragma once
#include <request.h>
#include <string>

class Gemini : public Request {
public:
  Gemini(const std::string apiKey) { this->apikey = apikey; };
  std::string question(std::string &input) override {
    return "Hello from Gemini";
  }
};
