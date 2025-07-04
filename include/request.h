#pragma once

#include <clip.h>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <lib/json.hpp>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
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

// Helper class to restore terminal mode on scope exit (RAII)
class TermModeGuard {
private:
  termios oldt;

public:
  TermModeGuard() {
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  }
  ~TermModeGuard() { tcsetattr(STDIN_FILENO, TCSANOW, &oldt); }
};

// Move cursor up by n lines
void moveCursorUp(int n) {
  if (n > 0)
    std::cout << "\033[" << n << "A";
}

// Clear from cursor to end of screen
void clearFromCursor() { std::cout << "\033[0J"; }

// Clear entire screen
void clearScreen() { std::cout << "\033[2J\033[H"; }

// Move cursor to specific position (row, col)
void moveCursorTo(int row, int col) {
  std::cout << "\033[" << row << ";" << col << "H";
}

// Clear entire line
void clearLine() { std::cout << "\033[2K\r"; }

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
};

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

  void execute() const {
    char input;
    int selection = 0;
    std::vector<std::pair<std::string, std::string>> commands = {
        {"General Command", command}, {"Runnable Command", runnableCommand}};

    TermModeGuard guard; // RAII for terminal mode

    // Print the complete interface initially
    printCompleteInterface(selection);

    while (true) {
      input = std::getchar();

      if (input == '\033') { // Arrow keys start with ESC sequence
        std::getchar();      // skip '['
        switch (std::getchar()) {
        case 'A':
          selection = (selection - 1 + commands.size()) % commands.size();
          break; // up
        case 'B':
          selection = (selection + 1) % commands.size();
          break; // down
        }

        // Clear screen and reprint everything
        clearScreen();
        printCompleteInterface(selection);

      } else if (input == '\n') {
        clip::set_text(commands[selection].second);
        break;
      }
    }

    return;
  }

  // Print the complete interface (question, explanation, and menu)
  void printCompleteInterface(int selection) const {
    // Print the static content (question, explanation)
    std::cout << RESET << WHITE << "You: " << question << RESET << BOLD << CYAN
              << "\n"
              << DIVIDER << "\n"
              << RESET << "\n";

    std::cout << wrapText(explanation) << "\n\n";

    // Print the menu
    printMenu(selection);
  }

  // Print the initial menu
  void printMenu(int selection) const {
    // First command (General Command)
    std::cout << (selection == 0 ? RESET : WHITE) << " >> " << RESET << BOLD
              << GREEN << command << RESET << (selection == 0 ? " << " : "")
              << WHITE
              << (selection == 0 ? " press enter to save to clipboard" : "")
              << "\n";

    // Parameters section (if any)
    if (!parameters.empty()) {
      size_t maxKeyWidth = 0;
      for (const auto &[key, _] : parameters) {
        maxKeyWidth = std::max(maxKeyWidth, key.length());
      }

      std::cout << BOLD << CYAN << "\n" << RESET;
      for (const auto &[key, value] : parameters) {
        std::cout << "  " << std::setw(static_cast<int>(maxKeyWidth))
                  << std::left << YELLOW << key << RESET << " : " << WHITE
                  << value << RESET << "\n";
      }
    }

    std::cout << BOLD << CYAN << "\n"
              << DIVIDER << "\n"
              << "Example Commands: \n\n"
              << RESET;

    // Second command (Runnable Command)
    std::cout << (selection == 1 ? RESET : WHITE) << " >> " << MAGENTA
              << runnableCommand << RESET << (selection == 1 ? " << " : "")
              << WHITE
              << (selection == 1 ? " press enter to save to clipboard" : "")
              << RESET << "\n\n";
  }

  void print(std::ostream &os = std::cout, int selection = -1) const {
    os << RESET << WHITE << "You: " << question << RESET << BOLD << CYAN << "\n"
       << DIVIDER << "\n"
       << RESET << "\n";

    os << wrapText(explanation) << "\n\n";

    os << (selection == 0 ? RESET : WHITE) << " >> " << RESET << BOLD << GREEN
       << command << RESET << (selection == 0 ? " << " : "") << WHITE
       << (selection == 0 ? " press enter to save to clipboard" : "") << "\n";

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
       << RESET << (selection == 1 ? RESET : WHITE) << " >> " << MAGENTA
       << runnableCommand << RESET << (selection == 1 ? " << " : "") << WHITE
       << (selection == 1 ? " press enter to save to clipboard" : "") << RESET
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
