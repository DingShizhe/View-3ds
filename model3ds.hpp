#ifndef MODEL_3DS
#define MODEL_3DS

#include <iosfwd>
#include <fstream>

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

#define MAINCHUNK_3DS        0x4D4D
#define MASHDATA_3DS         0x3D3D //3D Editor Chunk
#define OBJNAME_3DS          0x4000 //Object Block
#define MESH_DATA_3DS        0x4100 //Triangular Mesh
#define VERTEX_DATA_3DS      0x4110 //Vertices List
#define INDEX_DATA_3DS       0x4120 //Faces Description
#define TEXMAP_DATA_3DS      0x4140 //Mapping Coordinates List

struct SVertex3DS {
    union {
        float v[3];
        struct {
            float X;
            float Y;
            float Z;
        };
    };
};

struct SIndex3DS {
    union {
        unsigned short i[3];
        struct {
            unsigned short p1;
            unsigned short p2;
            unsigned short p3;
        };
    };
};


class ModelObj
{
public:
    ModelObj();
    ~ModelObj();
    bool LoadVertex(ifstream &f, unsigned short v_num);
    bool LoadFace(ifstream &f, unsigned short f_num);
    inline unsigned short* GetIB() {return (unsigned short*)m_pIBbuff;}
    inline float* GetVB() {return (float*)m_pVBbuff;}
    inline short GetVertexNum() {return m_VertexNum;}
    inline unsigned short GetFaceNum() {return m_FaceNum;}
private:
    SIndex3DS* m_pIBbuff;
    SVertex3DS* m_pVBbuff;
    unsigned short m_VertexNum;
    unsigned short m_FaceNum;
};

ModelObj::ModelObj(void): m_pIBbuff(0), m_pVBbuff(0), m_VertexNum(0) {}

ModelObj::~ModelObj(void) {
    if (m_pIBbuff)
        delete [] m_pIBbuff;
    if (m_pVBbuff)
        delete [] m_pVBbuff;
}

bool ModelObj::LoadVertex(ifstream &f, unsigned short v_num) {
    m_VertexNum = v_num;
    m_pVBbuff = new SVertex3DS[m_VertexNum];
    for (int i = 0; i < m_VertexNum; ++i) {
        f.read((char*) & (m_pVBbuff[i].X), sizeof(float));
        f.read((char*) & (m_pVBbuff[i].Y), sizeof(float));
        f.read((char*) & (m_pVBbuff[i].Z), sizeof(float));
    }
    return true;
}

bool ModelObj::LoadFace(ifstream &f, unsigned short f_num) {
    m_FaceNum = f_num;
    m_pIBbuff = new SIndex3DS[m_FaceNum];
    for (int i = 0; i < m_FaceNum; ++i) {
        f.read((char*)&m_pIBbuff[i].p1, 2);
        f.read((char*)&m_pIBbuff[i].p2, 2);
        f.read((char*)&m_pIBbuff[i].p3, 2);
        f.seekg(2, ios_base::cur);
    }
    return true;
}



class Model3ds {
public:
    Model3ds(void);
    ~Model3ds(void);

    bool Load(const char* fileName);
    inline unsigned short GetObjNum() {return m_pModelObj.size();}
    inline vector<ModelObj *> GetOB() {return m_pModelObj;}

private:
    vector<ModelObj *> m_pModelObj;
};

Model3ds::Model3ds(void) { }
Model3ds::~Model3ds(void) {
    while (!m_pModelObj.empty()) {
        ModelObj *tmp = m_pModelObj.back();
        m_pModelObj.pop_back();
        delete tmp;
    }
}

bool Model3ds::Load(const char* fileName ) {
    ifstream f;
    f.open(fileName, ios::in | ios::binary);
    ModelObj *MObj = NULL;

    short chunk_id = 0;
    unsigned int chunk_len = 0;
    char a;
    while (!f.eof() && !(f.tellg() == -1)) {
        cout << "Loading...  " << f.tellg() << endl;
        f.read((char*)&chunk_id, 2);
        f.read((char*)&chunk_len, 4);
        switch (chunk_id) {
        case MAINCHUNK_3DS:
            cout << "MAINCHUNK_3DS" << endl;
            break;
        case MASHDATA_3DS:
            cout << "MASHDATA_3DS" << endl;
            break;
        case OBJNAME_3DS:
            cout << "OBJNAME_3DS" << endl;
            {
                int i = 0;
                char c;
                do {
                    f.read(&c, 1);
                    i++;
                } while (c != '\0' && i < 20);
            }

            break;
        case MESH_DATA_3DS:
            cout << "MESH_DATA_3DS" << endl;
            break;
        case VERTEX_DATA_3DS:
            cout << "VERTEX_DATA_3DS" << endl;
            short m_VertexNum;
            f.read((char*)&m_VertexNum, 2);
            cout << "VERTEX_NUM: " << m_VertexNum << endl;
            if (m_VertexNum <= 0) return false;

            MObj = new ModelObj;
            MObj->LoadVertex(f, m_VertexNum);
            m_pModelObj.push_back(MObj);
            // cin.get();
            break;
        case INDEX_DATA_3DS:
            cout << "INDEX_DATA_3DS" << endl;
            short m_FaceNum;
            f.read((char*)&m_FaceNum, 2);
            cout << "FACE_NUM: " << m_FaceNum << endl;
            if (m_FaceNum <= 0) return false;

            MObj->LoadFace(f, m_FaceNum);
            // cin.get();

            break;
        case TEXMAP_DATA_3DS:
            cout << "TEXMAP_DATA_3DS" << endl;
            break;
        default:
            cout << "default" << endl;
            f.seekg(chunk_len - 6, ios_base::cur);
        }
    }

    f.close();
    return true;
}

#endif