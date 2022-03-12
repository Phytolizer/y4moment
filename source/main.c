#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define FRAME_WIDTH 800LU
#define FRAME_HEIGHT 600LU
#define FRAME_RATE 60LU
#define FRAME_SIZE (FRAME_WIDTH * FRAME_HEIGHT)
#define VIDEO_LENGTH 1.0

typedef struct {
    double r;
    double g;
    double b;
} rgb_t;

typedef struct {
    double y;
    double cb;
    double cr;
} ycbcr_t;

static ycbcr_t rgb_to_ycbcr(rgb_t c) {
    double y = 16.0 + 65.738 * c.r / 256.0 + 129.057 * c.g / 256.0 +
               25.064 * c.b / 256.0;
    double cb = 128.0 - 37.945 * c.r / 256.0 - 74.494 * c.g / 256.0 +
                112.439 * c.b / 256.0;
    double cr = 128.0 + 112.439 * c.r / 256.0 - 94.154 * c.g / 256.0 -
                18.285 * c.b / 256.0;
    return (ycbcr_t){y, cb, cr};
}

static void dump_frame(FILE* fp, ycbcr_t color) {
    fprintf(fp, "FRAME\n");
    for (size_t i = 0; i < FRAME_SIZE; i += 1) {
        fputc((int)color.y, fp);
    }
    for (size_t i = 0; i < FRAME_SIZE; i += 1) {
        fputc((int)color.cb, fp);
    }
    for (size_t i = 0; i < FRAME_SIZE; i += 1) {
        fputc((int)color.cr, fp);
    }
}

int main(void) {
    FILE* fp = fopen("output.y4m", "wb");
    // file signature
    fprintf(fp, "YUV4MPEG2");
    fprintf(fp, " W%lu", FRAME_WIDTH);
    fprintf(fp, " H%lu", FRAME_HEIGHT);
    fprintf(fp, " F%lu:1", FRAME_RATE);
    fprintf(fp, " Ip");
    fprintf(fp, " A1:1");
    fprintf(fp, " C444\n");
    ycbcr_t red = rgb_to_ycbcr((rgb_t){255.0, 0.0, 0.0});
    for (size_t i = 0; i < (size_t)(VIDEO_LENGTH * FRAME_RATE); i += 1) {
        printf("FRAME %zu / %zu\n", i, (size_t)(VIDEO_LENGTH * FRAME_RATE));
        dump_frame(fp, red);
    }
    fclose(fp);
}
