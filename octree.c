#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _Octree Octree;

struct _Octree {
    Octree *children[8];
    int lod;
    int octant;
};

Octree *new_Octree(int lod, int octant)
{
    Octree *octree = (Octree*)malloc(sizeof(Octree));
    memset((void*)octree->children, 0, sizeof(Octree*) * 8);
    octree->lod    = lod;
    octree->octant = octant;
    return octree;
}

void free_Octree(Octree *octree)
{
    for (int i = 0; i < 8 && octree->children[i] != NULL; ++i)
        free_Octree(octree->children[i]);
    free(octree);
}

float variance(float *density_grid, int xMin, int xMax, int yMin, int yMax, int zMin, int zMax, int width, int height, int depth)
{
    int   vWidth  = xMax - xMin;
    int   vHeight = yMax - yMin;
    int   vDepth  = zMax - zMin;
    float total   = (float)(vWidth * vHeight * vDepth);
    float sum = 0.0;
    for (int z = zMin; z < zMax; ++z)
        for (int y = yMin; y < yMax; ++y)
            for (int x = xMin; x < xMax; ++x)
            {
                int index = z * width * height + y * width + x;
                sum += density_grid[index];
            }
    float mean = sum / total;
    sum = 0.0;
    for (int z = zMin; z < zMax; ++z)
        for (int y = yMin; y < yMax; ++y)
            for (int x = xMin; x < xMax; ++x)
            {
                int index = z * width * height + y * width + x;
                float diff = density_grid[index] - mean;
                sum += (diff * diff);
            }
    float variance = sum / total;
    printf("Mean: %f Variance: %f\n", mean, variance);
    return variance;
}

int main(void)
{
    int   width   = 512;
    int   height  = 512;
    int   depth   = 512;
    float *density_grid = (float*)malloc(sizeof(float) * width * height * depth);
    FILE *f = fopen("grid.bin", "rb");
    fread((void*)density_grid, sizeof(float), width * height * depth, f);
    fclose(f);

    float variance = 0.0;
    Octree *root = new_Octree(0, 0);
    free_Octree(root);

    free(density_grid);
    return 0;
}
