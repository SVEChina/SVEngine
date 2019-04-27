//
// SVRResGLShader.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRResGLShader.h"
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
#include "../SVContextBase.h"
#include "../SVRendererBase.h"
#include "../SVRenderState.h"
#include "../SVGL/SVRendererGL.h"

/*
GL Shader
*/

SVRResGLShader::SVRResGLShader(SVInst* _app)
:SVResShader(_app){
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

void SVRResGLShader::setTechFName(cptr8 _filename) {
    m_tech_fname = _filename;
    m_use_tech = true;
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
            SV_LOG_DEBUG("fs shader compile error log : \n %s fname:%s \n", log, _filename);            free(log);
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
    glBindAttribLocation(t_program_id, CHANNEL_INSOFFSET, NAME_INSOFFSET);
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

bool SVRResGLShader::active(SVRendererBasePtr _render) {
    SVRenderStatePtr t_state = _render->getState();
    if(m_programm>0) {
        glUseProgram(m_programm);
        t_state->m_shaderID = m_programm;
        return true;
    }
    t_state->m_shaderID = 0;
    return false;
}
