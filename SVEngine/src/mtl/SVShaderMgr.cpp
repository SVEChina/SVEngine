//
// SVShaderMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVShaderMgr.h"
#include "../file/SVFileMgr.h"
#include "../base/SVDataChunk.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalParam.h"
#include "../third/rapidjson/document.h"
#include "../third/rapidjson/stringbuffer.h"
#include "../third/rapidjson/writer.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVGL/SVRResGL.h"
#include "../rendercore/SVMetal/SVResMetal.h"

SVShaderMgr::SVShaderMgr(SVInst *_app)
:SVSysBase(_app) {
}

SVShaderMgr::~SVShaderMgr() {
}

void SVShaderMgr::init() {
    loadAllShader();    //加载表
}

void SVShaderMgr::destroy() {
    clearAllShader();
}

void SVShaderMgr::loadAllShader() {
    SVDataChunk tDataStream;
    SV_LOG_ERROR("load shadercfg.json begin\n");
    bool tflag = mApp->getFileMgr()->loadFileContentStr(&tDataStream, "shader/shadercfg.json");
    if (!tflag) {
        SV_LOG_INFO("not find shadercfg.json!please check shader file path!\n");
        return;
    }
    SV_LOG_ERROR("file context %s \n", tDataStream.m_data);
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse<0>(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_ERROR("rapidjson error code:%d \n", code);
        return;
    }
    //
    RAPIDJSON_NAMESPACE::Value &shader = doc["shader"];
    if (shader.IsArray()) {
        for (s32 i = 0; i < shader.Size(); ++i) {
            RAPIDJSON_NAMESPACE::Value &shaderitem = shader[i];
            assert(shaderitem.IsObject());
            SVString t_vs_fname;
            SVString t_fs_fname;
            SVString t_gs_fname;
            SVString t_cs_fname;
            SVString t_tsc_fname;
            SVString t_tse_fname;
            SVString t_programme_name;
            if (shaderitem.HasMember("vs") && shaderitem["vs"].IsString()) {
                t_vs_fname = shaderitem["vs"].GetString();
                t_vs_fname = SVString("shader/") + t_vs_fname + ".vs";
            }
            if (shaderitem.HasMember("fs") && shaderitem["fs"].IsString()) {
                t_fs_fname = shaderitem["fs"].GetString();
                t_fs_fname = SVString("shader/") + t_fs_fname + ".fs";
            }
            if (shaderitem.HasMember("programme") && shaderitem["programme"].IsString()) {
                t_programme_name = shaderitem["programme"].GetString();
            }
            
            SVResShaderPtr t_resShader = nullptr;
            if( mApp->m_pGlobalParam->getEngCore() == SV_E_CORE_GL ) {
                t_resShader = MakeSharedPtr<SVRResGLShader>(mApp);
            }else if(mApp->m_pGlobalParam->getEngCore() == SV_E_CORE_METAL){
                t_resShader = MakeSharedPtr<SVRResMetalShader>(mApp);
            }else if(mApp->m_pGlobalParam->getEngCore() == SV_E_CORE_VULKAN){
                //t_resShader = MakeSharedPtr<SVRResGLShader>(mApp);
            }
            //
            if(t_resShader) {
                ShaderMap.append(t_programme_name, t_resShader);
                t_resShader->setVSFName(t_vs_fname.c_str());
                t_resShader->setFSFName(t_fs_fname.c_str());
                t_resShader->setProgrammeName(t_programme_name.c_str());
                mApp->getRenderMgr()->pushRCmdCreate(t_resShader);
                SV_LOG_ERROR("shader : vs(%s) fs(%s)\n", t_vs_fname.c_str(),t_fs_fname.c_str());
            }
        }
    }
    SV_LOG_DEBUG("load shader end\n");
}

void SVShaderMgr::clearAllShader() {
    ShaderMap.clear();
}

u32 SVShaderMgr::getProgramme(cptr8 _name) {
    SHADERPOOL::Iterator it = ShaderMap.find(_name);
    if(it!=ShaderMap.end()) {
        SVResShaderPtr t_resShader = it->data;
        if (t_resShader) {
            u32 t_programmeID = t_resShader->getProgramm();
            if (t_programmeID == 0) {
                SV_LOG_DEBUG("create shader program:%s error!\n", t_resShader->getProgrammeName());
                return 0;
            }
            return t_programmeID;
        }
    }
    return 0;
}

