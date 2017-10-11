#include <FragmentShader.hpp>
#include <VertexShader.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <ShaderSquare.hpp>
#include "ShaderScene.hpp"

#define XMAX 1000
#define RESOLUTION 500.0f

using namespace std;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;
using glm::radians;

ShaderScene::ShaderScene(int w, int h) : NoiseScene(w,
                                                    h,
                                                    XMAX,
                                                    RESOLUTION,
                                                    std::make_unique<ShaderSquare>(XMAX, RESOLUTION)) { }


void ShaderScene::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);

    auto& program = landscape->program;
    program.use();

    mat4 view = camera.getViewMatrix();
    mat4 projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 500.0f);
    program.setUniform("ProjectionMatrix", projection);
    program.setUniform("ViewMatrix", view);

    mat4 model = mat4(1.0f);
    model = glm::translate(model, vec3(-XMAX / 2, 0.0f, -XMAX / 2));
    mat4 mv = view * model;
    mat3 normalMatrix = glm::inverseTranspose(mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    program.setUniform("ModelMatrix", model);
    program.setUniform("NormalMatrix", normalMatrix);

    landscape->draw();
}


void ShaderScene::redraw() {
    landscape->uploadNewData();
    landscape->printInfo();
}
