#include <stdio.h>
#include <stdlib.h>
static int SEED = 65536;
static int SIZE = 16;
static int hash[] = {};
double get_random() {
    return ((double)rand() / (double)RAND_MAX);
}
void gen_hash() {
    srand(SEED * SEED);
    for (int i = 0; i < 256; ++i) {
        hash[i] = get_random() * 255;
    }
}
int noise2(int x, int y)
{
    int tmp = hash[(y + SEED) % 256];
    return hash[(tmp + x) % 256];
}
float lin_inter(float x, float y, float s)
{
    return x + s * (y-x);
}
float smooth_inter(float x, float y, float s)
{
    return lin_inter(x, y, s * s * (3-2*s));
}
float noise2d(float x, float y)
{
    int x_int = x;
    int y_int = y;
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int s = noise2(x_int, y_int);
    int t = noise2(x_int+1, y_int);
    int u = noise2(x_int, y_int+1);
    int v = noise2(x_int+1, y_int+1);
    float low = smooth_inter(s, t, x_frac);
    float high = smooth_inter(u, v, x_frac);
    return smooth_inter(low, high, y_frac);
}
float perlin2d(float x, float y, float freq, int depth)
{
    float xa = x*freq;
    float ya = y*freq;
    float amp = 1.0;
    float fin = 0;
    float div = 0.0;
    int i;
    for(i=0; i<depth; i++)
    {
        div += 256 * amp;
        fin += noise2d(xa, ya) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }
    return fin/div;
}
int main(int argc, char *argv[])
{
    int x, y;
    gen_hash();
    for(y=0; y<SIZE; y++) {
        for(x=0; x<SIZE; x++) {
            printf("%.0f ",perlin2d(x, y, 0.1, 4) * 100);
        }
        printf("\n"); 
    }
    return 0;
}
