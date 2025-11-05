#include <string>
std::string getGitRepoRoot();

using std::string;
static string SHADER_FOLDER = getGitRepoRoot() + "/build/src/shaders/spv/";

namespace compiled_shaders {
inline string OBJECT_VERTEX = SHADER_FOLDER + "vertex_light.vert.spv";
inline string OBJECT_FRAG = SHADER_FOLDER + "frag_light.frag.spv";
inline string LIGHT_VERTEX = SHADER_FOLDER + "light_source.vert.spv";
inline string LIGHT_FRAG = SHADER_FOLDER + "light_source.frag.spv";
inline string OBJECT_W_TEXTURE_VERT =
    SHADER_FOLDER + "object_w_texture.vert.spv";
inline string OBJECT_W_TEXTURE_FRAG =
    SHADER_FOLDER + "object_w_texture.frag.spv";
inline string LIGHT_W_TEXTURE_VERT =
    SHADER_FOLDER + "light_source_w_texture.vert.spv";
inline string LIGHT_W_TEXTURE_FRAG =
    SHADER_FOLDER + "light_source_w_texture.frag.spv";

inline string PLANE_GRAVITY_VERT = SHADER_FOLDER + "plane_gravity.vert.spv";

inline string PLANE_GRAVITY_FRAG = SHADER_FOLDER + "plane_gravity.frag.spv";
inline string TEXT_VERT = SHADER_FOLDER + "text.vert.spv";
inline string TEXT_FRAG = SHADER_FOLDER + "text.frag.spv";
inline string MULTI_OBJ_PLANE_VERT =
    SHADER_FOLDER + "plane_gravity_multi.vert.spv";
inline string MULTI_OBJ_PLANE_FRAG =
    SHADER_FOLDER + "plane_gravity_multi.frag.spv";
inline string SPHERES_VERT = SHADER_FOLDER + "Spheres.vert.spv";
inline string SPHERES_FRAG = SHADER_FOLDER + "Spheres.frag.spv";
} // namespace compiled_shaders
