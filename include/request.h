#pragma once

#include <cmath>
#include <iostream>
#include <lib/json.hpp>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// ANSI escape codes for colors and styles
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define DIVIDER "────────────────────────────────────────────"

std::string wrapText(const std::string &text, std::size_t width = 80) {
  std::istringstream words(text);
  std::ostringstream wrapped;
  std::string word;
  std::string line;

  while (words >> word) {
    if (line.length() + word.length() + 1 > width) {
      wrapped << line << '\n';
      line = word;
    } else {
      if (!line.empty())
        line += ' ';
      line += word;
    }
  }

  if (!line.empty()) {
    wrapped << line << '\n';
  }

  return wrapped.str();
}

class Response {
public:
  Response(const std::string &q, const nlohmann::json &j) {
    this->question = q;
    this->explanation = j["generalExplanation"];
    this->command = j["command"];
    this->runnableCommand = j["runnableCommand"];
    if (j.contains("parameters")) {
      for (const auto &element : j["parameters"]) {
        parameters.push_back({element["parameter"], element["description"]});
      }
    }
  };
  Response() {}
  std::string question;
  std::string explanation;
  std::string command;
  std::string runnableCommand;
  std::vector<std::pair<std::string, std::string>> parameters;

  void print(std::ostream &os = std::cout) const {
    os << RESET << WHITE << "You: " << question << RESET << BOLD << CYAN << "\n"
       << DIVIDER << "\n"
       << RESET << "\n";

    os << wrapText(explanation) << "\n\n";

    os << WHITE << " >> " << RESET << BOLD << GREEN << command << RESET << "\n";

    if (!parameters.empty()) {
      size_t maxKeyWidth = 0;
      for (const auto &[key, _] : parameters) {
        maxKeyWidth = std::max(maxKeyWidth, key.length());
      }

      os << BOLD << CYAN << "\n" << RESET;
      for (const auto &[key, value] : parameters) {
        os << "  " << std::setw(static_cast<int>(maxKeyWidth)) << std::left
           << YELLOW << key << RESET << " : " << WHITE << value << RESET
           << "\n";
      }
    }

    os << BOLD << CYAN << "\n"
       << DIVIDER << "\n"
       << "Example Commands: \n\n"
       << RESET << WHITE << " >> " << MAGENTA << runnableCommand << RESET
       << "\n\n";
  }

  friend std::ostream &operator<<(std::ostream &os, const Response &obj) {
    os << "Question: " << obj.question << "\n"
       << "Command: " << obj.command << "\n"
       << "Run:     " << obj.runnableCommand << "\n";
    return os;
  }
};

class Request {
public:
  virtual Response question(std::string &input) { return Response(); };

protected:
  std::string apikey;
};
