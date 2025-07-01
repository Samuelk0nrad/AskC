#pragma once

#include <string>
class Request {
public:
  virtual std::string question(std::string &input) { return input; };

protected:
  std::string apikey;
};
