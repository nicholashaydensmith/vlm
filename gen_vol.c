#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void load_bar(int progress, int total)
{
    static int percent_change = 0;
    int bar_size = 100;
    float percent_progress = (float)progress / (float)total;
    int bar_progress = (int)(percent_progress * bar_size);
    if (bar_progress == percent_change)
        return;
    percent_change = bar_progress;
    printf("\r[");
    for (int i = 0; i < bar_size; ++i)
        printf("%c", (i < bar_progress) ? '#' : ' ');
    printf("] %d%% ", (int)(percent_progress * 100));
    fflush(stdout);
    if (bar_progress == bar_size)
        printf("\n");
}

int main(void)
{
    int   width   = 512;
    int   height  = 512;
    int   depth   = 512;
    float *density_grid = (float*)malloc(sizeof(float) * width * height * depth);

    for (int z = 0; z < depth; ++z)
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
            {
                float density = (((float)x / (float)width) + ((float)y / (float)height) + ((float)z / (float)depth)) / 3.0;
                int   index   = z * width * height + y * width + x;
                density_grid[index] = ( x > (width  / 3) && (width  - x) < (width  / 20) &&
                                        y > (height / 3) && (height - y) < (height / 20) &&
                                        z > (depth  / 3) && (depth  - z) < (depth  / 20)    )
                                      ? density : 0.0;
                load_bar(index, width * height * depth);
            }

    FILE *f = fopen("grid.bin", "wb");
    fwrite((void*)density_grid, sizeof(float), width * height * depth, f);
    fclose(f);
    free(density_grid);

    return 0;
}
