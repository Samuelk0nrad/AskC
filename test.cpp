
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

// Injects a string into terminal input buffer
void injectToShell(const std::string &command) {
  for (char c : command) {
    ioctl(STDIN_FILENO, TIOCSTI, &c);
  }
}

int main() {
  std::vector<std::string> commands = {"ls -la", "git status"};

  int selected = 0;
  char input;

  // Turn off terminal buffering and echo
  termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  // Loop to handle arrow key navigation
  while (true) {
    system("clear"); // or use ANSI to clear if preferred
    std::cout << "Use / to select a command, Enter to inject it:\n\n";

    for (size_t i = 0; i < commands.size(); ++i) {
      if (i == selected)
        std::cout << "> \033[1;32m" << commands[i] << "\033[0m\n";
      else
        std::cout << "  " << commands[i] << "\n";
    }

    input = getchar();

    if (input == '\033') { // Arrow keys start with ESC sequence
      getchar();           // skip '['
      switch (getchar()) {
      case 'A':
        selected = (selected - 1 + commands.size()) % commands.size();
        break; // up
      case 'B':
        selected = (selected + 1) % commands.size();
        break; // down
      }
    } else if (input == '\n') {
      injectToShell(commands[selected]);
      break;
    }
  }

  // Restore terminal
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return 0;
}
