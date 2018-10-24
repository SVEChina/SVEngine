//
//  SVParseLUTFilter.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/10/23.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVParseLUTFilter.h"
#include "SVFilterLUT.h"
#include "../../app/SVGlobalMgr.h"
#include "../../file/SVFileMgr.h"
#include "../../base/SVDataChunk.h"
#include "../../app/SVInst.h"

SVParseLUTFilter::SVParseLUTFilter(SVInst *_app)
:SVParseData(_app){
}

SVParseLUTFilter::~SVParseLUTFilter(){
}

SVFilterLUTPtr SVParseLUTFilter::parse(cptr8 _path, s32 resid){
    RAPIDJSON_NAMESPACE::Document t_doc;
    //解析滤镜包

    SVDataChunk tDataStream;
    bool tflag = mApp->m_pGlobalMgr->m_pFileMgr->loadFileContentStr(&tDataStream, _path);
    if (!tflag)
    return nullptr;
    SV_LOG_ERROR("SVParseMain::load effect sucess\n");
 //   SV_LOG_ERROR("filedata %s\n", tDataStream.m_data);
    if (!tDataStream.m_data) {
        SV_LOG_ERROR("data stream is null");
        return nullptr;
    }
    
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_ERROR("rapidjson error code:%d \n", code);
        return nullptr;
    }
    
    if (doc.HasMember("version")) {
        RAPIDJSON_NAMESPACE::Value &version = doc["version"];
    }
   
    if(doc.HasMember("filterLUT")&& doc["filterLUT"].IsObject()){
        RAPIDJSON_NAMESPACE::Value &t_lut = doc["filterLUT"];
        SVFilterLUTPtr t_filterLUT = MakeSharedPtr<SVFilterLUT>(mApp);
        t_filterLUT->fromJSON(t_lut);
        return t_filterLUT;
    }
    
    return nullptr;
}
