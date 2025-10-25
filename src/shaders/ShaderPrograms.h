#include "ShaderProgram.h"
#include <unordered_map>
#include <vector>
std::string getGitRepoRoot();

class ShaderPrograms {

public:
  ShaderPrograms();
  ~ShaderPrograms();
  void CompileShaders();

private:
  std::unordered_map<std::string_view, ShaderProgram> programs;
};
