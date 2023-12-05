#include "shape/cube.h"

Cube::Cube(float *vertices, cube_state state) {
  glGenVertexArrays(1, &ID);
  glGenBuffers(1, &VBO);

  glBindVertexArray(ID);

  bindCubeData(vertices, state);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Cube::use() { glBindVertexArray(ID); }

void Cube::bindCubeData(float *vertices, cube_state state) {
  if (state == cube_state::without_texture) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(float), vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
  } else if (state == cube_state::with_texture) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 36 * 5 * sizeof(float), vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }
}
