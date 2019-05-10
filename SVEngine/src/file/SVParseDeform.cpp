//
//  SVParseDeform.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/10/29.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVParseDeform.h"
#include "../core/SVDeformImageMove.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderMgr.h"
#include "../operate/SVOpParse.h"
#include "../work/SVThreadPool.h"
#include "../file/SVFileMgr.h"
#include "../base/SVDataChunk.h"
#include "../app/SVInst.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"


SVDeformImageMovePtr SVParseDeform::parseDeform(SVInst *app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path){
    SVDeformImageMovePtr m_deform=MakeSharedPtr<SVDeformImageMove>(app);
    SVTexturePtr t_innerTex = app->getRenderer()->getSVTex(E_TEX_MAIN);
    m_deform->init(t_innerTex,t_innerTex);
    m_deform->setIsDetect(true);
    // m_deform->setvisiPoint(true);
    m_deform->setFlip(true);
    m_deform->m_rule=0;
    m_deform->fromJSON(item);
    return m_deform;
}

void SVParseDeform::parse(SVInst *app,cptr8 path, s32 resid,SVDeformImageMovePtr _deform) {
    RAPIDJSON_NAMESPACE::Document t_doc;
    //解析效果包
    SVString t_path = SVString(path) + SVString("/");
    SV_LOG_INFO("SVParseMain::parse begin\n");
    SVString config_name = t_path + "config.json";
    SVDataChunk tDataStream;
    SV_LOG_ERROR("SVParseMain::parse begin config name %s\n", config_name.get());
    bool tflag = app->m_pGlobalMgr->m_pFileMgr->loadFileContentStr(&tDataStream, config_name.get());
    if (!tflag)
        return ;
    SV_LOG_ERROR("SVParseMain::load effect sucess\n");
    SV_LOG_ERROR("filedata %s\n", tDataStream.m_data);
    if (!tDataStream.m_data) {
        SV_LOG_ERROR("data stream is null");
        return ;
    }
    
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_ERROR("rapidjson error code:%d \n", code);
        return ;
    }
    
    if (doc.HasMember("SVDeform") && doc["SVDeform"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &deformObj = doc["SVDeform"];
        _deform->fromJSON(deformObj);
    }
    SV_LOG_ERROR("SVParseMain::parse end\n");
}
