//
// SVImage.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVImage.h"
#include "../base/SVDataSwap.h"
#include "../file/SVLoaderPng.h"
/*
 */
static cptr8 type_names[] = {
    "2D", "3D", "Cube", "2D_Array",
};

static cptr8 format_names[] = {
    "R8", "RG8", "RGB8", "RGBA8",
    "R16", "RG16", "RGB16", "RGBA16",
    "R16F", "RG16F", "RGB16F", "RGBA16F",
    "R32F", "RG32F", "RGB32F", "RGBA32F",
    "R5G6B5", "RGBA4", "RGB5A1", "RGB10A2",
    "DXT1", "DXT3", "DXT5", "ATI1", "ATI2",
    "ETC1", "ETC2", "ETC5", "EAC1", "EAC2",
    "ATC1", "ATC3", "ATC5", "PVR4",
    "ZLC1", "ZLC2",
};

static const s32 pixel_and_block_sizes[] = {
    1,  2,  3,  4,
    2,  4,  6,  8,
    2,  4,  6,  8,
    4,  8, 12, 16,
    2,  2,  2,  4,
    8, 16, 16,  8, 16,
    8,  8, 16,  8, 16,
    8, 16, 16,  8,
    0,  0,
};

/*
 */

SVImage::SVImage(SVInst* _app)
:SVGBase(_app){
    m_pData = MakeSharedPtr<SVDataSwap>();
    clear();
}

SVImage::SVImage(SVInst* _app, cptr8 _filename):SVGBase(_app){
    m_pData = MakeSharedPtr<SVDataSwap>();
    clear();
    load(_filename);
}

SVImage::~SVImage() {
    clear();
    m_pData = nullptr;
}

s32 SVImage::load(cptr8 _filename){
    SV_LOG_INFO("load image: %s \n",_filename );
    SVLoaderPng pngLoad(mApp);
    u8 *pTexData = nullptr;
    pngLoad.loadData(_filename, &pTexData);
    if (pTexData) {
        create2D(pngLoad.getWidth(), pngLoad.getHeight(), SV_FORMAT_RGBA8);
        setPixels(pTexData, pngLoad.getDataLen());
        free(pTexData);
        return 1;
    }
    return 0;
}

void SVImage::clear(){
    m_pData->reback();
    m_type = SV_IMAGE_2D;
    m_format = SV_FORMAT_RGBA8;
    m_width = 1;
    m_height = 1;
    m_depth = 1;
}

s32 SVImage::create2D(s32 _width, s32 _height, s32 _format , bool _allocate ){
    assert(_width > 0 && _height > 0 && "SVImage::create2D(): bad image size");
    clear();

    m_type = SV_IMAGE_2D;
    m_format = _format;
    m_width = _width;
    m_height = _height;
    
    if(_allocate) {
        s32 t_size = getSize();
        m_pData->resize(t_size);
    }
    return 1;
}
s32 SVImage::create3D(s32 _width, s32 _height, s32 _depth, s32 _format, bool _allocate){
    assert(_width > 0 && _height > 0 && _depth > 0 && "SVImage::create3D(): bad image size");
    
    clear();
    
    m_type = SV_IMAGE_3D;
    m_format = _format;
    m_width = _width;
    m_height = _height;
    m_depth = _depth;
    
    if(_allocate) {
        s32 t_size = getSize();
        m_pData->resize(t_size);
    }
    return 1;
}

s32 SVImage::getSize() const{
    if(isRawFormat()) {
        return getPixelSize() * getWidth() * getHeight() * getDepth();
    }
    return 0;
}

s32 SVImage::getPixelSize() const {
    assert(m_format >= SV_FORMAT_R8 && m_format <= SV_FORMAT_RGB10A2 && "SVImage::getPixelSize(): bad image format");
    return pixel_and_block_sizes[(s32)m_format];
}

s32 SVImage::getWidth() const {
    return m_width;
}

s32 SVImage::getHeight() const {
    return m_height;
}

s32 SVImage::getDepth() const {
    return m_depth;
}

s32 SVImage::isRawFormat() const {
    if(m_format >= SV_FORMAT_R8 && m_format <= SV_FORMAT_RGB10A2) return 1;
    return 0;
}

s32 SVImage::isUCharFormat() const {
    if(m_format >= SV_FORMAT_R8 && m_format <= SV_FORMAT_RGBA8) return 1;
    if(m_format >= SV_FORMAT_DXT1 && m_format <= SV_FORMAT_PVR4) return 1;
    return 0;
}

s32 SVImage::isHalfFormat() const {
    if(m_format >= SV_FORMAT_R16F && m_format <= SV_FORMAT_RGBA16F) return 1;
    return 0;
}

s32 SVImage::isFloatFormat() const {
    if(m_format >= SV_FORMAT_R32F && m_format <= SV_FORMAT_RGBA32F) return 1;
    return 0;
}

s32 SVImage::isCombinedFormat() const {
    if(m_format >= SV_FORMAT_R5G6B5 && m_format <= SV_FORMAT_RGB10A2) return 1;
    return 0;
}

s32 SVImage::isCompressedFormat() const {
    if(m_format >= SV_FORMAT_DXT1 && m_format <= SV_FORMAT_ZLC2) return 1;
    return 0;
}

s32 SVImage::isZCompressedFormat() const {
    if(m_format >= SV_FORMAT_ZLC1 && m_format <= SV_FORMAT_ZLC2) return 1;
    return 0;
}

// loaded flag
s32 SVImage::isLoaded() const{
    return 1;
}

// image type
s32 SVImage::getType() const{
    return m_type;
}
cptr8 SVImage::getTypeName() const{
    return type_names[(s32)m_type];
}

// image format
s32 SVImage::getFormat() const{
    return m_format;
}
cptr8 SVImage::getFormatName() const{
    return format_names[(s32)m_format];
}

void SVImage::set2D(s32 _x, s32 _y, const Pixel &_p){
    assert(m_pData != nullptr && "SVImage::set2D(): data is NULL");
    assert(isRawFormat() && "SVImage::set2D(): is not a raw format");
    assert(m_type == SV_IMAGE_2D && "SVImage::set2D(): is not a 2D image");
    _set(_x,_y,0,_p);
}

void SVImage::set2D(s32 _x, s32 _y, s32 _r, s32 _g, s32 _b, s32 _a){
    Pixel p;
    p.i.r = _r;
    p.i.g = _g;
    p.i.b = _b;
    p.i.a = _a;
    p.f.r = _r;
    p.f.g = _g;
    p.f.b = _b;
    p.f.a = _a;
    set2D(_x,_y,p);
}

SVImage::Pixel SVImage::get2D(s32 _x, s32 _y) const{
    Pixel p;
    assert(m_pData != NULL && "SVImage::get2D(): data is NULL");
    assert(isRawFormat() && "SVImage::get2D(): is not a raw format");
    assert(m_type == SV_IMAGE_2D && "SVImage::get2D(): is not a 2D image");
    _get(_x,_y,0,p);
    return p;
}

SVImage::Pixel SVImage::get2D(f32 _x, f32 _y) const{
    Pixel p,p00,p10,p01,p11;
    assert(m_pData != NULL && "SVImage::get2D(): data is NULL");
    assert(isRawFormat() && "SVImage::get2D(): is not a raw format");
    assert(m_type == SV_IMAGE_2D && "SVImage::get2D(): is not a 2D image");
    f32 x = _x * (m_width - 1);
    f32 y = _y * (m_height - 1);
    s32 x0 = (int)floorf(x);
    s32 y0 = (int)floorf(y);
    _get(x0,y0,0,p00,p10,p01,p11);
    x -= x0;
    y -= y0;
    _interpolate(x,y,p00,p10,p01,p11,p);
    return p;
}

s32 SVImage::get2Di(f32 _x, f32 _y, s32 _index) const{
    s32 p00,p10,p01,p11;
    assert(m_pData != NULL && "Image::get2Di(): data is NULL");
    assert(isRawFormat() && "Image::get2Di(): is not a raw format");
    assert(m_type == SV_IMAGE_2D && "Image::get2Di(): is not a 2D image");
    f32 x = _x * (m_width - 1);
    f32 y = _y * (m_height - 1);
    s32 x0 = (s32)floorf(x);
    s32 y0 = (s32)floorf(y);
    _geti(x0,y0,0,_index,p00,p10,p01,p11);
    x -= x0;
    y -= y0;
    f32 x1 = 1.0f - x;
    f32 y1 = 1.0f - y;
    return (s32)(p00 * (x1 * y1) + p01 * (x1 * y) + p10 * (x * y1) + p11 * (x * y));
}

void SVImage::set3D(s32 _x, s32 _y, s32 _z, const Pixel &_p){
    assert(m_pData != nullptr && "SVImage::set3D(): data is NULL");
    assert(isRawFormat() && "SVImage::set3D(): is not a raw format");
    assert(m_type == SV_IMAGE_3D && "SVImage::set3D(): is not a 3D image");
    _set(_x,_y,_z,0,_p);
}

void SVImage::set3D(s32 _x, s32 _y, s32 _z, s32 _r, s32 _g, s32 _b, s32 _a){
    Pixel p;
    p.i.r = _r;
    p.i.g = _g;
    p.i.b = _b;
    p.i.a = _a;
    p.f.r = _r;
    p.f.g = _g;
    p.f.b = _b;
    p.f.a = _a;
    set3D(_x,_y,_z,p);
}

SVImage::Pixel SVImage::get3D(s32 _x, s32 _y, s32 _z) const{
    Pixel p;
    assert(m_pData != nullptr && "SVImage::get3D(): data is NULL");
    assert(isRawFormat() && "SVImage::get3D(): is not a raw format");
    assert(m_type == SV_IMAGE_3D && "SVImage::get3D(): is not a 3D image");
    _get(_x,_y,_z,0,p);
    return p;
}

SVImage::Pixel SVImage::get3D(f32 _x, f32 _y, f32 _z) const{
    Pixel p,p000,p100,p010,p110,p001,p101,p011,p111,p0,p1;
    assert(m_pData != nullptr && "SVImage::get3D(): data is NULL");
    assert(isRawFormat() && "SVImage::get3D(): is not a raw format");
    assert(m_type == SV_IMAGE_3D && "SVImage::get3D(): is not a 3D image");
    f32 x = _x * (m_width - 1);
    f32 y = _y * (m_height - 1);
    f32 z = _z * (m_depth - 1);
    s32 x0 = (s32)floorf(x);
    s32 y0 = (s32)floorf(y);
    s32 z0 = (s32)floorf(z);
    s32 z1 = z0 + 1;
    _get(x0,y0,z0,0,p000,p100,p010,p110);
    _get(x0,y0,z1,0,p001,p101,p011,p111);
    x -= x0;
    y -= y0;
    z -= z0;
    _interpolate(x,y,p000,p100,p010,p110,p0);
    _interpolate(x,y,p001,p101,p011,p111,p1);
    f32 k0 = 1.0f - z;
    f32 k1 = z;
    if(isFloatFormat()) {
#ifdef USE_SSE
        __m128 res_0 = _mm_mul_ps(p0.f.vec,_mm_set1_ps(k0));
        __m128 res_1 = _mm_mul_ps(p1.f.vec,_mm_set1_ps(k1));
        p.f.vec = _mm_add_ps(res_0,res_1);
#elif USE_ALTIVEC
        vec_float4 res_0 = vec_madd(p0.f.vec,vec_splats(k0),vec_splats(0.0f));
        p.f.vec = vec_madd(p1.f.vec,vec_splats(k1),res_0);
#else
        p.f.r = p0.f.r * k0 + p1.f.r * k1;
        p.f.g = p0.f.g * k0 + p1.f.g * k1;
        p.f.b = p0.f.b * k0 + p1.f.b * k1;
        p.f.a = p0.f.a * k0 + p1.f.a * k1;
#endif
    } else {
#ifdef USE_SSE2
        __m128 res_0 = _mm_mul_ps(_mm_cvtepi32_ps(p0.i.vec),_mm_set1_ps(k0));
        __m128 res_1 = _mm_mul_ps(_mm_cvtepi32_ps(p1.i.vec),_mm_set1_ps(k1));
        p.i.vec = _mm_cvtps_epi32(_mm_add_ps(res_0,res_1));
#elif USE_ALTIVEC
        vec_float4 res_0 = vec_madd(vec_ctf(p0.i.vec,0),vec_splats(k0),vec_splats(0.0f));
        p.i.vec = vec_cts(vec_madd(vec_ctf(p1.i.vec,0),vec_splats(k1),res_0),0);
#else
        p.i.r = (s32)(p0.i.r * k0 + p1.i.r * k1);
        p.i.g = (s32)(p0.i.g * k0 + p1.i.g * k1);
        p.i.b = (s32)(p0.i.b * k0 + p1.i.b * k1);
        p.i.a = (s32)(p0.i.a * k0 + p1.i.a * k1);
#endif
    }
    return p;
}

void SVImage::setPixels(u8 *pixels, s32 _size){
    m_pData->writeData(pixels, _size);
}

u8 * SVImage::getPixels(){
    return (u8 *)m_pData->getData();
}
u8 * SVImage::getPixels2D(){
    return (u8 *)m_pData->getData();
}
u8 * SVImage::getPixels3D(){
    return (u8 *)m_pData->getData();
}

cptr8 SVImage::getPixels() const{
    return (cptr8)m_pData->getData();
}
cptr8 SVImage::getPixels2D() const{
    return (cptr8)m_pData->getData();
}
cptr8 SVImage::getPixels3D() const{
    return (cptr8)m_pData->getData();
}

void SVImage::_set(s32 _x, s32 _y, s32 _offset, const Pixel &_p){
    s32 x = _x % m_width;
    s32 y = _y % m_height;
    s32 t_id = _offset + m_width * y + x;
    u8 *data = (u8 *)m_pData->getData();
    switch(m_format) {
        // 8-bits per pixel unsigned formats
        case SV_FORMAT_R8: {
            u8 *d = &data[t_id * 1];
            *(d + 0) = (u8)_p.i.r;
            return;
        }
        case SV_FORMAT_RG8: {
            u8 *d = &data[t_id * 2];
            *(d + 0) = (u8)_p.i.r;
            *(d + 1) = (u8)_p.i.g;
            return;
        }
        case SV_FORMAT_RGB8: {
            u8 *d = &data[t_id * 3];
            *(d + 0) = (u8)_p.i.r;
            *(d + 1) = (u8)_p.i.g;
            *(d + 2) = (u8)_p.i.b;
            return;
        }
        case SV_FORMAT_RGBA8: {
            u8 *d = &data[t_id * 4];
            *(d + 0) = (u8)_p.i.r;
            *(d + 1) = (u8)_p.i.g;
            *(d + 2) = (u8)_p.i.b;
            *(d + 3) = (u8)_p.i.a;
            return;
        }
        // 16-bits per pixel unsigned formats
        case SV_FORMAT_R16: {
            u16 *d = (u16*)&data[t_id * 2];
            *(d + 0) = (u16)_p.i.r;
            return;
        }
        case SV_FORMAT_RG16: {
            u16 *d = (u16*)&data[t_id * 4];
            *(d + 0) = (u16)_p.i.r;
            *(d + 1) = (u16)_p.i.g;
            return;
        }
        case SV_FORMAT_RGB16: {
            u16 *d = (u16*)&data[t_id * 6];
            *(d + 0) = (u16)_p.i.r;
            *(d + 1) = (u16)_p.i.g;
            *(d + 2) = (u16)_p.i.b;
            return;
        }
        case SV_FORMAT_RGBA16: {
            u16 *d = (u16*)&data[t_id * 8];
            *(d + 0) = (u16)_p.i.r;
            *(d + 1) = (u16)_p.i.g;
            *(d + 2) = (u16)_p.i.b;
            *(d + 3) = (u16)_p.i.a;
            return;
        }
        // 16-bits per pixel f32 formats
        case SV_FORMAT_R16F: {
            u16 *d = (u16*)&data[t_id * 2];
            *(d + 0) = (u16)_p.i.r;
            return;
        }
        case SV_FORMAT_RG16F: {
            u16 *d = (u16*)&data[t_id * 4];
            *(d + 0) = (u16)_p.i.r;
            *(d + 1) = (u16)_p.i.g;
            return;
        }
        case SV_FORMAT_RGB16F: {
            u16 *d = (u16*)&data[t_id * 6];
            *(d + 0) = (u16)_p.i.r;
            *(d + 1) = (u16)_p.i.g;
            *(d + 2) = (u16)_p.i.b;
            return;
        }
        case SV_FORMAT_RGBA16F: {
            u16 *d = (u16*)&data[t_id * 8];
            *(d + 0) = (u16)_p.i.r;
            *(d + 1) = (u16)_p.i.g;
            *(d + 2) = (u16)_p.i.b;
            *(d + 3) = (u16)_p.i.a;
            return;
        }
        // 32-bits per pixel f32 formats
        case SV_FORMAT_R32F: {
            f32 *d = (f32*)&data[t_id * 4];
            *(d + 0) = _p.f.r;
            return;
        }
        case SV_FORMAT_RG32F: {
            f32 *d = (f32*)&data[t_id * 8];
            *(d + 0) = _p.f.r;
            *(d + 1) = _p.f.g;
            return;
        }
        case SV_FORMAT_RGB32F: {
            f32 *d = (f32*)&data[t_id * 12];
            *(d + 0) = _p.f.r;
            *(d + 1) = _p.f.g;
            *(d + 2) = _p.f.b;
            return;
        }
        case SV_FORMAT_RGBA32F: {
            f32 *d = (f32*)&data[t_id * 16];
            *(d + 0) = _p.f.r;
            *(d + 1) = _p.f.g;
            *(d + 2) = _p.f.b;
            *(d + 3) = _p.f.a;
            return;
        }
    }
}
void SVImage::_get(s32 _x, s32 _y, s32 _offset, Pixel &_p) const{
    s32 x = _x % m_width;
    s32 y = _y % m_height;
    s32 t_id = _offset + m_width * y + x;
    u8 *data = (u8 *)m_pData->getData();
    switch(m_format) {
        // 8-bits per pixel unsigned formats
        case SV_FORMAT_R8: {
            u8 *d = &data[t_id * 1];
            _p.i.r = *(d + 0);
            _p.i.g = 0;
            _p.i.b = 0;
            _p.i.a = 255;
            return;
        }
        case SV_FORMAT_RG8: {
            u8 *d = &data[t_id * 2];
            _p.i.r = *(d + 0);
            _p.i.g = *(d + 1);
            _p.i.b = 0;
            _p.i.a = 255;
            return;
        }
        case SV_FORMAT_RGB8: {
            u8 *d = &data[t_id * 3];
            _p.i.r = *(d + 0);
            _p.i.g = *(d + 1);
            _p.i.b = *(d + 2);
            _p.i.a = 255;
            return;
        }
        case SV_FORMAT_RGBA8: {
            u8 *d = &data[t_id * 4];
            _p.i.r = *(d + 0);
            _p.i.g = *(d + 1);
            _p.i.b = *(d + 2);
            _p.i.a = *(d + 3);
            return;
        }
        // 16-bits per pixel unsigned formats
        case SV_FORMAT_R16: {
            u16 *d = (u16*)&data[t_id * 2];
            _p.i.r = *(d + 0);
            _p.i.g = 0;
            _p.i.b = 0;
            _p.i.a = 65535;
            return;
        }
        case SV_FORMAT_RG16: {
            u16 *d = (u16*)&data[t_id * 4];
            _p.i.r = *(d + 0);
            _p.i.g = *(d + 1);
            _p.i.b = 0;
            _p.i.a = 65535;
            return;
        }
        case SV_FORMAT_RGB16: {
            u16 *d = (u16*)&data[t_id * 6];
            _p.i.r = *(d + 0);
            _p.i.g = *(d + 1);
            _p.i.b = *(d + 2);
            _p.i.a = 65535;
            return;
        }
        case SV_FORMAT_RGBA16: {
            u16 *d = (u16*)&data[t_id * 8];
            _p.i.r = *(d + 0);
            _p.i.g = *(d + 1);
            _p.i.b = *(d + 2);
            _p.i.a = *(d + 3);
            return;
        }
        // 16-bits per pixel f32 formats
        case SV_FORMAT_R16F: {
            u16 *d = (u16*)&data[t_id * 2];
            _p.i.r = *(d + 0);
            _p.i.g = 0x0000;
            _p.i.b = 0x0000;
            _p.i.a = 0x3c00;
            return;
        }
        case SV_FORMAT_RG16F: {
            u16 *d = (u16*)&data[t_id * 4];
            _p.i.r = *(d + 0);
            _p.i.g = *(d + 1);
            _p.i.b = 0x0000;
            _p.i.a = 0x3c00;
            return;
        }
        case SV_FORMAT_RGB16F: {
            u16 *d = (u16*)&data[t_id * 6];
            _p.i.r = *(d + 0);
            _p.i.g = *(d + 1);
            _p.i.b = *(d + 2);
            _p.i.a = 0x3c00;
            return;
        }
        case SV_FORMAT_RGBA16F: {
            u16 *d = (u16*)&data[t_id * 8];
            _p.i.r = *(d + 0);
            _p.i.g = *(d + 1);
            _p.i.b = *(d + 2);
            _p.i.a = *(d + 3);
            return;
        }
        // 32-bits per pixel f32 formats
        case SV_FORMAT_R32F: {
            f32 *d = (f32*)&data[t_id * 4];
            _p.f.r = *(d + 0);
            _p.f.g = 0.0f;
            _p.f.b = 0.0f;
            _p.f.a = 1.0f;
            return;
        }
        case SV_FORMAT_RG32F: {
            f32 *d = (f32*)&data[t_id * 8];
            _p.f.r = *(d + 0);
            _p.f.g = *(d + 1);
            _p.f.b = 0.0f;
            _p.f.a = 1.0f;
            return;
        }
        case SV_FORMAT_RGB32F: {
            f32 *d = (f32*)&data[t_id * 12];
            _p.f.r = *(d + 0);
            _p.f.g = *(d + 1);
            _p.f.b = *(d + 2);
            _p.f.a = 1.0f;
            return;
        }
        case SV_FORMAT_RGBA32F: {
            f32 *d = (f32*)&data[t_id * 16];
            _p.f.r = *(d + 0);
            _p.f.g = *(d + 1);
            _p.f.b = *(d + 2);
            _p.f.a = *(d + 3);
            return;
        }
    }
}
void SVImage::_get(s32 _x, s32 _y, s32 _offset, Pixel &_p00, Pixel &_p10, Pixel &_p01, Pixel &_p11) const{
    s32 x0 = _x % m_width;
    s32 y0 = _y % m_height;
    s32 x1 = (_x + 1) % m_width;
    s32 y1 = (_y + 1) % m_height;
    s32 id_00 = _offset + m_width * y0 + x0;
    s32 id_10 = _offset + m_width * y0 + x1;
    s32 id_01 = _offset + m_width * y1 + x0;
    s32 id_11 = _offset + m_width * y1 + x1;
    u8 *data = (u8 *)m_pData->getData();
    switch(m_format) {
        // 8-bits per pixel unsigned formats
        case SV_FORMAT_R8: {
            u8 *d00 = &data[id_00 * 1];
            u8 *d10 = &data[id_10 * 1];
            u8 *d01 = &data[id_01 * 1];
            u8 *d11 = &data[id_11 * 1];
            _p00.i.r = *(d00 + 0); _p10.i.r = *(d10 + 0); _p01.i.r = *(d01 + 0); _p11.i.r = *(d11 + 0);
            _p00.i.g = 0; _p10.i.g = 0; _p01.i.g = 0; _p11.i.g = 0;
            _p00.i.b = 0; _p10.i.b = 0; _p01.i.b = 0; _p11.i.b = 0;
            _p00.i.a = 255; _p10.i.a = 255; _p01.i.a = 255; _p11.i.a = 255;
            return;
        }
        case SV_FORMAT_RG8: {
            u8 *d00 = &data[id_00 * 2];
            u8 *d10 = &data[id_10 * 2];
            u8 *d01 = &data[id_01 * 2];
            u8 *d11 = &data[id_11 * 2];
            _p00.i.r = *(d00 + 0); _p10.i.r = *(d10 + 0); _p01.i.r = *(d01 + 0); _p11.i.r = *(d11 + 0);
            _p00.i.g = *(d00 + 1); _p10.i.g = *(d10 + 1); _p01.i.g = *(d01 + 1); _p11.i.g = *(d11 + 1);
            _p00.i.b = 0; _p10.i.b = 0; _p01.i.b = 0; _p11.i.b = 0;
            _p00.i.a = 255; _p10.i.a = 255; _p01.i.a = 255; _p11.i.a = 255;
            return;
        }
        case SV_FORMAT_RGB8: {
            u8 *d00 = &data[id_00 * 3];
            u8 *d10 = &data[id_10 * 3];
            u8 *d01 = &data[id_01 * 3];
            u8 *d11 = &data[id_11 * 3];
            _p00.i.r = *(d00 + 0); _p10.i.r = *(d10 + 0); _p01.i.r = *(d01 + 0); _p11.i.r = *(d11 + 0);
            _p00.i.g = *(d00 + 1); _p10.i.g = *(d10 + 1); _p01.i.g = *(d01 + 1); _p11.i.g = *(d11 + 1);
            _p00.i.b = *(d00 + 2); _p10.i.b = *(d10 + 2); _p01.i.b = *(d01 + 2); _p11.i.b = *(d11 + 2);
            _p00.i.a = 255; _p10.i.a = 255; _p01.i.a = 255; _p11.i.a = 255;
            return;
        }
        case SV_FORMAT_RGBA8: {
            u8 *d00 = &data[id_00 * 4];
            u8 *d10 = &data[id_10 * 4];
            u8 *d01 = &data[id_01 * 4];
            u8 *d11 = &data[id_11 * 4];
            _p00.i.r = *(d00 + 0); _p10.i.r = *(d10 + 0); _p01.i.r = *(d01 + 0); _p11.i.r = *(d11 + 0);
            _p00.i.g = *(d00 + 1); _p10.i.g = *(d10 + 1); _p01.i.g = *(d01 + 1); _p11.i.g = *(d11 + 1);
            _p00.i.b = *(d00 + 2); _p10.i.b = *(d10 + 2); _p01.i.b = *(d01 + 2); _p11.i.b = *(d11 + 2);
            _p00.i.a = *(d00 + 3); _p10.i.a = *(d10 + 3); _p01.i.a = *(d01 + 3); _p11.i.a = *(d11 + 3);
            return;
        }
        // 16-bits per pixel unsigned formats
        case SV_FORMAT_R16: {
            u16 *d00 = (u16*)&data[id_00 * 2];
            u16 *d10 = (u16*)&data[id_10 * 2];
            u16 *d01 = (u16*)&data[id_01 * 2];
            u16 *d11 = (u16*)&data[id_11 * 2];
            _p00.i.r = *(d00 + 0); _p10.i.r = *(d10 + 0); _p01.i.r = *(d01 + 0); _p11.i.r = *(d11 + 0);
            _p00.i.g = 0; _p10.i.g = 0; _p01.i.g = 0; _p11.i.g = 0;
            _p00.i.b = 0; _p10.i.b = 0; _p01.i.b = 0; _p11.i.b = 0;
            _p00.i.a = 65535; _p10.i.a = 65535; _p01.i.a = 65535; _p11.i.a = 65535;
            return;
        }
        case SV_FORMAT_RG16: {
            u16 *d00 = (u16*)&data[id_00 * 4];
            u16 *d10 = (u16*)&data[id_10 * 4];
            u16 *d01 = (u16*)&data[id_01 * 4];
            u16 *d11 = (u16*)&data[id_11 * 4];
            _p00.i.r = *(d00 + 0); _p10.i.r = *(d10 + 0); _p01.i.r = *(d01 + 0); _p11.i.r = *(d11 + 0);
            _p00.i.g = *(d00 + 1); _p10.i.g = *(d10 + 1); _p01.i.g = *(d01 + 1); _p11.i.g = *(d11 + 1);
            _p00.i.b = 0; _p10.i.b = 0; _p01.i.b = 0; _p11.i.b = 0;
            _p00.i.a = 65535; _p10.i.a = 65535; _p01.i.a = 65535; _p11.i.a = 65535;
            return;
        }
        case SV_FORMAT_RGB16: {
            u16 *d00 = (u16*)&data[id_00 * 6];
            u16 *d10 = (u16*)&data[id_10 * 6];
            u16 *d01 = (u16*)&data[id_01 * 6];
            u16 *d11 = (u16*)&data[id_11 * 6];
            _p00.i.r = *(d00 + 0); _p10.i.r = *(d10 + 0); _p01.i.r = *(d01 + 0); _p11.i.r = *(d11 + 0);
            _p00.i.g = *(d00 + 1); _p10.i.g = *(d10 + 1); _p01.i.g = *(d01 + 1); _p11.i.g = *(d11 + 1);
            _p00.i.b = *(d00 + 2); _p10.i.b = *(d10 + 2); _p01.i.b = *(d01 + 2); _p11.i.b = *(d11 + 2);
            _p00.i.a = 65535; _p10.i.a = 65535; _p01.i.a = 65535; _p11.i.a = 65535;
            return;
        }
        case SV_FORMAT_RGBA16: {
            u16 *d00 = (u16*)&data[id_00 * 8];
            u16 *d10 = (u16*)&data[id_10 * 8];
            u16 *d01 = (u16*)&data[id_01 * 8];
            u16 *d11 = (u16*)&data[id_11 * 8];
            _p00.i.r = *(d00 + 0); _p10.i.r = *(d10 + 0); _p01.i.r = *(d01 + 0); _p11.i.r = *(d11 + 0);
            _p00.i.g = *(d00 + 1); _p10.i.g = *(d10 + 1); _p01.i.g = *(d01 + 1); _p11.i.g = *(d11 + 1);
            _p00.i.b = *(d00 + 2); _p10.i.b = *(d10 + 2); _p01.i.b = *(d01 + 2); _p11.i.b = *(d11 + 2);
            _p00.i.a = *(d00 + 3); _p10.i.a = *(d10 + 3); _p01.i.a = *(d01 + 3); _p11.i.a = *(d11 + 3);
            return;
        }
        // 32-bits per pixel f32 formats
        case SV_FORMAT_R32F: {
            f32 *d00 = (f32*)&data[id_00 * 4];
            f32 *d10 = (f32*)&data[id_10 * 4];
            f32 *d01 = (f32*)&data[id_01 * 4];
            f32 *d11 = (f32*)&data[id_11 * 4];
            _p00.f.r = *(d00 + 0); _p10.f.r = *(d10 + 0); _p01.f.r = *(d01 + 0); _p11.f.r = *(d11 + 0);
            _p00.f.g = 0.0f; _p10.f.g = 0.0f; _p01.f.g = 0.0f; _p11.f.g = 0.0f;
            _p00.f.b = 0.0f; _p10.f.b = 0.0f; _p01.f.b = 0.0f; _p11.f.b = 0.0f;
            _p00.f.a = 1.0f; _p10.f.a = 1.0f; _p01.f.a = 1.0f; _p11.f.a = 1.0f;
            return;
        }
        case SV_FORMAT_RG32F: {
            f32 *d00 = (f32*)&data[id_00 * 8];
            f32 *d10 = (f32*)&data[id_10 * 8];
            f32 *d01 = (f32*)&data[id_01 * 8];
            f32 *d11 = (f32*)&data[id_11 * 8];
            _p00.f.r = *(d00 + 0); _p10.f.r = *(d10 + 0); _p01.f.r = *(d01 + 0); _p11.f.r = *(d11 + 0);
            _p00.f.g = *(d00 + 1); _p10.f.g = *(d10 + 1); _p01.f.g = *(d01 + 1); _p11.f.g = *(d11 + 1);
            _p00.f.b = 0.0f; _p10.f.b = 0.0f; _p01.f.b = 0.0f; _p11.f.b = 0.0f;
            _p00.f.a = 1.0f; _p10.f.a = 1.0f; _p01.f.a = 1.0f; _p11.f.a = 1.0f;
            return;
        }
        case SV_FORMAT_RGB32F: {
            f32 *d00 = (f32*)&data[id_00 * 12];
            f32 *d10 = (f32*)&data[id_10 * 12];
            f32 *d01 = (f32*)&data[id_01 * 12];
            f32 *d11 = (f32*)&data[id_11 * 12];
            _p00.f.r = *(d00 + 0); _p10.f.r = *(d10 + 0); _p01.f.r = *(d01 + 0); _p11.f.r = *(d11 + 0);
            _p00.f.g = *(d00 + 1); _p10.f.g = *(d10 + 1); _p01.f.g = *(d01 + 1); _p11.f.g = *(d11 + 1);
            _p00.f.b = *(d00 + 2); _p10.f.b = *(d10 + 2); _p01.f.b = *(d01 + 2); _p11.f.b = *(d11 + 2);
            _p00.f.a = 1.0f; _p10.f.a = 1.0f; _p01.f.a = 1.0f; _p11.f.a = 1.0f;
            return;
        }
        case SV_FORMAT_RGBA32F: {
            f32 *d00 = (f32*)&data[id_00 * 16];
            f32 *d10 = (f32*)&data[id_10 * 16];
            f32 *d01 = (f32*)&data[id_01 * 16];
            f32 *d11 = (f32*)&data[id_11 * 16];
            _p00.f.r = *(d00 + 0); _p10.f.r = *(d10 + 0); _p01.f.r = *(d01 + 0); _p11.f.r = *(d11 + 0);
            _p00.f.g = *(d00 + 1); _p10.f.g = *(d10 + 1); _p01.f.g = *(d01 + 1); _p11.f.g = *(d11 + 1);
            _p00.f.b = *(d00 + 2); _p10.f.b = *(d10 + 2); _p01.f.b = *(d01 + 2); _p11.f.b = *(d11 + 2);
            _p00.f.a = *(d00 + 3); _p10.f.a = *(d10 + 3); _p01.f.a = *(d01 + 3); _p11.f.a = *(d11 + 3);
            return;
        }
    }
}
void SVImage::_geti(s32 _x, s32 _y, s32 _offset, s32 _index, s32 &_p00, s32 &_p10, s32 &_p01, s32 &_p11) const{
    s32 x0 = _x % m_width;
    s32 y0 = _y % m_height;
    s32 x1 = (_x + 1) % m_width;
    s32 y1 = (_y + 1) % m_height;
    s32 id_00 = _offset + m_width * y0 + x0;
    s32 id_10 = _offset + m_width * y0 + x1;
    s32 id_01 = _offset + m_width * y1 + x0;
    s32 id_11 = _offset + m_width * y1 + x1;
    u8 *data = (u8 *)m_pData->getData();
    switch(m_format) {
        case SV_FORMAT_R8: {
            _p00 = data[id_00 * 1 + _index];
            _p10 = data[id_10 * 1 + _index];
            _p01 = data[id_01 * 1 + _index];
            _p11 = data[id_11 * 1 + _index];
            return;
        }
        case SV_FORMAT_RG8: {
            _p00 = data[id_00 * 2 + _index];
            _p10 = data[id_10 * 2 + _index];
            _p01 = data[id_01 * 2 + _index];
            _p11 = data[id_11 * 2 + _index];
            return;
        }
        case SV_FORMAT_RGB8: {
            _p00 = data[id_00 * 3 + _index];
            _p10 = data[id_10 * 3 + _index];
            _p01 = data[id_01 * 3 + _index];
            _p11 = data[id_11 * 3 + _index];
            return;
        }
        case SV_FORMAT_RGBA8: {
            _p00 = data[id_00 * 4 + _index];
            _p10 = data[id_10 * 4 + _index];
            _p01 = data[id_01 * 4 + _index];
            _p11 = data[id_11 * 4 + _index];
            return;
        }
    }
}

void SVImage::_set(s32 _x, s32 _y, s32 _z, s32 _offset, const Pixel &_p){
    s32 z = _z % m_depth;
    s32 offset = m_width * m_height * z + _offset;
    _set(_x,_y,offset,_p);
}

void SVImage::_get(s32 _x, s32 _y, s32 _z, s32 _offset, Pixel &_p) const{
    s32 z = _z % m_depth;
    s32 offset = m_width * m_height * z + _offset;
    _get(_x,_y,offset,_p);
}

void SVImage::_get(s32 _x, s32 _y, s32 _z, s32 _offset, Pixel &_p00, Pixel &_p10, Pixel &_p01, Pixel &_p11) const{
    s32 z = _z % m_depth;
    s32 offset = m_width * m_height * z + _offset;
    _get(_x,_y,offset,_p00,_p10,_p01,_p11);
}
void SVImage::_interpolate(f32 _x, f32 _y, const Pixel &_p00, const Pixel &_p10, const Pixel &_p01, const Pixel &_p11, Pixel &_p) const{
    f32 x1 = 1.0f - _x;
    f32 y1 = 1.0f - _y;
    f32 k00 = x1 * y1;
    f32 k10 = _x * y1;
    f32 k01 = x1 * _y;
    f32 k11 = _x * _y;
    if(isFloatFormat()) {
#ifdef USE_SSE
        __m128 res_0 = _mm_mul_ps(p00.f.vec,_mm_set1_ps(k00));
        __m128 res_1 = _mm_mul_ps(p10.f.vec,_mm_set1_ps(k10));
        __m128 res_2 = _mm_mul_ps(p01.f.vec,_mm_set1_ps(k01));
        __m128 res_3 = _mm_mul_ps(p11.f.vec,_mm_set1_ps(k11));
        p.f.vec = _mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,res_3));
#elif USE_ALTIVEC
        vec_float4 res_0 = vec_madd(p00.f.vec,vec_splats(k00),vec_splats(0.0f));
        vec_float4 res_1 = vec_madd(p10.f.vec,vec_splats(k10),res_0);
        vec_float4 res_2 = vec_madd(p01.f.vec,vec_splats(k01),res_1);
        p.f.vec = vec_madd(p11.f.vec,vec_splats(k11),res_2);
#else
        _p.f.r = _p00.f.r * k00 + _p01.f.r * k01 + _p10.f.r * k10 + _p11.f.r * k11;
        _p.f.g = _p00.f.g * k00 + _p01.f.g * k01 + _p10.f.g * k10 + _p11.f.g * k11;
        _p.f.b = _p00.f.b * k00 + _p01.f.b * k01 + _p10.f.b * k10 + _p11.f.b * k11;
        _p.f.a = _p00.f.a * k00 + _p01.f.a * k01 + _p10.f.a * k10 + _p11.f.a * k11;
#endif
    } else {
#ifdef USE_SSE2
        __m128 res_0 = _mm_mul_ps(_mm_cvtepi32_ps(p00.i.vec),_mm_set1_ps(k00));
        __m128 res_1 = _mm_mul_ps(_mm_cvtepi32_ps(p10.i.vec),_mm_set1_ps(k10));
        __m128 res_2 = _mm_mul_ps(_mm_cvtepi32_ps(p01.i.vec),_mm_set1_ps(k01));
        __m128 res_3 = _mm_mul_ps(_mm_cvtepi32_ps(p11.i.vec),_mm_set1_ps(k11));
        p.i.vec = _mm_cvtps_epi32(_mm_add_ps(_mm_add_ps(res_0,res_1),_mm_add_ps(res_2,res_3)));
#elif USE_ALTIVEC
        vec_float4 res_0 = vec_madd(vec_ctf(p00.i.vec,0),vec_splats(k00),vec_splats(0.0f));
        vec_float4 res_1 = vec_madd(vec_ctf(p10.i.vec,0),vec_splats(k10),res_0);
        vec_float4 res_2 = vec_madd(vec_ctf(p01.i.vec,0),vec_splats(k01),res_1);
        p.i.vec = vec_cts(vec_madd(vec_ctf(p11.i.vec,0),vec_splats(k11),res_2),0);
#else
        _p.i.r = (s32)(_p00.i.r * k00 + _p01.i.r * k01 + _p10.i.r * k10 + _p11.i.r * k11);
        _p.i.g = (s32)(_p00.i.g * k00 + _p01.i.g * k01 + _p10.i.g * k10 + _p11.i.g * k11);
        _p.i.b = (s32)(_p00.i.b * k00 + _p01.i.b * k01 + _p10.i.b * k10 + _p11.i.b * k11);
        _p.i.a = (s32)(_p00.i.a * k00 + _p01.i.a * k01 + _p10.i.a * k10 + _p11.i.a * k11);
#endif
    }
}
