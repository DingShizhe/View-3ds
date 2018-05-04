#include "main.hpp"
#include "model3ds.hpp"
#include "util.hpp"
#include "buffer.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <stdio.h>


Model3ds Model;
Buffer Buff;
glm::vec4 **Vertex_array;
vector<ModelObj *> OBvec;

void init() {
    glm::vec3 view_pos(2.0, 3.0, 4.0);
    glm::vec3 model_pos(0.0, 0.0, 0.0);
    glm::mat4 trans = lookAt(view_pos, model_pos, glm::vec3(0.0, 0.0, 1.0));
    // Model.Load("test4.3ds");
    Model.Load("./3ds_files/test6.3ds");

    int m_num = Model.GetObjNum();
    cout << "Obj num is " << m_num << std::endl;
    OBvec = Model.GetOB();
    Vertex_array = new glm::vec4 *[m_num];      // A Obj has a vertex array

    for (int i = 0; i < m_num; ++i) {
        ModelObj *MObj = OBvec.at(i);
        float *vb = MObj->GetVB();
        int v_num = MObj->GetVertexNum();
        Vertex_array[i] = new glm::vec4 [v_num];
        for (int j = 0; j < v_num; ++j) {
            glm::vec4 v_vec(vb[j * 3], vb[j * 3 + 1], vb[j * 3 + 2], 1.0);
            Vertex_array[i][j] = trans * v_vec;
        }
    }
}

void end() {
    for (int i = 0; i < Model.GetObjNum(); ++i)
        delete [] Vertex_array[i];
    delete [] Vertex_array;
}

#define BAR(str, now, total) do { \
        cout << str << " " << now << "/" << total << endl; \
    }while (0);

int main(int argc, char const *argv[]) {
    init();

    int m_num = Model.GetObjNum();

    for (int i = 0; i < m_num; ++i) {
        BAR("Object:", i, m_num)

        ModelObj *MObj = OBvec.at(i);

        int f_num = MObj->GetFaceNum();
        unsigned short *ib = MObj->GetIB();
        float z_min = 1000.0, z_max = -1000.0;
        for (int j = 0; j < f_num; ++j) {
            BAR("Face:", j, f_num)
            glm::vec4 V0, V1, V2;
            V0 = Vertex_array[i][ib[j * 3]];
            V1 = Vertex_array[i][ib[j * 3 + 1]];
            V2 = Vertex_array[i][ib[j * 3 + 2]];

            // find the quation of face-plane
            glm::mat3 XYZ3 = glm::mat3(V0.xyz(), V1.xyz(), V2.xyz());
            XYZ3 = glm::inverse(XYZ3);
            glm::vec3 ABC = glm::vec3(1.0, 1.0, 1.0) * XYZ3;

            // find bounding box
            float v0_x, v0_y, v1_x, v1_y, v2_x, v2_y;
            v0_x = - V0.x / V0.z; v0_y = - V0.y / V0.z;
            v1_x = - V1.x / V1.z; v1_y = - V1.y / V1.z;
            v2_x = - V2.x / V2.z; v2_y = - V2.y / V2.z;
            unsigned min_i = max(0, X2I(min(min(v0_x, v1_x), v2_x)));
            unsigned min_j = max(0, Y2J(max(max(v0_y, v1_y), v2_y)));  // y axis
            unsigned max_i = min(WIDTH, X2I(max(max(v0_x, v1_x), v2_x)));
            unsigned max_j = min(HEIGHT, Y2J(min(min(v0_y, v1_y), v2_y)));

            // for (int ii = 0; ii < WIDTH; ++ii) {
                // for (int jj = 0; jj < HEIGHT; ++jj) {
            for (int ii = min_i-2; ii < max_i+2; ++ii) {
                for (int jj = min_j-2; jj < max_j+2; ++jj) {
                    float x = ((float)(ii - (WIDTH / 2))) / (WIDTH / 2);
                    float y = ((float)((HEIGHT / 2) - jj)) / (WIDTH / 2);
                    glm::vec2 D(x, y);
                    if (in_tri(V0, V1, V2, D)) {
                        float z_v = z_value(x, y, ABC);
                        z_min = min(z_min, z_v); z_max = max(z_max, z_v);
                        if (z_v > Buff.z_buff(ii, jj)) {
                            // float n = ((-z_v) - 3);
                            // float f_v = 20 + 64 * (tanh(n/2.0));
                            float n = ((-z_v) - 3.5) / 15;
                            float f_v = 50 + 400 * n;
                            Buff.update(ii, jj, (unsigned char)f_v, z_v);
                        }
                    }
                }
            }
        }
        cout << z_max << " " << z_min << endl;
    }

    Buff.svimg();
    end();
    return 0;
}