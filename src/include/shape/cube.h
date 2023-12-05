#include <glad/glad.h>

enum class cube_state { with_texture, without_texture };

class Cube {
public:
  unsigned int ID, VBO;
  Cube(float *vertices, cube_state state);
  Cube() = delete;
  void use();

private:
  void bindCubeData(float *vertices, cube_state state);
};
