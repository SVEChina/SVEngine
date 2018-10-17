//
// SVPngWriter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPngWriter.h"
#include <png.h>

SVPngWriter::SVPngWriter(SVInst *_app) : SVFileWriter(_app) {

}

bool SVPngWriter::write(cptr8 fname, s32 pngw, s32 pngh, void *data) {
    FILE *fp = nullptr;
//    png_colorp调色板
    fp = fopen(fname, "wb");
    if (fp == nullptr)
        return false;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        fclose(fp);
        return false;
    }
    //分配内存并初始化图像信息数据(必要)
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return false;
    }
//    / *设置错误处理。如果你在调用png_create_write_struct（）时没
//    *有设置错误处理函数，那么这段代码是必须写的。* /
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }
//    / *设置输出控制，如果你使用的是C的标准I / O流*
    png_init_io(png_ptr, fp);
//      / *（必需）在这里设置图像的信息，宽度，高度的上限是2 ^ 31。
//    * bit_depth取值必需是1,2,4,8或者16，但是可以的值也依赖于color_type。
//    * color_type可选值有：PNG_COLOR_TYPE_GRAY，PNG_COLOR_TYPE_GRAY_ALPHA，
//    * PNG_COLOR_TYPE_PALETTE，PNG_COLOR_TYPE_RGB，PNG_COLOR_TYPE_RGB_ALPHA。
//    * interlace可以是PNG_INTERLACE_NONE或PNG_INTERLACE_ADAM7，
//    *而compression_type和filter_type目前必需是PNG_COMPRESSION_TYPE_BASE
//    *和和PNG_FILTER_TYPE_BASE。
//    * /
    png_set_IHDR(png_ptr, info_ptr, pngw, pngh, 8,
                 PNG_COLOR_TYPE_GRAY_ALPHA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
//    / *写入文件头部信息（必需）* /
    png_write_info(png_ptr, info_ptr);
    if (pngh > PNG_UINT_32_MAX / sizeof(png_bytep)) {
        png_error(png_ptr, "图像太高,无法在内存中处理");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }
//    / *将这些像素行指针指向你的“data”字节数组中对应的位置，即：指向每行像素的起始处* /
    s32 bytes_per_pixel = 4;
    png_bytep row_pointers[pngh];
    for (png_uint_32 k = 0; k < pngh; k++) {
        row_pointers[k] = (png_bytep) ((c8 *) data + k * pngw * bytes_per_pixel);
    }
    //    / *一次调用就将整个图像写进文件* /
    png_write_image(png_ptr, row_pointers);
    //    / *必需调用这个函数完成写入文件其余部分* /
    png_write_end(png_ptr, info_ptr);
    //    / *写完后清理并释放已分配的内存* /
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return true;
}
