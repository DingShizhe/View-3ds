#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/vector_angle.hpp>


#define print_vec4(vec) std::cout << vec.x << " " \
    << vec.y << " " \
    << vec.z << " " \
    << vec.w << " " << std::endl;

int main(int argc, char const *argv[])
{
    // glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec3 a(1.0, 0.0, 0.0);
    glm::vec3 b(0.0, 1.0, 0.0);
    std::cout << glm::angle(a, b) << std::endl;

    glm::mat4 view;
    glm::vec3 view_pos = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 model_pos = glm::vec3(0.0,0.0,0.0);
    view = glm::lookAt(view_pos, model_pos, glm::vec3(0.0,0.0,1.0));
    glm::vec4 new_model_pos = view * glm::vec4(model_pos, 1.0);
    print_vec4(new_model_pos);
    return 0;
}