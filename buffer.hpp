
#ifndef BUFFER_H
#define BUFFER_H

#include "main.hpp"
#include <cstring>

class Buffer
{
public:
    Buffer();
    ~Buffer();
    float z_buff(int i, int j);
    float f_buff(int i, int j);
    void update(int i, int j, unsigned char new_f, float new_z);
    void svimg(const char *f_name);

private:
    unsigned char FB[HEIGHT][WIDTH];
    float ZB[HEIGHT][WIDTH];
};

Buffer::Buffer() {
    for (int j = 0; j < HEIGHT; ++j) {
        for (int i = 0; i < WIDTH; ++i) {
            this->FB[j][i] = 0x8F;
            this->ZB[j][i] = -1000.0;
        }
    }
}
Buffer::~Buffer() { }

float Buffer::z_buff(int i, int j) {
    return this->ZB[j][i];
}

float Buffer::f_buff(int i, int j) {
    return this->FB[j][i];
}

void Buffer::update(int i, int j, unsigned char new_f, float new_z) {
    this->FB[j][i] = new_f;
    this->ZB[j][i] = new_z;
}

void Buffer::svimg(const char *f_name) {
    FILE *f = fopen(f_name, "wb");
    fprintf(f, "P5\n%i %i 255\n", WIDTH, HEIGHT);
    for (int jj = 0; jj < HEIGHT; jj++)
        for (int ii = 0; ii < WIDTH; ii++)
            fputc(this->FB[jj][ii], f);
    fclose(f);

    char cmd[30] = {0};
    strcat(cmd, "convert ");
    strcat(cmd, f_name);
    strcat(cmd, " ");
    strcat(cmd, f_name);
    system(cmd);
}


#endif

