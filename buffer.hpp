
#ifndef BUFFER_H
#define BUFFER_H

#include "main.hpp"


class Buffer
{
public:
    Buffer();
    ~Buffer();
    float z_buff(int i, int j);
    float f_buff(int i, int j);
    void update(int i, int j, unsigned char new_f, float new_z);
    void svimg();

private:
    unsigned char FB[WIDTH][HEIGHT];
    float ZB[WIDTH][HEIGHT];
};

Buffer::Buffer() {
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < HEIGHT; ++j) {
            this->FB[i][j] = 0x8F;
            this->ZB[i][j] = -1000.0;
        }
    }
}
Buffer::~Buffer() { }

float Buffer::z_buff(int i, int j) {
    return this->ZB[i][j];
}

float Buffer::f_buff(int i, int j) {
    return this->FB[i][j];
}

void Buffer::update(int i, int j, unsigned char new_f, float new_z) {
    this->FB[i][j] = new_f;
    this->ZB[i][j] = new_z;
}

void Buffer::svimg() {
    FILE *f = fopen("outputs/out.ppm", "wb");
    fprintf(f, "P5\n%i %i 255\n", WIDTH, HEIGHT);
    for (int jj = 0; jj < HEIGHT; jj++)
        for (int ii = 0; ii < WIDTH; ii++)
            fputc(this->FB[ii][jj], f);
    fclose(f);
}


#endif

