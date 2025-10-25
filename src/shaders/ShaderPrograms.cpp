#include "ShaderPrograms.h"

std::string getGitRepoRoot() {
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
  return result;
}

static std::string SHADER_FOLDER = getGitRepoRoot() + "/src/shaders/";

ShaderPrograms::ShaderPrograms() {}

ShaderPrograms::~ShaderPrograms() {}

void ShaderPrograms::CompileShaders() {}
