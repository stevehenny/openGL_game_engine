#include "ShaderPrograms.h"
#include <iostream>

using std::string, std::string_view;
string getGitRepoRoot() {
  FILE *pipe = popen("git rev-parse --show-toplevel 2>/dev/null", "r");
  if (!pipe) {
    return "";
  }

  char buffer[128];
  std::string result = "";
  while (!feof(pipe)) {
    if (fgets(buffer, 128, pipe) != nullptr) {
      result += buffer;
    }
  }
  pclose(pipe);

  // Remove trailing newline character if present
  if (!result.empty() && result.back() == '\n') {
    result.pop_back();
  }
  std::cout << buffer << '\n';
  return result;
}
