
/*******************************************************************************
* image
* filename: unsaved
* name: icon_moonstars
*
* preset name: E-Ink
* data block size: 8 bit(s), uint8_t
* RLE compression enabled: no
* conversion type: Monochrome, Edge 220
* bits per pixel: 1
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: yes
*******************************************************************************/

/*
 typedef struct {
     const uint8_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;
*/
#include <stdint.h>
#include <pgmspace.h>



static const uint8_t image_data_icon_moonstars[512] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x03, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x06, 0x38, 0x00, 0x00, 0xf0, 0x00, 0x00, 
    0x00, 0x3c, 0x1f, 0x00, 0x00, 0xfe, 0x00, 0x00, 
    0x00, 0xf8, 0x0f, 0xc0, 0x00, 0xff, 0x00, 0x00, 
    0x00, 0x3c, 0x0f, 0x00, 0x60, 0x7f, 0xc0, 0x00, 
    0x00, 0x0e, 0x18, 0x00, 0x60, 0x7f, 0xe0, 0x00, 
    0x00, 0x03, 0x30, 0x00, 0x60, 0x3d, 0xf0, 0x00, 
    0x00, 0x03, 0xe0, 0x00, 0xf0, 0x3c, 0xf8, 0x00, 
    0x00, 0x01, 0xc0, 0x03, 0x9c, 0x1c, 0x78, 0x00, 
    0x00, 0x00, 0xc0, 0x0f, 0x0f, 0x1e, 0x3c, 0x00, 
    0x00, 0x00, 0xc0, 0x03, 0x9c, 0x1e, 0x1e, 0x00, 
    0x00, 0x00, 0xc1, 0x00, 0xf0, 0x1e, 0x0e, 0x00, 
    0x00, 0x00, 0x81, 0x80, 0x60, 0x0e, 0x0e, 0x00, 
    0x00, 0x00, 0x01, 0x80, 0x60, 0x0e, 0x07, 0x00, 
    0x00, 0x00, 0x01, 0x80, 0x60, 0x0e, 0x07, 0x00, 
    0x00, 0x00, 0x03, 0xc0, 0x00, 0x0e, 0x07, 0x00, 
    0x00, 0x00, 0x06, 0x60, 0x00, 0x1e, 0x03, 0x80, 
    0x00, 0x00, 0x0c, 0x70, 0x00, 0x1e, 0x03, 0x80, 
    0x00, 0x00, 0xf8, 0x1e, 0x00, 0x1c, 0x03, 0x80, 
    0x00, 0x00, 0xf8, 0x1e, 0x00, 0x1c, 0x03, 0x80, 
    0x00, 0x00, 0x0c, 0x70, 0x00, 0x3c, 0x03, 0x80, 
    0x00, 0x00, 0x06, 0x60, 0x00, 0x38, 0x03, 0x80, 
    0x00, 0x00, 0x03, 0xc0, 0x00, 0x78, 0x03, 0x80, 
    0x00, 0x00, 0x01, 0x80, 0x00, 0xf0, 0x03, 0x80, 
    0x00, 0x00, 0x01, 0x80, 0x00, 0xf0, 0x07, 0x00, 
    0x00, 0x00, 0x01, 0x80, 0x01, 0xe0, 0x07, 0x00, 
    0x00, 0x00, 0x01, 0x00, 0x03, 0xc0, 0x07, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x0e, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x0e, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x1c, 0x00, 
    0x00, 0x00, 0x03, 0xff, 0xfc, 0x00, 0x3c, 0x00, 
    0x00, 0x00, 0x03, 0xff, 0xf0, 0x00, 0x78, 0x00, 
    0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0xf0, 0x00, 
    0x00, 0x00, 0x01, 0xfc, 0x00, 0x01, 0xe0, 0x00, 
    0x00, 0x00, 0x00, 0xfc, 0x00, 0x07, 0xc0, 0x00, 
    0x00, 0x00, 0x00, 0x3f, 0x00, 0x1f, 0x80, 0x00, 
    0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x07, 0xff, 0xfc, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xff, 0xe0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const tImage icon_moonstars = { image_data_icon_moonstars, 64, 64,
    8 };

