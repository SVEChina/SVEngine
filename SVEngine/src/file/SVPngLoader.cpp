#include "SVPngLoader.h"
#include "SVFileMgr.h"
#include "../base/SVDataChunk.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"

static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length) {
    tImageSource *isource = (tImageSource *) png_get_io_ptr(png_ptr);
    if ((s32) (isource->offset + length) <= isource->size) {
        memcpy(data, isource->data + isource->offset, length);
        isource->offset += length;
    } else {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}


#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))

SVPngLoader::SVPngLoader(SVInst *_app)
:SVFileLoader(_app) {
    m_iHeight = 0;
    m_iWidth = 0;
    m_iDataLength = 0;
    m_iDataType = -1;
}

SVPngLoader::~SVPngLoader() {
    m_iHeight = 0;
    m_iWidth = 0;
    m_iDataLength = 0;
    m_iDataType = -1;
}

void SVPngLoader::loadData(cptr8 name, u8 **data) {
    SVDataChunk tSVDataChunk;
    bool t_flag = mApp->getFileMgr()->loadFileContent(&tSVDataChunk, name);
    if (t_flag) {
        _readPngFromStream((void *) tSVDataChunk.m_data, tSVDataChunk.m_size, name, data);
    }
}

void SVPngLoader::_premultipliedAlpha(u8 *_data, s32 _width, s32 _height) {
    u32 *fourBytes = (u32 *) _data;
    for (u32 i = 0; i < _width * _height; i++) {
        u8 *p = _data + i * 4;
        fourBytes[i] = CC_RGB_PREMULTIPLY_ALPHA(p[0], p[1], p[2], p[3]);
    }
}

void SVPngLoader::_readPngFromStream(void *instream, s32 dataLen, cptr8 fname,u8 **data) {
    // length of bytes to check if it is a valid png file
#define PNGSIGSIZE  8
    if (dataLen < PNGSIGSIZE)
        return;
    png_byte header[PNGSIGSIZE] = {0};
    png_structp png_ptr = 0;
    png_infop info_ptr = 0;
    //
    memcpy(header, instream, PNGSIGSIZE);
    if (png_sig_cmp(header, 0, PNGSIGSIZE)) {
        //error not png formate
        return;
    }
    // init png_struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        return;
    }
    // init png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        return;
    }
    //设置内部跳转错误，如果在解析png的过程中出现错误 直接跳转到这里
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        return;
    }
    // set the read call back function
    tImageSource imageSource;
    imageSource.data = (u8 *) instream;
    imageSource.size = dataLen;
    imageSource.offset = 0;
    //设置回调函数 来用IO流替换File
    png_set_read_fn(png_ptr, &imageSource, pngReadCallback);
    //读取png头
    png_read_info(png_ptr, info_ptr);
    //读取基本信息
    bool _preMulti = false;
    m_iWidth = png_get_image_width(png_ptr, info_ptr);
    m_iHeight = png_get_image_height(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);
    //不同颜色格式调用
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_packing(png_ptr);           //调色板模式
        png_set_palette_to_rgb(png_ptr);    //扩展索引数据格式到 24位RGB 或 32位RGBA模式(alpha存在)
    }
    //
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        bit_depth = 8;
        png_set_expand_gray_1_2_4_to_8(png_ptr);    //要求位深度强制8bit(补位深度)
    }
    //
    if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA || color_type == PNG_COLOR_TYPE_GRAY) {
        png_set_gray_to_rgb(png_ptr);   //灰度图必须转成RGB
    }
    //
    if (bit_depth == 16) {
        bit_depth = 8;
        png_set_strip_16(png_ptr);  //要求位深度从16位强制转换8bit深度
    }
    //png背景相关
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }
    //
    if (bit_depth < 8) {
        png_set_packing(png_ptr);// Expanded earlier for grayscale, now take care of palette and rgb
    }
   
    //分配内存大小  // read png data
    //重新更新一下Png信息
    png_read_update_info(png_ptr, info_ptr);
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    s32 t_chan_num = s32(rowbytes) / m_iWidth;
    //根据颜色类型 设置渲染格式
    mRenderFormat = GL_RGBA;
    switch (color_type) {
        case PNG_COLOR_TYPE_PALETTE:
            if(t_chan_num == 3){
                mRenderFormat = GL_RGB;
            }else{
                mRenderFormat = GL_RGBA;;
            }
            break;
        case PNG_COLOR_TYPE_GRAY:
            mRenderFormat = GL_RGBA;//GL_LUMINANCE;//Texture2D::PixelFormat::I8;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            mRenderFormat = GL_RGBA;//GL_LUMINANCE_ALPHA;//(int)SVTexture::PixelFormat::AI88;
            break;
        case PNG_COLOR_TYPE_RGB:
            mRenderFormat = GL_RGB;//(int)SVTexture::PixelFormat::RGB888;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            mRenderFormat = GL_RGBA;//(int)SVTexture::PixelFormat::RGBA8888;
            break;
        default:
            break;
    }
    //
    m_iDataLength = rowbytes * m_iHeight;
    *data = (u8 *) (malloc(m_iDataLength * sizeof(u8)));
    u8 *pdata = *data;
    if (!pdata) {
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        return;
    }
    //分配数据流指针
    png_bytep *row_pointers = (png_bytep *) png_malloc(png_ptr, sizeof(png_bytep) * m_iHeight);
    for (u16 i = 0; i < m_iHeight; ++i) {
        row_pointers[i] = pdata + i * rowbytes;
    }
    //读取图形数据
    png_read_image(png_ptr, row_pointers);
    //不继续读下面的内容(辅助模块)
    png_read_end(png_ptr, nullptr);

    // premultiplied alpha for RGBA8888
    if (rowbytes / m_iWidth == 4) {
        //4通道 预乘alpha
        _premultipliedAlpha(pdata, m_iWidth, m_iHeight);
        _preMulti = true;
    } else {
        _preMulti = false;
    }
    if (row_pointers) {
        free(row_pointers);
    }

    m_iDataType = color_type;

    png_destroy_read_struct(&png_ptr, &info_ptr, 0);

    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
    }
    return;
}
