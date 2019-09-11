//
// SVConfig.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVConfig.h"
#include "../file/SVFileMgr.h"
#include "../app/SVInst.h"
#include "../base/SVDataChunk.h"
#include "../third/rapidjson/document.h"
#include "../third/rapidjson/stringbuffer.h"
#include "../third/rapidjson/writer.h"

SVConfig::SVConfig(SVInst *_app) : SVGBase(_app) {
    //m_subsysType = "SVConfig";
    width = 720;
    height = 1280;
    designWidth = 720;
    designHeight = 1280;
    designImageWidth = 720;
    designImageHeight = 1280;
    cameraWidth = 1280;
    cameraHeight = 720;
    cameraAngle = 90;
    cameraFormate = SV_PF_NULL;
    dataoutFormate = SV_OUT_STEAM_NULL;
    m_designAdaptmode = E_APT_M_FULL;
    glVersion = 0x00020000;
    detectType = DETECT_T_NULL;
    m_tag = "svengine";
}

SVConfig::~SVConfig() {

}

void SVConfig::init() {
    detectType = DETECT_T_NULL;
    //加载系统的配置文件
    width = 720;
    height = 1280;
    //设计分辨率
    designWidth = 1080;
    designHeight = 1920;
    //
    designImageWidth = 1080;
    designImageHeight = 1920;
    //相机
    cameraWidth = 1280;
    cameraHeight = 720;
    cameraAngle = 90;
    cameraMirror = false;
    mirror = true;
    cameraFormate = SV_PF_NULL;
    //数据流输出格式
    dataoutFormate = SV_OUT_STEAM_NULL;
    m_designAdaptmode = E_APT_M_FULL;
    m_designAdaptScale = 1.0f;
}

void SVConfig::destroy() {
    width = 720;
    height = 1280;
    designWidth = 720;
    designHeight = 1280;
    designImageWidth = 700;
    designImageHeight = 1004;
    cameraWidth = 1280;
    cameraHeight = 720;
    cameraAngle = 90;
    cameraMirror = false;
    mirror = true;
    cameraFormate = SV_PF_NULL;
    dataoutFormate = SV_OUT_STEAM_NULL;
    m_designAdaptmode = E_APT_M_FULL;
    glVersion = 0x00020000;
}

s32 SVConfig::getRealCameraWidth() {
    if (cameraAngle == 0)
        return cameraWidth;
    else if (cameraAngle == 90)
        return cameraHeight;
    else if (cameraAngle == 180)
        return cameraWidth;
    else if (cameraAngle == 270)
        return cameraHeight;
    return cameraWidth;
}

s32 SVConfig::getRealCameraHeight() {
    if (cameraAngle == 0)
        return cameraHeight;
    else if (cameraAngle == 90)
        return cameraWidth;
    else if (cameraAngle == 180)
        return cameraHeight;
    else if (cameraAngle == 270)
        return cameraWidth;
    return cameraHeight;
}

void SVConfig::loadConfig() {
    //加载配置文件
    SVDataChunk tDataStream;
    bool tflag = mApp->getFileMgr()->loadFileContentStr(&tDataStream, "svres/fmcfg.json");
    if (!tflag)
        return ;
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
    if (doc.HasMember("param")) {
        RAPIDJSON_NAMESPACE::Value &t_param = doc["param"];
        if(t_param.IsObject()){
            if (t_param.HasMember("detecttype")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["detecttype"];
                detectType = (DETECTTYPE)t_value.GetInt();
            }
            if (t_param.HasMember("design_width")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["design_width"];
                designWidth = t_value.GetInt();
            }
            if (t_param.HasMember("design_height")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["design_height"];
                designHeight = t_value.GetInt();
            }
            if (t_param.HasMember("design_img_width")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["design_img_width"];
                cameraWidth = t_value.GetInt();
            }
            if (t_param.HasMember("design_img_height")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["design_img_height"];
                cameraHeight = t_value.GetInt();
            }
            if (t_param.HasMember("camera_width")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["camera_width"];
                cameraWidth = t_value.GetInt();
            }
            if (t_param.HasMember("camera_height")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["camera_height"];
                cameraHeight = t_value.GetInt();
            }
            if (t_param.HasMember("camera_angle")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["camera_angle"];
                cameraAngle = t_value.GetFloat();
            }
            if (t_param.HasMember("designadaptmode")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["designadaptmode"];
                m_designAdaptmode = (SV_DESIGN_ADAPT_MODE)t_value.GetInt();
            }
            if (t_param.HasMember("glversion")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["glversion"];
                glVersion = t_value.GetInt();
            }
            if (t_param.HasMember("usepbo")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["usepbo"];
//                designWidth = t_value.GetInt();
            }
            if (t_param.HasMember("stroke_width") && t_param["stroke_width"].IsFloat()) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["stroke_width"];
                m_strokeWidth = t_value.GetFloat();
            }
            if (t_param.HasMember("stroke_color") && t_param["stroke_color"].IsArray()) {
                RAPIDJSON_NAMESPACE::Value &t_values = t_param["stroke_color"];
                if (t_values.Size() > 3) {
                    m_strokeColor.x = t_values[0].GetFloat();
                    m_strokeColor.y = t_values[1].GetFloat();
                    m_strokeColor.z = t_values[2].GetFloat();
                    m_strokeColor.w = t_values[3].GetFloat();
                }
            }
            if (t_param.HasMember("stroke_glowidth") && t_param["stroke_glowidth"].IsFloat()) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["stroke_glowidth"];
                m_strokeGlowWidth = t_value.GetFloat();
            }
            if (t_param.HasMember("stroke_glowcolor") && t_param["stroke_glowcolor"].IsArray()) {
                RAPIDJSON_NAMESPACE::Value &t_values = t_param["stroke_glowcolor"];
                if (t_values.Size() > 3) {
                    m_strokeGlowColor.x = t_values[0].GetFloat();
                    m_strokeGlowColor.y = t_values[1].GetFloat();
                    m_strokeGlowColor.z = t_values[2].GetFloat();
                    m_strokeGlowColor.w = t_values[3].GetFloat();
                }
            }
        }
    }
    if (doc.HasMember("filter")) {
        RAPIDJSON_NAMESPACE::Value &t_filter = doc["filter"];
        if(t_filter.IsObject()){
            if (t_filter.HasMember("2dmask")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["2dmask"];
                m_cfgFilter.m_2dmask = t_value.GetInt();
            }
            if (t_filter.HasMember("3dmask")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["3dmask"];
                m_cfgFilter.m_3dmask = t_value.GetInt();
            }
            if (t_filter.HasMember("2dbone")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["2dbone"];
                m_cfgFilter.m_2dbone = t_value.GetInt();
            }
            if (t_filter.HasMember("3dbone")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["3dbone"];
                m_cfgFilter.m_3dbone = t_value.GetInt();
            }
            if (t_filter.HasMember("2dframe")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["2dframe"];
                m_cfgFilter.m_2dframe = t_value.GetInt();
            }
            if (t_filter.HasMember("distorb")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["distorb"];
                m_cfgFilter.m_distorb = t_value.GetInt();
            }
            if (t_filter.HasMember("gpumorph")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["gpumorph"];
                m_cfgFilter.m_gpumorph = t_value.GetInt();
            }
            if (t_filter.HasMember("morph")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["morph"];
                m_cfgFilter.m_morph = t_value.GetInt();
            }
            if (t_filter.HasMember("particle")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["particle"];
                m_cfgFilter.m_particle = t_value.GetInt();
            }
            if (t_filter.HasMember("sky")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["sky"];
                m_cfgFilter.m_sky = t_value.GetInt();
            }
            if (t_filter.HasMember("audio")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_filter["audio"];
                m_cfgFilter.m_audio = t_value.GetInt();
            }
        }
    }
    _adaptScale();
    SV_LOG_ERROR("SVConfig::_loadConfig\n");
}

void SVConfig::setCameraDsp(s32 inCameraWidth, s32 inCameraHeight, s32 inCameraAngle) {
    cameraWidth = inCameraWidth;
    cameraHeight = inCameraHeight;
    cameraAngle = inCameraAngle;
    _adaptScale();
}

void SVConfig::_adaptScale() {
    f32 t_ratio_design = designImageWidth*1.0f/designImageHeight;
    f32 t_ratio_camera = cameraWidth*1.0f/cameraHeight;
    f32 t_scale_width = cameraWidth*1.0f/designImageWidth;
    f32 t_scale_height = cameraHeight*1.0f/designImageHeight;
    switch (m_designAdaptmode) {
        case E_APT_M_BODER: {
            m_designAdaptScale = t_ratio_design <= t_ratio_camera ? t_scale_height : t_scale_width;
            break;
        }
        case E_APT_M_FULL: {
            m_designAdaptScale = t_ratio_design <= t_ratio_camera ? t_scale_width : t_scale_height;
            break;
        }
        default:
            break;
    }

}
