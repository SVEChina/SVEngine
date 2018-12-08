//
// SVRResGL.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRResGL.h"
#include "../../third/rapidjson/document.h"
#include "../../base/SVPreDeclare.h"
#include "../../app/SVInst.h"
#include "../../mtl/SVMtlDef.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../base/SVDataChunk.h"
#include "../../file/SVFileMgr.h"
#include "../../base/SVDataSwap.h"
#include "../../base/SVLock.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../renderer/SVContextBase.h"
#include "../renderer/SVRendererBase.h"
#include "../SVGL/SVRendererGL.h"

SVRResGLTex::SVRResGLTex(SVInst* _app)
        :SVResTex(_app){
    m_uid = mApp->m_IDPool.applyUID();
    m_texLock = MakeSharedPtr<SVLock>();
    m_bLoad = false;
    m_enableMipMap = false;
    m_name = "";
    m_id = 0;
    m_width = 1;
    m_height = 1;
    m_type = GL_TEXTURE_2D;
    m_informate = GL_RGBA;
    m_dataformate = GL_RGBA;
}

SVRResGLTex::~SVRResGLTex(){
    m_texLock = nullptr;
    mApp->m_IDPool.returnUID(m_uid);
}

void SVRResGLTex:: create(SVRendererBasePtr _renderer) {
    SVRObjBase::create(_renderer); {
        if( m_id == 0 ){
            m_bLoad = true;
            glGenTextures(1, &m_id);
            glBindTexture(GL_TEXTURE_2D, m_id);
            if(m_pData){
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             m_informate,
                             m_width,
                             m_height,
                             0,
                             m_dataformate,
                             GL_UNSIGNED_BYTE,
                             m_pData->getData());
                
            }else{
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             m_informate,
                             m_width,
                             m_height,
                             0,
                             m_dataformate,
                             GL_UNSIGNED_BYTE,
                             nullptr);
            }
            m_pData = nullptr;
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            if (m_enableMipMap) {
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            }
        }
    }
}

void SVRResGLTex::destroy(SVRendererBasePtr _renderer) {
    SVRObjBase::destroy(_renderer);
    if(m_id>0){
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}

void SVRResGLTex::commit() {
    m_texLock->lock();
    if (m_pData) {
        if(m_bLoad){
            //更新数据
            m_pData->lockData();
            glBindTexture(GL_TEXTURE_2D, m_id);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_informate,GL_UNSIGNED_BYTE,m_pData->getData());
            m_pData->unlockData();
            m_pData = nullptr;
        }
    }
    m_texLock->unlock();
}

void SVRResGLTex::setTexData(void *_data, s32 _len){
    m_texLock->lock();
    if( _data && _len>0 ) {
        SVDataSwapPtr t_pDataSwap = MakeSharedPtr<SVDataSwap>();
        t_pDataSwap->writeData(_data, _len);
        setData(t_pDataSwap);
    }
    m_texLock->unlock();
}



u32 SVRResGLTex::getTexID(){
    return m_id;
}

u32 SVRResGLTex::getuid(){
    return m_uid;
}

bool SVRResGLTex::getbLoad() {
    return m_bLoad;
}
//////
SVRResGLTexWithTexID::SVRResGLTexWithTexID(SVInst* _app, s32 _id):SVRResGLTex(_app){
    m_id = _id;
}

SVRResGLTexWithTexID::~SVRResGLTexWithTexID(){
    
}

void SVRResGLTexWithTexID:: create(SVRendererBasePtr _renderer) {
    SVRObjBase::create(_renderer);
    m_bLoad = true;
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void SVRResGLTexWithTexID::destroy(SVRendererBasePtr _renderer) {
    SVRObjBase::destroy(_renderer);
    if(m_id>0){
        m_id = 0;
    }
}

//////
SVRResGLTexPlist::SVRResGLTexPlist(SVInst *mApp)
        : SVRResGLTex(mApp) {
    m_type = 3;
    rot = false;    //是否旋转
    trim = false;   //是否旋转
    srcw = 128;     //原有纹理的宽度
    srch = 128;     //原有纹理的高度
    m_pTexset = nullptr;
}

SVRResGLTexPlist::~SVRResGLTexPlist() {
    unbindTexset();
}

void SVRResGLTexPlist::refreshParam(){
    if(m_pTexset){
        m_id = m_pTexset->getTexID();  //同步纹理ID
        m_width = m_pTexset->getwidth();
        m_height = m_pTexset->getheight();
    }
}

void SVRResGLTexPlist::apply(){
    if(m_pTexset){
        //m_pTexset->apply();
        refreshParam();
    }
}

void SVRResGLTexPlist::apply(void *data){
    if(m_pTexset){
        //m_pTexset->apply(data);
        refreshParam();
    }
}

void SVRResGLTexPlist::commit() {
    if(m_pTexset){
        m_pTexset->commit();
        refreshParam();
    }
}

bool SVRResGLTexPlist::getbLoad(){
    if(m_pTexset){
        return m_pTexset->getbLoad();
    }
    return SVRResGLTex::getbLoad();
}

void SVRResGLTexPlist::bindTexset(SVRResGLTexSetPtr _texset){
    if(m_pTexset!=_texset){
        m_pTexset = _texset;
    }
}

void SVRResGLTexPlist::unbindTexset(){
    if(m_pTexset){
        m_pTexset = nullptr;
    }
}

SVRect *SVRResGLTexPlist::getSrcRect() {
    return &m_srcRect;
}

SVRect *SVRResGLTexPlist::getDstRect() {
    return &m_dstRect;
}

//
SVRResGLTexSet::SVRResGLTexSet(SVInst *mApp)
        : SVRResGLTex(mApp) {
    m_type = 2;
}

SVRResGLTexSet::~SVRResGLTexSet() {
}

SVRResGLTexiOS::SVRResGLTexiOS(SVInst *_app): SVRResGLTex(_app)
{
#ifdef SV_IOS
    m_pTexCacheRef = nullptr;
    m_pTexRef = nullptr;
    m_pPixelBuf = nullptr;
#endif
}

SVRResGLTexiOS::~SVRResGLTexiOS() {
}

void SVRResGLTexiOS::create(SVRendererBasePtr _renderer){
    if(!_renderer)
        return ;
    SVRObjBase::create(_renderer);
#ifdef SV_IOS
    SVContextBasePtr t_contextbase = _renderer->getRenderContext();
    CVEAGLContext t_glcontext = CVEAGLContext((__bridge id)t_contextbase->getContext());
    CVReturn t_flag = CVOpenGLESTextureCacheCreate(nullptr,
                                                   nullptr,
                                                   t_glcontext,
                                                   nullptr,
                                                   &m_pTexCacheRef);
    if (t_flag) {
        SV_LOG_ERROR("SVTextureIOS create texture cache failed");
    }
    const void *keys[] = {
        kCVPixelBufferOpenGLESCompatibilityKey,
        kCVPixelBufferIOSurfacePropertiesKey,
        kCVPixelBufferBytesPerRowAlignmentKey,
    };
    const void *values[] = {
        (__bridge const void *)([NSNumber numberWithBool:YES]),
        (__bridge const void *)([NSDictionary dictionary]),
        (__bridge const void *)([NSNumber numberWithInteger:16])
    };
    CFDictionaryRef opDic = CFDictionaryCreate(NULL, keys,
                                               values, 2,
                                               NULL, NULL);
    t_flag = CVPixelBufferCreate(kCFAllocatorDefault,
                                 m_width,
                                 m_height,
                                 kCVPixelFormatType_32BGRA,
                                 opDic,
                                 &m_pPixelBuf);
    if( t_flag == 0) {

    }
    t_flag = CVOpenGLESTextureCacheCreateTextureFromImage(nullptr,
                                                          m_pTexCacheRef,
                                                          m_pPixelBuf,
                                                          nullptr,
                                                          GL_TEXTURE_2D,
                                                          m_informate,
                                                          m_width,
                                                          m_height,
                                                          m_dataformate,
                                                          GL_UNSIGNED_BYTE,
                                                          0,
                                                          &m_pTexRef);
    if(t_flag == 0){
        m_id = CVOpenGLESTextureGetName(m_pTexRef);
        glBindTexture(GL_TEXTURE_2D , m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    CVOpenGLESTextureCacheFlush(m_pTexCacheRef,0);
    CFRelease(opDic);
    m_bLoad = true;
#endif
}

void SVRResGLTexiOS::destroy(SVRendererBasePtr _renderer){
#ifdef SV_IOS
    if(m_pPixelBuf){
        CFRelease(m_pPixelBuf);
        m_pPixelBuf = nullptr;
    }
    if(m_pTexRef){
        CFRelease(m_pTexRef);
        m_pTexRef = nullptr;
    }
    if(m_pTexCacheRef){
        CVOpenGLESTextureCacheFlush(m_pTexCacheRef,0);
        CFRelease(m_pTexCacheRef);
        m_pTexCacheRef = nullptr;
    }
#endif
    SVRObjBase::destroy(_renderer);
}

void SVRResGLTexiOS::pushData(u8* _srcPtr,s32 _w,s32 _h,s32 _pixelformate){
#ifdef SV_IOS
    if( (!m_pPixelBuf) || (!m_pTexRef) || (!m_pTexCacheRef) )
        return;
    if(CVPixelBufferLockBaseAddress(m_pPixelBuf, 0) == kCVReturnSuccess){
        s32 width = (s32)CVPixelBufferGetWidth(m_pPixelBuf);
        s32 height = (s32)CVPixelBufferGetHeight(m_pPixelBuf);
        s32 bytesPerRow = (s32)CVPixelBufferGetBytesPerRowOfPlane(m_pPixelBuf, 0);
        u8* _dstPtr = (u8*)CVPixelBufferGetBaseAddress(m_pPixelBuf);
        //判断从 CVImageBufferRef 里取出来的数据是否带panding
        if (bytesPerRow/4 - width !=0) {
            //带panding，要处理
            s32 a = 0;
            for (s32 i = 0; i < height; i++) {
                memcpy(_dstPtr +  a, _srcPtr + i * width * 4, width*4);
                a += bytesPerRow;
            }
        }else{
            memcpy(_dstPtr,_srcPtr,_w*_h*4);
        }
        
        CVPixelBufferUnlockBaseAddress(m_pPixelBuf, 0);
    }
#endif
}

void SVRResGLTexiOS::fetchData(u8* _dstPtr,s32 _w,s32 _h) {
#ifdef SV_IOS
    if( (!m_pPixelBuf) || (!m_pTexRef) || (!m_pTexCacheRef) )
        return;
    if(CVPixelBufferLockBaseAddress(m_pPixelBuf, 0) == kCVReturnSuccess){
        s32 width = (s32)CVPixelBufferGetWidth(m_pPixelBuf);
        s32 height = (s32)CVPixelBufferGetHeight(m_pPixelBuf);
        s32 bytesPerRow = (s32)CVPixelBufferGetBytesPerRowOfPlane(m_pPixelBuf, 0);
        u8* t_srtPtr = (u8*)CVPixelBufferGetBaseAddress(m_pPixelBuf);
        //判断从 CVImageBufferRef 里取出来的数据是否带panding
        if (bytesPerRow/4 - width !=0) {
            //带panding，要处理
            s32 a = 0;
            for (s32 i = 0; i < height; i++) {
                memcpy(_dstPtr + i * width * 4 , t_srtPtr + a, width*4);
                a += bytesPerRow;
            }
        }else{
            memcpy(_dstPtr,t_srtPtr,_w*_h*4);
        }

        CVPixelBufferUnlockBaseAddress(m_pPixelBuf, 0);
    }
#endif
}


//////////
//Shader
SVRResGLShader::SVRResGLShader(SVInst* _app)
        :SVRObjBase(_app){
    m_programm = 0;
    m_vs = 0;
    m_fs = 0;
    m_gs = 0;
    m_tsc = 0;
    m_tse = 0;
    m_cs = 0;
    m_use_tech = false;
}

SVRResGLShader::~SVRResGLShader(){
    m_programm = 0;
    m_vs = 0;
    m_fs = 0;
    m_gs = 0;
    m_tsc = 0;
    m_tse = 0;
    m_cs = 0;
}

void SVRResGLShader::create(SVRendererBasePtr _renderer) {
    SVRObjBase::create(_renderer);
    if( m_use_tech ) {
        _parseTech();
    } else {
        m_vs = _loadVS(m_vs_fname.c_str());
        m_fs = _loadFS(m_fs_fname.c_str());
        m_gs = _loadGS(m_gs_fname.c_str());
        m_tsc = _loadTSC(m_tsc_fname.c_str());
        m_tse = _loadTSE(m_tse_fname.c_str());
        m_cs = _loadCS(m_cs_fname.c_str());
        m_programm = _createProgram();
        _clearShaderRes();//生产program后就删除shader资源
    }
}

void SVRResGLShader::setProgrammeName(cptr8 _filename) {
    m_programme_fname = _filename;
}

cptr8 SVRResGLShader::getProgrammeName(){
    return m_programme_fname.c_str();
}

u32 SVRResGLShader::getProgramm(){
    return m_programm;
}

void SVRResGLShader::setTechFName(cptr8 _filename) {
    m_tech_fname = _filename;
    m_use_tech = true;
}

void SVRResGLShader::setVSFName(cptr8 _filename){
    m_vs_fname = _filename;
}

void SVRResGLShader::setFSFName(cptr8 _filename){
    m_fs_fname = _filename;
}

void SVRResGLShader::setGSFName(cptr8 _filename){
    m_gs_fname = _filename;
}

void SVRResGLShader::setCSFName(cptr8 _filename){
    m_cs_fname = _filename;
}

void SVRResGLShader::setTSCFName(cptr8 _filename){
    m_tsc_fname = _filename;
}

void SVRResGLShader::setTSEFName(cptr8 _filename){
    m_tse_fname = _filename;
}

//解析tech
bool SVRResGLShader::_parseTech() {
    SVDataChunk tDataStream;
    bool tflag = mApp->getFileMgr()->loadFileContentStr(&tDataStream, m_tech_fname.c_str());
    if (!tflag)
        return false;
    RAPIDJSON_NAMESPACE::Document t_doc;
    t_doc.Parse(tDataStream.m_data);
    if (t_doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = t_doc.GetParseError();
        SV_LOG_ERROR("rapidjson error code:%d \n", code);
        return false;
    }
    //
    if (t_doc.HasMember("version")) {
        RAPIDJSON_NAMESPACE::Value &version = t_doc["version"];
    }
    if (t_doc.HasMember("language")) {
        RAPIDJSON_NAMESPACE::Value &language = t_doc["language"];
    }
    //
    if (t_doc.HasMember("vs")) {
        RAPIDJSON_NAMESPACE::Value &vs = t_doc["vs"];
        if (vs.IsObject()) {
            SVString t_precision = SVString::null;
            SVString t_src = SVString::null;
            if (vs.HasMember("precision")) {
                RAPIDJSON_NAMESPACE::Value &t_value = vs["precision"];
                t_precision = t_value.GetString();
            }
            if (vs.HasMember("source")) {
                RAPIDJSON_NAMESPACE::Value &t_value = vs["source"];
                t_src = t_value.GetString();
            }
            //拼接字符串 这里需要
            m_vs = _loadTechVS(t_precision.c_str(),t_src.c_str());
        }
    }
    //
    if (t_doc.HasMember("fs")) {
        RAPIDJSON_NAMESPACE::Value &fs = t_doc["fs"];
        if (fs.IsObject()) {
            SVString t_precision = SVString::null;
            SVString t_src = SVString::null;
            if (fs.HasMember("precision")) {
                RAPIDJSON_NAMESPACE::Value &t_value = fs["precision"];
                t_precision = t_value.GetString();
            }
            if (fs.HasMember("source")) {
                RAPIDJSON_NAMESPACE::Value &t_value = fs["source"];
                t_src = t_value.GetString();
            }
            m_fs = _loadTechFS(t_precision.c_str(),t_src.c_str());
        }
    }
    //
    if (t_doc.HasMember("gs")) {
        RAPIDJSON_NAMESPACE::Value &gs = t_doc["gs"];
        if (gs.IsObject()) {
            SVString t_precision = SVString::null;
            SVString t_src = SVString::null;
            if (gs.HasMember("precision")) {
                RAPIDJSON_NAMESPACE::Value &t_value = gs["precision"];
                t_precision = t_value.GetString();
            }
            if (gs.HasMember("source")) {
                RAPIDJSON_NAMESPACE::Value &t_value = gs["source"];
                t_src = t_value.GetString();
            }
            m_gs = _loadTechGS(t_precision.c_str(),t_src.c_str());
        }
    }
    return true;
}

u32 SVRResGLShader::_loadTechVS(cptr8 _precision,cptr8 _src) {
    SVString t_source = _src;
#ifdef SV_ANDROID
    if(strcmp(_src,"lowp") == 0 ) {

    } else if(strcmp(_src,"mediump") == 0 ) {

    } else if(strcmp(_src,"highp") == 0 ) {

    }
#endif

#ifdef  SV_IOS
    if(strcmp(_src,"lowp") == 0 ) {

    } else if(strcmp(_src,"mediump") == 0 ) {

    } else if(strcmp(_src,"highp") == 0 ) {

    }
#endif

    return 0;
}

u32 SVRResGLShader::_loadTechFS(cptr8 _precision,cptr8 _src) {
    SVString t_source = _src;
#ifdef SV_ANDROID
    if(strcmp(_src,"lowp") == 0 ) {
        //t_source =
    } else if(strcmp(_src,"mediump") == 0 ) {

    } else if(strcmp(_src,"highp") == 0 ) {

    }
#endif

#ifdef SV_IOS
    if(strcmp(_src,"lowp") == 0 ) {
        //t_source =
    } else if(strcmp(_src,"mediump") == 0 ) {

    } else if(strcmp(_src,"highp") == 0 ) {

    }
#endif
    return 0;
}

u32 SVRResGLShader::_loadTechGS(cptr8 _precision,cptr8 _src) {
#ifdef SV_ANDROID
    if(strcmp(_src,"lowp") == 0 ) {

    } else if(strcmp(_src,"mediump") == 0 ) {

    } else if(strcmp(_src,"highp") == 0 ) {

    }
#endif
#ifdef SV_IOS
    if(strcmp(_src,"lowp") == 0 ) {

    } else if(strcmp(_src,"mediump") == 0 ) {

    } else if(strcmp(_src,"highp") == 0 ) {

    }
#endif
    return 0;
}

u32 SVRResGLShader::_loadVS(cptr8 _filename) {
    SVDataChunk tDataStream;
    u32 t_id=0;
    bool t_flag=false;
    if(!t_id){
        t_flag = mApp->getFileMgr()->loadFileContentStr(&tDataStream, _filename);
    }else{
        return t_id;
        
    }
    if (!t_flag)
        return 0;
    cptr8 vs_shader = tDataStream.m_data;
    u32 m_vs_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_vs_id, 1, &vs_shader, 0);
    glCompileShader(m_vs_id);

    GLint compileErr = 0;
    glGetShaderiv(m_vs_id, GL_COMPILE_STATUS, &compileErr);
    if (GL_FALSE == compileErr) {
        GLint logLen;
        glGetShaderiv(m_vs_id, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char *log = (char *) malloc(logLen);
            GLsizei written;
            glGetShaderInfoLog(m_vs_id, logLen, &written, log);
            SV_LOG_DEBUG("vs shader compile error log : \n %s fname:%s \n", log,
                         _filename/*[filename UTF8String]*/);
            free(log);
        }
        m_vs_id = 0;
    }else{
        SV_LOG_DEBUG("load fs: %s (%d) sucess\n", _filename, m_vs_id);
    }
    return m_vs_id;
}

u32 SVRResGLShader::_loadFS(cptr8 _filename){
    SVDataChunk tDataStream;
    u32 t_id=0;
    bool t_flag=false;
    if(!t_id){
        t_flag = mApp->getFileMgr()->loadFileContentStr(&tDataStream, _filename);
    }else{
        return t_id;
    }
    if (!t_flag)
        return 0;
    s32 t_error = 0;

    cptr8 fs_shader = tDataStream.m_data;
    u32 m_fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    t_error = glGetError();

    glShaderSource(m_fs_id, 1, &fs_shader, 0);
    t_error = glGetError();

    glCompileShader(m_fs_id);
    t_error = glGetError();

    GLint compileErr = 0;
    glGetShaderiv(m_fs_id, GL_COMPILE_STATUS, &compileErr);
    if (GL_FALSE == compileErr) {
        GLint logLen;
        glGetShaderiv(m_fs_id, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char *log = (char *) malloc(logLen);
            GLsizei written;
            glGetShaderInfoLog(m_fs_id, logLen, &written, log);
            SV_LOG_DEBUG("fs shader compile error log : \n %s fname:%s \n", log, _filename);
            free(log);
        }
        m_fs_id = 0;
    }else{
        SV_LOG_DEBUG("load fs: %s (%d) sucess\n", _filename, m_fs_id);
    }
    return m_fs_id;
}

u32 SVRResGLShader::_loadGS(cptr8 _filename){
    return 0;
}

u32 SVRResGLShader::_loadCS(cptr8 _filename){
    return 0;
}

u32 SVRResGLShader::_loadTSC(cptr8 _filename){
    return 0;
}

u32 SVRResGLShader::_loadTSE(cptr8 _filename){
    return 0;
}

u32 SVRResGLShader::_createProgram(){
    if (m_vs == 0 || m_fs == 0) {
        SV_LOG_DEBUG("create program fail, please check out shader:%s\n", m_programme_fname.c_str());
        return 0;
    }
    s32 t_error = 0;
    u32 t_program_id = glCreateProgram();
    glAttachShader(t_program_id, m_vs);
    glAttachShader(t_program_id, m_fs);
    //bind prop
    glBindAttribLocation(t_program_id, CHANNEL_POSITION, NAME_POSITION);
    glBindAttribLocation(t_program_id, CHANNEL_NORMAL, NAME_NORMAL);
    glBindAttribLocation(t_program_id, CHANNEL_COLOR0, NAME_COLOR);
    glBindAttribLocation(t_program_id, CHANNEL_COLOR1, NAME_COLOR1);
    glBindAttribLocation(t_program_id, CHANNEL_TEXCOORD0, NAME_TEXCOORD0);
    glBindAttribLocation(t_program_id, CHANNEL_TEXCOORD1, NAME_TEXCOORD1);
    glBindAttribLocation(t_program_id, CHANNEL_TEXCOORD2, NAME_TEXCOORD2);
    glBindAttribLocation(t_program_id, CHANNEL_TEXCOORD3, NAME_TEXCOORD3);
    //蒙皮动画相关
    glBindAttribLocation(t_program_id, CHANNEL_BONE_ID, NAME_BONE_ID);
    glBindAttribLocation(t_program_id, CHANNEL_BONE_WEIGHT, NAME_BONE_WEIGHT);
    //bind attri(new)
    glBindAttribLocation(t_program_id, CHANNEL_ATTRI_0, "s_attribute_0");
    glBindAttribLocation(t_program_id, CHANNEL_ATTRI_1, "s_attribute_1");
    glBindAttribLocation(t_program_id, CHANNEL_ATTRI_2, "s_attribute_2");
    glBindAttribLocation(t_program_id, CHANNEL_ATTRI_3, "s_attribute_3");
    glBindAttribLocation(t_program_id, CHANNEL_ATTRI_4, "s_attribute_4");
    glBindAttribLocation(t_program_id, CHANNEL_ATTRI_5, "s_attribute_5");
    glBindAttribLocation(t_program_id, CHANNEL_ATTRI_6, "s_attribute_6");
    glBindAttribLocation(t_program_id, CHANNEL_ATTRI_7, "s_attribute_7");
    //
    glLinkProgram(t_program_id);
    GLint linkstatus;
    glGetProgramiv(t_program_id, GL_LINK_STATUS, &linkstatus);
    if (linkstatus == GL_FALSE) {
        t_error = glGetError();
        GLint logLen = 1000;
        if (logLen > 0) {
            char *log = (char *) malloc(logLen);
            GLsizei written;
            glGetProgramInfoLog(t_program_id, logLen, &written, log);
            SV_LOG_DEBUG("link status:%s, error: %d\n", log, t_error);
            free(log);
        }
    } else {
        glUseProgram(t_program_id);
    }
    return t_program_id;
}

void SVRResGLShader::_clearShaderRes(){
    if(m_vs != 0) {
        glDeleteShader(m_vs);
        m_vs = 0;
    }
    if(m_fs != 0) {
        glDeleteShader(m_fs);
        m_fs = 0;
    }
    if(m_gs != 0) {
        glDeleteShader(m_gs);
        m_gs = 0;
    }
    if(m_cs != 0) {
        glDeleteShader(m_cs);
        m_cs = 0;
    }
    if(m_tsc != 0) {
        glDeleteShader(m_tsc);
        m_tsc = 0;
    }
    if(m_tse != 0) {
        glDeleteShader(m_tse);
        m_tse = 0;
    }
}

void SVRResGLShader::destroy(SVRendererBasePtr _renderer) {
    if(m_programm != 0){
        glDeleteProgram(m_programm);
    }
    SVRObjBase::destroy(_renderer);
}
//FBO资源
SVRResGLFBO::SVRResGLFBO(SVInst* _app):SVResFBO(_app),m_fboID(0)
        ,m_depthID(0)
        ,m_width(0)
        ,m_height(0)
        ,m_depth(false)
        ,m_stencil(false)
        ,m_dirty(false){

}
SVRResGLFBO::~SVRResGLFBO(){
}

void SVRResGLFBO::create(SVRendererBasePtr _renderer){
    SVRObjBase::create(_renderer);
    glGenFramebuffers(1,&m_fboID);
    _bindColor();
    _bindDepth();
    s32 t_error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(t_error){
        case GL_FRAMEBUFFER_COMPLETE:
            SV_LOG_INFO("GL_FRAMEBUFFER_COMPLETE");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            SV_LOG_INFO("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT    :
            SV_LOG_INFO("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            SV_LOG_INFO("GL_FRAMEBUFFER_UNSUPPORTED");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            SV_LOG_INFO("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
            break;
        case GL_FRAMEBUFFER_UNDEFINED :
            SV_LOG_INFO("GL_FRAMEBUFFER_UNDEFINED");
            break;
#if defined SV_OSX
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            SV_LOG_INFO("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            SV_LOG_INFO("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
            break;
#endif
    }
}

void SVRResGLFBO::destroy(SVRendererBasePtr _renderer){
    if(m_fboID>0) {
        glBindFramebuffer(GL_FRAMEBUFFER,m_fboID);
        if(m_stencil) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,0);
        }
        if(m_depth) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,0);
        }
        glDeleteRenderbuffers(1, &m_depthID);
        glDeleteFramebuffers(1,&m_fboID);
    }
    SVRObjBase::destroy(_renderer);
}

void SVRResGLFBO::_bindColor() {
}

void SVRResGLFBO::_bindDepth() {
    if(m_depth && m_width>0 && m_height>0) {
        glGenRenderbuffers(1, &m_depthID);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthID);
        if( m_stencil ) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthID);
        }
    }
}

void SVRResGLFBO::refresh() {

}

void SVRResGLFBO::bind() {
    //保存下当前的fbo
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_lastFboID);
    if(m_dirty){
        m_dirty = false;
        refresh();
    }
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        t_renderer->svBindFrameBuffer(m_fboID);
        t_renderer->svPushViewPort(0,0,m_width,m_height);
    }
}

void  SVRResGLFBO::clear(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        t_renderer->svBindClearColor(m_fboID);
    }
}

void SVRResGLFBO::unbind() {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        t_renderer->svPopViewPort();
        t_renderer->svBindFrameBuffer(m_lastFboID);
    }
    m_lastFboID = 0;
}

//
SVRResGLOutFBO::SVRResGLOutFBO(SVInst *_app,u32 _fboid)
:SVRResGLFBO(_app){
    m_fboID = _fboid;
}

SVRResGLOutFBO::~SVRResGLOutFBO() {
    m_fboID = 0;
}

void SVRResGLOutFBO::create(SVRendererBasePtr _renderer){
    //外部设置的FBO 直接使用外部的就好
}

void SVRResGLOutFBO::destroy(SVRendererBasePtr _renderer){
    m_fboID = 0;
}

//RenderTarget
//
SVResGLRenderTarget::SVResGLRenderTarget(SVInst *_app,s32 _w, s32 _h,bool _depth,bool _stencil)
        :SVRResGLFBO(_app){
    m_width = _w;
    m_height = _h;
    m_depth = _depth;
    m_stencil = _stencil;
    m_colorID = 0;
}

SVResGLRenderTarget::~SVResGLRenderTarget() {

}

void SVResGLRenderTarget::create(SVRendererBasePtr _renderer) {
    SVRResGLFBO::create(_renderer);
    if( m_fboID >0 ) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glGenRenderbuffers(1, &m_colorID);
        glBindRenderbuffer(GL_RENDERBUFFER, m_colorID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorID);
    }
}

void SVResGLRenderTarget::destroy(SVRendererBasePtr _renderer) {
    if( m_fboID >0 ) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1,&m_colorID);
        m_colorID = 0;
    }
    SVRResGLFBO::destroy(_renderer);
}
//
SVResGLRenderTargetOut::SVResGLRenderTargetOut(SVInst *_app,s32 _w, s32 _h,u32 _fboid,u32 _colorID)
        :SVResGLRenderTarget(_app,_w,_h,false,false){
    m_fboID = _fboid;
    m_colorID = _colorID;
}

SVResGLRenderTargetOut::~SVResGLRenderTargetOut(){
    m_width = 0;
    m_height = 0;
    m_fboID = 0;
    m_colorID = 0;
}

void SVResGLRenderTargetOut::create(SVRendererBasePtr _renderer) {
}

void SVResGLRenderTargetOut::destroy(SVRendererBasePtr _renderer) {
    m_fboID = 0;
    m_colorID = 0;
}
//
//RenderTexture
SVResGLRenderTexture::SVResGLRenderTexture(SVInst *_app,SVRResGLTexPtr _tex, bool _depth,bool _stencil)
:SVRResGLFBO(_app){
    if(_tex){
        m_tex = _tex;
        m_width = m_tex->getwidth();
        m_height = m_tex->getheight();
    }else{
        m_tex = nullptr;
        m_width = 0;
        m_height = 0;
    }
    m_depth = _depth;
    m_stencil = _stencil;
}

SVResGLRenderTexture::~SVResGLRenderTexture() {
}

void SVResGLRenderTexture::create(SVRendererBasePtr _renderer) {
    SVRResGLFBO::create(_renderer);
    m_dirty = true;
}

void SVResGLRenderTexture::_bindColor() {
    if( m_fboID>0 && m_tex) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_tex->getTexID(),0);
    }
}

void SVResGLRenderTexture::destroy(SVRendererBasePtr _renderer) {
    if(m_fboID>0) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,0,0);
    }
    m_tex = nullptr;
    SVRResGLFBO::destroy(_renderer);
}

void SVResGLRenderTexture::setTexture(SVRResGLTexPtr _tex) {
    if(m_tex != _tex){
        m_tex = _tex;
        if(m_tex) {
            //正常应该 reCreate才对
            m_width = m_tex->getwidth();
            m_height = m_tex->getheight();
        }
        m_dirty = true;
    }
}

void SVResGLRenderTexture::refresh() {
    if(m_fboID>0 && m_tex) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_tex->getTexID(),0);
    }
}

////////////////////////////////////////////////////////////////
//VBO资源
SVRResGLVBO::SVRResGLVBO(SVInst* _app)
        :SVRObjBase(_app){
}

SVRResGLVBO::~SVRResGLVBO(){
}

void SVRResGLVBO::create(SVRendererBasePtr _renderer) {
    SVRObjBase::create(_renderer);
}

void SVRResGLVBO::destroy(SVRendererBasePtr _renderer) {
    SVRObjBase::destroy(_renderer);
}

//mesh
s32 SVResGLRenderMesh::getVertexFormateSize(VFTYPE _type) {
    if (_type == E_VF_V2) {
        return sizeof(V2);
    } else if (_type == E_VF_V2_T0) {
        return sizeof(V2_T0);
    } else if (_type == E_VF_V2_T0_T1) {
        return sizeof(V2_T0_T1);
    } else if (_type == E_VF_V2_C) {
        return sizeof(V2_C);
    } else if (_type == E_VF_V2_C_T0) {
        return sizeof(V2_C_T0);
    } else if (_type == E_VF_V2_C_T0_T1) {
        return sizeof(V2_C_T0_T1);
    } else if (_type == E_VF_V3) {
        return sizeof(V3);
    } else if (_type == E_VF_V3_T0) {
        return sizeof(V3_T0);
    } else if (_type == E_VF_V3_T0_T1) {
        return sizeof(V3_T0_T1);
    } else if (_type == E_VF_V3_N) {
        return sizeof(V3_N);
    } else if (_type == E_VF_V3_N_T0) {
        return sizeof(V3_N_T0);
    } else if (_type == E_VF_V3_N_T0_T1) {
        return sizeof(V3_N_T0_T1);
    } else if (_type == E_VF_V3_C) {
        return sizeof(V3_C);
    } else if (_type == E_VF_V3_C_T0) {
        return sizeof(V3_C_T0);
    } else if (_type == E_VF_V3_C_T0_T1) {
        return sizeof(V3_C_T0_T1);
    } else if (_type == E_VF_V3_N_C) {
        return sizeof(V3_N_C);
    } else if (_type == E_VF_V3_N_C_T0) {
        return sizeof(V3_N_C_T0);
    } else if (_type == E_VF_V3_N_C_T0_T1) {
        return sizeof(V3_N_C_T0_T1);
    } else if (_type == E_VF_V3_N_T0_BONE_W) {
        return sizeof(V3_N_T0_BONE4);
    } else if (_type == E_VF_V3_PARTICLE) {
        return sizeof(V3_PARTICLE);
    }
    return 0;
}

//
SVResGLRenderMesh::SVResGLRenderMesh(SVInst* _app)
        :SVRResGLVBO(_app){
            _reset();
}

SVResGLRenderMesh::~SVResGLRenderMesh() {
    _reset();
}

void SVResGLRenderMesh::_reset(){
    m_vaoID = 0;
    m_vboID = 0;
    m_indexID = 0;
    m_indexNum = 0;
    m_pointNum = 0;
    m_vftype = E_VF_V3;
    m_drawmethod = E_DM_TRIANGLES;
    m_vertPoolType = GL_STATIC_DRAW;
    m_indexPoolType = GL_STATIC_DRAW;
    m_useVAO = false;
}

void SVResGLRenderMesh::create(SVRendererBasePtr _renderer){
    SVRResGLVBO::create(_renderer);
}

void SVResGLRenderMesh::destroy(SVRendererBasePtr _renderer) {
    if (m_indexID > 0) {
        glDeleteBuffers(1, &m_indexID);
        m_indexID = 0;
    }
    if (m_vboID > 0) {
        glDeleteBuffers(1, &m_vboID);
        m_vboID = 0;
    }
    SVRResGLVBO::destroy(_renderer);
}

void SVResGLRenderMesh::setIndexPoolType(u32 itype) {
    if (itype == GL_STREAM_DRAW || itype == GL_STATIC_DRAW || itype == GL_DYNAMIC_DRAW) {
        m_indexPoolType = itype;
    } else {
        SV_LOG_ERROR("error parameter: SVResGLRenderMesh setIndexPoolType \n");
    }
}

void SVResGLRenderMesh::setVertexPoolType(u32 vtype) {
    if (vtype == GL_STREAM_DRAW || vtype == GL_STATIC_DRAW || vtype == GL_DYNAMIC_DRAW) {
        m_vertPoolType = vtype;
    } else {
        SV_LOG_ERROR("error parameter: SVResGLRenderMesh setVertexPoolType \n");
    }
}

void SVResGLRenderMesh::setDrawMethod(DRAWMETHOD drawtype) {
    m_drawmethod = drawtype;
}

void SVResGLRenderMesh::setVertexType(VFTYPE type) {
    m_vftype = type;
}

void SVResGLRenderMesh::setIndexData(SVDataSwapPtr _data,s32 _num){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(_data && t_renderer){
        if(_num>m_indexNum) {
            if (m_indexID > 0) {
                glDeleteBuffers(1, &m_indexID);
                m_indexID = 0;
            }
        }
        if(m_indexID==0) {
            glGenBuffers(1, &m_indexID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _data->getSize(), _data->getData(),m_indexPoolType);
        }else{
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexID);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _data->getSize(), _data->getData() );
        }
    }
}

void SVResGLRenderMesh::setVertexDataNum(s32 _vertexNum){
    if( m_pointNum > 0 && m_pointNum < _vertexNum ){
        if (m_vboID > 0) {
            glDeleteBuffers(1, &m_vboID);
            m_vboID = 0;
        }
    }
    m_pointNum = _vertexNum;
}

void SVResGLRenderMesh::setVertexData(SVDataSwapPtr _data){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(_data && t_renderer){
        if(m_vboID == 0) {
            glGenBuffers(1, &m_vboID);
            glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
            glBufferData(GL_ARRAY_BUFFER,_data->getSize(), _data->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _data->getSize(), _data->getData() );
        }
    }
}

void SVResGLRenderMesh::render() {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer) {
        return ;
    }
    if(m_useVAO) {
        //使用vao
        if(m_indexID>0) {
            //索引模式
        }else{
            //非索引模式
            glDrawArrays(m_drawmethod, 0, m_pointNum);
        }
    }else{
        //使用vbo
        if(m_vboID>0) {
            if(m_indexID>0) {
                //索引绘制
                //因为索引这块可能大于65536 所以需要分批渲染 by fyz
                t_renderer->svBindIndexBuffer(m_indexID);
                t_renderer->svBindVertexBuffer(m_vboID);
                _updateVertDsp();
                glDrawElements(m_drawmethod, m_indexNum, GL_UNSIGNED_SHORT, 0);//NUM_FACE_MESHVER
                t_renderer->svBindVertexBuffer(0);
                t_renderer->svBindIndexBuffer(0);
            } else {
                //非索引绘制
                t_renderer->svBindVertexBuffer(m_vboID);
                _updateVertDsp();
                glDrawArrays(m_drawmethod, 0, m_pointNum);
                t_renderer->svBindVertexBuffer(0);
            }
        }
    }
}

void SVResGLRenderMesh::_updateVertDsp() {
     SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        t_renderer->svUpdateVertexFormate(m_vftype);
    }
}

void SVResGLRenderMesh::_bindVerts(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        if(m_indexID>0){
            t_renderer->svBindIndexBuffer(m_indexID);
        }else{
            t_renderer->svBindIndexBuffer(0);
        }
        //
        if(m_vboID>0){
            t_renderer->svBindVertexBuffer(m_vboID);
        }else{
            t_renderer->svBindVertexBuffer(0);
        }
    }
}

void SVResGLRenderMesh::_unbindVerts(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        for(s32 i=0;i<8;i++){
            glDisableVertexAttribArray(i);
        }
        if(m_indexID>0){
            t_renderer->svBindIndexBuffer(0);
        }
        if(m_vboID>0){
            t_renderer->svBindVertexBuffer(0);
        }
    }
}

//
SVResGLRenderMeshDvid::SVResGLRenderMeshDvid(SVInst* _app)
        :SVResGLRenderMesh(_app){
            _reset();
}

SVResGLRenderMeshDvid::~SVResGLRenderMeshDvid() {
}

void SVResGLRenderMeshDvid::_reset(){
    SVResGLRenderMesh::_reset();
    vertex2ID_0 = 0;
    vertex2ID_1 = 0;
    vertex2ID_2 = 0;
    vertex3ID = 0;
    color0ID = 0;
    color1ID = 0;
    texcoord0ID = 0;
    texcoord1ID = 0;
    texcoord2ID = 0;
    normalID = 0;
    tagentID = 0;
    btagentID = 0;
}

void SVResGLRenderMeshDvid::create(SVRendererBasePtr _renderer){
    SVResGLRenderMesh::create(_renderer);
}

void SVResGLRenderMeshDvid::destroy(SVRendererBasePtr _renderer) {
    SVResGLRenderMesh::destroy(_renderer);
    if (vertex2ID_0 != 0){
        glDeleteBuffers(1, &vertex2ID_0);
        vertex2ID_0 = 0;
    }
    if (vertex3ID != 0){
        glDeleteBuffers(1, &vertex3ID);
        vertex3ID = 0;
    }
    if (color0ID != 0){
        glDeleteBuffers(1, &color0ID);
        color0ID = 0;
    }
    if (color1ID != 0){
        glDeleteBuffers(1, &color1ID);
        color1ID = 0;
    }
    if (texcoord0ID != 0){
        glDeleteBuffers(1, &texcoord0ID);
        texcoord0ID = 0;
    }
    if (texcoord1ID != 0){
        glDeleteBuffers(1, &texcoord1ID);
        texcoord1ID = 0;
    }
    if (texcoord2ID != 0){
        glDeleteBuffers(1, &texcoord2ID);
        texcoord2ID = 0;
    }
    if (normalID != 0){
        glDeleteBuffers(1, &normalID);
        normalID = 0;
    }
    if (tagentID != 0){
        glDeleteBuffers(1, &tagentID);
        tagentID = 0;
    }
    if (btagentID != 0){
        glDeleteBuffers(1, &btagentID);
        btagentID = 0;
    }
}

//流格式
void SVResGLRenderMeshDvid::_updateVertDsp() {
    if (m_vftype & D_VF_V2) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex2ID_0);
        glEnableVertexAttribArray(CHANNEL_POSITION);
        glVertexAttribPointer(CHANNEL_POSITION, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_V3) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex3ID);
        glEnableVertexAttribArray(CHANNEL_POSITION);
        glVertexAttribPointer(CHANNEL_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_NOR) {
        glBindBuffer(GL_ARRAY_BUFFER, normalID);
        glEnableVertexAttribArray(CHANNEL_NORMAL);
        glVertexAttribPointer(CHANNEL_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_C0) {
        glBindBuffer(GL_ARRAY_BUFFER, color0ID);
        glEnableVertexAttribArray(CHANNEL_COLOR0);
        glVertexAttribPointer(CHANNEL_COLOR0, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_C1) {
        glBindBuffer(GL_ARRAY_BUFFER, color1ID);
        glEnableVertexAttribArray(CHANNEL_COLOR1);
        glVertexAttribPointer(CHANNEL_COLOR1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_T0) {
        glBindBuffer(GL_ARRAY_BUFFER, texcoord0ID);
        glEnableVertexAttribArray(CHANNEL_TEXCOORD0);
        glVertexAttribPointer(CHANNEL_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_T1) {
        glBindBuffer(GL_ARRAY_BUFFER, texcoord1ID);
        glEnableVertexAttribArray(CHANNEL_TEXCOORD1);
        glVertexAttribPointer(CHANNEL_TEXCOORD1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (m_vftype & D_VF_T2) {
        glBindBuffer(GL_ARRAY_BUFFER, texcoord2ID);
        glEnableVertexAttribArray(CHANNEL_TEXCOORD2);
        glVertexAttribPointer(CHANNEL_TEXCOORD2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
}

void SVResGLRenderMeshDvid::setVertex2Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(vertex2ID_0 == 0) {
            glGenBuffers(1, &vertex2ID_0);
            glBindBuffer(GL_ARRAY_BUFFER, vertex2ID_0);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, vertex2ID_0);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setVertex3Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(vertex3ID == 0) {
            glGenBuffers(1, &vertex3ID);
            glBindBuffer(GL_ARRAY_BUFFER, vertex3ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, vertex3ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setColor0Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(color0ID == 0) {
            glGenBuffers(1, &color0ID);
            glBindBuffer(GL_ARRAY_BUFFER, color0ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, color0ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setColor1Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(color1ID == 0) {
            glGenBuffers(1, &color1ID);
            glBindBuffer(GL_ARRAY_BUFFER, color1ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, color1ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setTexcoord0Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(texcoord0ID == 0) {
            glGenBuffers(1, &texcoord0ID);
            glBindBuffer(GL_ARRAY_BUFFER, texcoord0ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, texcoord0ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setTexcoord1Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(texcoord1ID == 0) {
            glGenBuffers(1, &texcoord1ID);
            glBindBuffer(GL_ARRAY_BUFFER, texcoord1ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, texcoord1ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setTexcoord2Data(SVDataSwapPtr _pdata){
    if(_pdata){
        if(texcoord2ID == 0) {
            glGenBuffers(1, &texcoord2ID);
            glBindBuffer(GL_ARRAY_BUFFER, texcoord2ID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, texcoord2ID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setNormalData(SVDataSwapPtr _pdata){
    if(_pdata){
        if(normalID == 0) {
            glGenBuffers(1, &normalID);
            glBindBuffer(GL_ARRAY_BUFFER, normalID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, normalID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setTagentData(SVDataSwapPtr _pdata){
    if(_pdata){
        if(tagentID == 0) {
            glGenBuffers(1, &tagentID);
            glBindBuffer(GL_ARRAY_BUFFER, tagentID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, tagentID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::setBTagentData(SVDataSwapPtr _pdata){
    if(_pdata){
        if(btagentID == 0) {
            glGenBuffers(1, &btagentID);
            glBindBuffer(GL_ARRAY_BUFFER, btagentID);
            glBufferData(GL_ARRAY_BUFFER,_pdata->getSize(), _pdata->getData(),m_vertPoolType);
        }else{
            glBindBuffer(GL_ARRAY_BUFFER, btagentID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, _pdata->getSize(), _pdata->getData());
        }
    }
}

void SVResGLRenderMeshDvid::render(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRendererGLPtr t_rendererGL = std::dynamic_pointer_cast<SVRendererGL>(t_renderer);
    if(t_rendererGL) {
        _updateVertDsp();
        _bindVerts();
        if ( m_indexID>0 ) {
            glDrawElements(m_drawmethod, m_indexNum, GL_UNSIGNED_SHORT, 0);//NUM_FACE_MESHVER
        } else {
            glDrawArrays(m_drawmethod, 0, m_pointNum);
        }
        _unbindVerts();
    }
}

//VAO资源
SVRResGLVAO::SVRResGLVAO(SVInst* _app)
        :SVRObjBase(_app){
}

SVRResGLVAO::~SVRResGLVAO(){
}

void SVRResGLVAO::create(SVRendererBasePtr _renderer) {
}

void SVRResGLVAO::destroy(SVRendererBasePtr _renderer) {
}


