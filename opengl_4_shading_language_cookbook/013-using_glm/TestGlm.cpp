#include <cstdlib>
#include <iomanip>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#ifdef __cplusplus // SDL2main
extern "C" 
#endif
int
main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    glm::vec4 position = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0));
    glm::mat4 mv = view * model;
    glm::vec4 transformed = mv * position;

    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(4);
    std::cout << "Transformed vec is x=" << transformed.x
              << " y=" << transformed.y
              << " z=" << transformed.z
              << "\n";
    
    return 0;
}
