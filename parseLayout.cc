
#include <vector>
#include <assert.h>

#include "parseLayout.h"
#include "png_helper.h"

#include <iostream>

static const double HEIGHT = 200;


using namespace std;

vector<Rectangle> parseLayout(const char* const filename) {
    static const Vector3 wallColor  = createVector3(0.8, 0.8, 0.8);
    static const Vector3 windowColor= createVector3(18, 16, 12);
    vector<Rectangle> segments;
    int width, height, color_type;
    uint32_t *pixel_buffer;
    read_png_file(filename, &width, &height, &color_type, (uint8_t**)&pixel_buffer);
    cout << "read image of size " << width << "x" << height << " with color_type " << color_type << endl;
    assert (color_type == PNG_COLOR_TYPE_RGBA);
    
    //segments.push_back(WallSegment( 0, 0, 2, 2));
    static const uint32_t BLACK = 0xFF000000;
    //static const uint32_t WHITE = 0xFFFFFFFF;
    static const uint32_t GRAY =  0xFF808080;
    static const uint32_t GREEN = 0xFF00FF00;
    

    /*for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            if (pixel_buffer[y * width + x] != BLACK && 
                pixel_buffer[y * width + x] != WHITE &&
                pixel_buffer[y * width + x] != GREEN &&
                pixel_buffer[y * width + x] != GRAY)
                cout << hex << pixel_buffer[y * width + x] << endl;
    */

    for (int y = 1; y < height; y++)
        for (int x = 1; x < width; x++) {
            if (pixel_buffer[(y-1) * width + (x)] != BLACK && pixel_buffer[y * width + x] == BLACK)
            {
                int start_x = x;
                while (x < width && 
                       pixel_buffer[(y-1) * width + x] != BLACK && 
                       pixel_buffer[ y    * width + x] == BLACK)
                    x++;
                segments.push_back(createRectangleWithColor( createVector3(x,y,0), createVector3(start_x-x,0,0), createVector3(0,0,HEIGHT), wallColor));        
//                segments.push_back(WallSegment( x, y, start_x, y));
            }

            if (pixel_buffer[(y-1) * width + (x)] == GRAY && pixel_buffer[y * width + x] == GREEN)
            {
                int start_x = x;
                while (x < width && 
                       pixel_buffer[(y-1) * width + x] == GRAY && 
                       pixel_buffer[ y    * width + x] == GREEN)
                    x++;
                segments.push_back(createRectangleWithColor( createVector3(start_x,y,0),  createVector3(x-start_x,0,0), createVector3(0,0,70), wallColor ));
                segments.push_back(createRectangleWithColor( createVector3(start_x,y,70), createVector3(x-start_x,0,0), createVector3(0,0,180-70), windowColor));
                segments.push_back(createRectangleWithColor( createVector3(start_x,y,180),createVector3(x-start_x,0,0), createVector3(0,0,HEIGHT-180), wallColor));


//                segments.push_back(WallSegment( x, y, start_x, y));
            }


            
            if (pixel_buffer[(y-1) * width + (x)] == BLACK && pixel_buffer[y * width + x] != BLACK)
            {
                int start_x = x;
                while (x < width && 
                       pixel_buffer[(y-1) * width + x] == BLACK && 
                       pixel_buffer[ y    * width + x] != BLACK)
                    x++;
                    
                segments.push_back(createRectangleWithColor( createVector3(start_x,y,0), createVector3(x-start_x,0,0), createVector3(0,0,HEIGHT), wallColor));
                //segments.push_back(WallSegment( start_x, y, x, y));
            }
            
            if (pixel_buffer[(y-1) * width + (x)] == GREEN && pixel_buffer[y * width + x] == GRAY)
            {
                int start_x = x;
                while (x < width && 
                       pixel_buffer[(y-1) * width + x] == GREEN && 
                       pixel_buffer[ y    * width + x] == GRAY)
                    x++;
                    
                segments.push_back(createRectangleWithColor( createVector3(x,y,0), createVector3(start_x-x,0,0), createVector3(0,0,70), wallColor));
                segments.push_back(createRectangleWithColor( createVector3(x,y,70), createVector3(start_x-x,0,0), createVector3(0,0,180-70), windowColor));
                segments.push_back(createRectangleWithColor( createVector3(x,y,180), createVector3(start_x-x,0,0), createVector3(0,0,HEIGHT-180), wallColor));
                //segments.push_back(WallSegment( start_x, y, x, y));
            }            
        }


    for (int x = 1; x < width; x++) 
        for (int y = 1; y < height; y++) {
            if (pixel_buffer[y * width + (x-1)] != BLACK && pixel_buffer[y * width + x] == BLACK)
            {
                int start_y = y;
                while (y < height && 
                       pixel_buffer[y * width + (x-1)] != BLACK && 
                       pixel_buffer[y * width +  x   ] == BLACK)
                    y++;
                
                segments.push_back(createRectangleWithColor( createVector3(x,start_y,0), createVector3(0,y-start_y,0), createVector3(0,0,HEIGHT), wallColor));
                //segments.push_back(WallSegment( x, start_y , x, y));
            }
            
            if (pixel_buffer[y * width + (x-1)] == BLACK && pixel_buffer[y * width + x] != BLACK)
            {
                int start_y = y;
                while (y < height && 
                       pixel_buffer[y * width + (x-1)] == BLACK && 
                       pixel_buffer[y * width +  x   ] != BLACK)
                    y++;
                
                segments.push_back(createRectangleWithColor( createVector3(x,y,0), createVector3(0,start_y-y,0), createVector3(0,0,HEIGHT), wallColor));
                //segments.push_back(WallSegment( x, y, x, start_y));
            }
        
        }   
     
    free (pixel_buffer);
    return segments;
}

