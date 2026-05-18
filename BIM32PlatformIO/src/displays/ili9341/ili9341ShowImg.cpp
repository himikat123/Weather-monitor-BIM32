#include "./ili9341.hpp"

#include <JPEGDecoder.h> // v2.0.0 https://github.com/Bodmer/JPEGDecoder

void ILI9341::_showImg(uint16_t x, uint16_t y, const byte img[], uint16_t size) {
    JpegDec.decodeArray(img, size);
    uint16_t *pImg;
    uint16_t mcu_w = JpegDec.MCUWidth;
    uint16_t mcu_h = JpegDec.MCUHeight;
    uint32_t max_x = JpegDec.width;
    uint32_t max_y = JpegDec.height;
    uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
    uint32_t min_h = minimum(mcu_h, max_y % mcu_h);
    uint32_t win_w = mcu_w;
    uint32_t win_h = mcu_h;
    max_x += x;
    max_y += y;

    while(JpegDec.readSwappedBytes()) {
        pImg = JpegDec.pImage;
        int mcu_x = JpegDec.MCUx * mcu_w + x;
        int mcu_y = JpegDec.MCUy * mcu_h + y;
        if(mcu_x + mcu_w <= max_x) win_w = mcu_w;
        else win_w = min_w;
        if(mcu_y + mcu_h <= max_y) win_h = mcu_h;
        else win_h = min_h;
        if(win_w != mcu_w) {
            uint16_t *cImg;
            int p = 0;
            cImg = pImg + win_w;
            for(int h=1; h<win_h; h++) {
                p += mcu_w;
                for(int w=0; w<win_w; w++) {
                    *cImg = *(pImg + w + p);
                    cImg++;
                }
            }
        }

        if((mcu_x + win_w) <= tft.width() && (mcu_y + win_h) <= tft.height()) {
            tft.pushRect(mcu_x, mcu_y, win_w, win_h, pImg);
        }
        else if((mcu_y + win_h) >= tft.height()) JpegDec.abort();
    }
}