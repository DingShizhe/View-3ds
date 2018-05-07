#include "main.hpp"
#include "model3ds.hpp"
#include "util.hpp"
#include "buffer.hpp"

#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <cstdio>
#include <cassert>
#include <cmath>

Model3ds Model;
Buffer Buff;
vector <ModelObj *> OBvec;
glm::vec3 **V_array;
glm::vec2 **prjV_array;

glm::vec3 view_p(2.0, 3.0, 4.0);
glm::vec3 model_p(0.0, 0.0, 0.0);
glm::vec3 lamp_p(4.0, 3.0, 2.0);
float lamp_intsty = 1.0;

void init(const char *model_f) {
    glm::mat4 trans = lookAt(view_p, model_p, glm::vec3(0.0, 0.0, 1.0));
    Model.Load(model_f);

    int m_num = Model.GetObjNum();
    cout << "Obj num is " << m_num << std::endl;
    OBvec = Model.GetOB();
    V_array = new glm::vec3 *[m_num];      // A Obj has a vertex array
    prjV_array = new glm::vec2 *[m_num];      // A Obj has a vertex array

    for (int i = 0; i < m_num; ++i) {
        ModelObj *MObj = OBvec.at(i);
        float *vb = MObj->GetVB();
        int v_num = MObj->GetVertexNum();
        V_array[i] = new glm::vec3 [v_num];
        prjV_array[i] = new glm::vec2 [v_num];
        for (int j = 0; j < v_num; ++j) {
            glm::vec4 tmp(vb[j * 3], vb[j * 3 + 1], vb[j * 3 + 2], 1.0);
            tmp = trans * tmp;
            glm::vec3 v_vec = tmp.xyz() / tmp.w;
            V_array[i][j] = v_vec;
            prjV_array[i][j] = - v_vec.xy() / v_vec.z;
        }
    }
}

void end() {
    for (int i = 0; i < Model.GetObjNum(); ++i)
        delete [] V_array[i];
    delete [] V_array;
}


int main(int argc, char const *argv[]) {
    // assert(argc > 1);
    const char *model_f = argv[1];
    init(model_f);

    int m_num = Model.GetObjNum();

    for (int i = 0; i < m_num; ++i) {
        LOG("Object:", i, m_num)

        ModelObj *MObj = OBvec.at(i);

        int f_num = MObj->GetFaceNum();
        unsigned short *ib = MObj->GetIB();
        // float z_min = 1000.0, z_max = -1000.0;
        for (int j = 0; j < f_num; ++j) {
            LOG("Face:", j, f_num)
            // get face' 3 vertex by index j
            glm::vec3 V0, V1, V2;
            get_from_index<glm::vec3>(
                V0, V1, V2, V_array, i, ib, j);
            glm::vec2 prjV0, prjV1, prjV2;
            get_from_index<glm::vec2>(
                prjV0, prjV1, prjV2, prjV_array, i, ib, j);

            // find the quation of face-plane
            // A*x + B*y + C*z = 1.0
            glm::mat3 XYZ3 = glm::mat3(V0, V1, V2);
            XYZ3 = glm::inverse(XYZ3);
            glm::vec3 ABC = glm::vec3(1.0, 1.0, 1.0) * XYZ3;

            // find bounding box
            // glm::vec2 proj_V0
            unsigned i_min, i_max, j_min, j_max;
            get_bound_box(prjV0, prjV1, prjV2, i_min, i_max, j_min, j_max);

            // for (int ii = 0; ii < WIDTH; ++ii) {
            // for (int jj = 0; jj < HEIGHT; ++jj) {
            for (int ii = i_min - 2; ii < i_max + 2; ++ii) {
                for (int jj = j_min - 2; jj < j_max + 2; ++jj) {
                    float x = ((float)(ii - (WIDTH / 2))) / (WIDTH / 2);
                    float y = ((float)((HEIGHT / 2) - jj)) / (WIDTH / 2);
                    glm::vec2 D(x, y);
                    glm::vec3 P;
                    screen_to_face(D, ABC, P);

                    if (in_tri(V0, V1, V2, D)) {
                        float z_v = z_value(P);
                        // z_min = min(z_min, z_v); z_max = max(z_max, z_v);
                        if (z_v > Buff.z_buff(ii, jj)) {
                        #ifdef PHONG
                            float f_v = f_value (
                                lamp_intsty,
                                255.0,
                                lamp_p,
                                view_p,
                                P,
                                ABC
                            );
                        #else
                            float f_v = f_value(z_v);
                        #endif
                            Buff.update(ii, jj, (unsigned char)f_v, z_v);
                        }
                    }
                }
            }
        }
        // cout << z_max << " " << z_min << endl;
    }

    if (argc == 4 && strcmp(argv[2], "-o") == 0)
        Buff.svimg(argv[3]);
    else
        Buff.svimg("out.png");

    end();
    return 0;
}
