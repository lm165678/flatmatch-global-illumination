#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif


#include "vector3_cl.h"
//#include "color3.h"
#include "assert.h"

static const float TILE_SIZE = 1000/20.0f*0.1;   //lightmap texels per m²
static const int   SUPER_SAMPLING = 1;


/* Rectangle structure to be passed to OpenCL
 * - the alignment is required by OpenCL
 * - this struct should be as compact as possible to save vector registers on AMD OpenCL 
 */
typedef struct __attribute__ ((aligned(16))) Rectangle{
    Vector3 pos, width, height, n;
//    Vector3 color;
    cl_int3 lightmapSetup; // [0] = lightBaseIdx, [1] = tiles_width, [2] = tiles_height
//    int lightNumTiles;
//    int hNumTiles, vNumTiles;
//    float 
} Rectangle;


Rectangle createRectangleV( const Vector3 _pos, const Vector3 _width, const Vector3 _height);
Rectangle createRectangle( float px, float py, float pz,
                           float wx, float wy, float wz,
                           float hx, float hy, float hz);


//Methods for the 'Rectangle' struct
int getPosition(const Rectangle *plane, const Rectangle *rect);
double getDistance(const Rectangle *plane, const Vector3 p);

float intersects( const Rectangle *rect, Vector3 ray_src, Vector3 ray_dir, float closestDist);
float distanceToPlane( Vector3 planeNormal, Vector3 planePos, Vector3 ray_src, Vector3 ray_dir);
int getNumTiles(const Rectangle *rect);
float getArea(const Rectangle *rect);
int getTileIdAt(const Rectangle *rect, const Vector3 p);

Vector3 getDiffuseColor(const Rectangle *rect, const Vector3 pos);
Vector3 getOrigin(const Rectangle *rect);
Vector3 getWidthVector(const Rectangle *rect);
Vector3 getHeightVector(const Rectangle *rect);
Vector3 getTileCenter(const Rectangle *rect, int tileId);
void saveAs(const Rectangle *rect, const char *filename, const Vector3 *lights, int tintExtra);
int saveAsMemoryPng(const Rectangle *rect, const Vector3 *lights, int tintExtra, uint8_t**data);
char* saveAsBase64Png(const Rectangle *rect, const Vector3 *lights, int tintExtra);
char* base64_encode(const unsigned char *data, size_t input_length);

void saveAsRaw(const Rectangle *rect, const char *filename, const Vector3 *lights);

#ifdef __cplusplus
}
#endif


#endif
