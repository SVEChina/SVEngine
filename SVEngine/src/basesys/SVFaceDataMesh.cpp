//
// SVFaceDataMesh.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFaceDataMesh.h"
#include "../app/SVInst.h"
#include "../base/SVDataChunk.h"
#include "../file/SVFileMgr.h"
#include "../third/rapidjson/document.h"
#include "../third/rapidjson/stringbuffer.h"
#include "../third/rapidjson/writer.h"

SVFaceDataMesh::SVFaceDataMesh(SVInst *_app)
:SVGBase(_app){
    
}
//
void SVFaceDataMesh::init(){
    //加载as标准脸数据
    _loadFaceMeshData("facemesh_as");
    _loadStanderFaceData("standerface_as");
    _loadFaceMeshData("facemesh_as_simplify");
    _loadStanderFaceData("standerface_as_simplify");
    //加载st标准脸数据
    _loadFaceMeshData("facemesh_st");
    _loadStanderFaceData("standerface_st");
    _loadFaceMeshData("facemesh_st_simplify");
    _loadStanderFaceData("standerface_st_simplify");
    _loadFaceMeshData("facemesh_st_normal");
    _loadStanderFaceData("standerface_st_normal");
    //加载fp标准脸数据
    _loadFaceMeshData("facemesh_fp");
    _loadStanderFaceData("standerface_fp");
    _loadFaceMeshData("facemesh_fp_simplify");
    _loadStanderFaceData("standerface_fp_simplify");
}

//
SVFaceDataMesh::~SVFaceDataMesh(){
}

//
void SVFaceDataMesh::destory(){
    _clearFaceMeshPool();
    _clearStanderFacePool();
}

void SVFaceDataMesh::_loadFaceMeshData(cptr8 _name){
    SVDataChunk tDataStream;
    SVString filename = SVString("svres/mesh/") + SVString(_name) + SVString(".json");
    bool tflag = mApp->getFileMgr()->loadFileContentStr(&tDataStream, filename.c_str());
    if (!tflag) {
        SVString info = SVString("not find ") + filename + SVString(" please check mesh file path!\n");
       // MA_LOG_INFO(info.c_str());
        return;
    }
    SV_LOG_DEBUG("file facemeshdata %s \n", tDataStream.m_data);
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse<0>(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_DEBUG("rapidjson error code:%d \n", code);
        return;
    }
    FaceMesh *facemesh = new FaceMesh();
    if (doc.HasMember("param")) {
        RAPIDJSON_NAMESPACE::Value &t_param = doc["param"];
        if (t_param.IsObject()) {
            if (t_param.HasMember("tricount")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["tricount"];
                facemesh->tricount = t_value.GetInt();
            }
            if (t_param.HasMember("vertexcount")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["vertexcount"];
                facemesh->vertexcount = t_value.GetInt();
            }
        }
    }
    if (doc.HasMember("data")) {
        RAPIDJSON_NAMESPACE::Value &t_param = doc["data"];
        if (t_param.IsArray()) {
            for (int i=0; i<t_param.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &v = t_param[i];
                if (v.IsInt()) {
                    facemesh->triangles[i] = v.GetInt();
                }
            }
        }
    }
    m_facemeshpool.append(SVString(_name),facemesh);
    SV_LOG_DEBUG("load facemeshdata end\n");
}

void SVFaceDataMesh::_loadStanderFaceData(cptr8 _name){
    SVDataChunk tDataStream;
    SV_LOG_DEBUG("load standerfacedata begin\n");
    SVString filename = SVString("svres/mesh/") + SVString(_name) + SVString(".json");
    bool tflag = mApp->getFileMgr()->loadFileContentStr(&tDataStream, filename.c_str());
    if (!tflag) {
        SVString info = SVString("not find ") + filename + SVString(" please check shader file path!\n");
        //MA_LOG_INFO(info.c_str());
        return;
    }
    SV_LOG_DEBUG("file standerfacedata %s \n", tDataStream.m_data);
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse<0>(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_DEBUG("rapidjson error code:%d \n", code);
        return;
    }
    StanderFace *standerface = new StanderFace();
    if (doc.HasMember("param")) {
        RAPIDJSON_NAMESPACE::Value &t_param = doc["param"];
        if (t_param.IsObject()) {
            if (t_param.HasMember("facepoints_count")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["facepoints_count"];
                standerface->facepoints_count = t_value.GetInt();
            }
            if (t_param.HasMember("face_top")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["face_top"];
                standerface->face_top = t_value.GetInt();
            }
            if (t_param.HasMember("face_left")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["face_left"];
                standerface->face_left = t_value.GetInt();
            }
            if (t_param.HasMember("face_bottom")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["face_bottom"];
                standerface->face_bottom = t_value.GetInt();
            }
            if (t_param.HasMember("face_right")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["face_right"];
                standerface->face_right = t_value.GetInt();
            }
            if (t_param.HasMember("face_yaw")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["face_yaw"];
                standerface->face_yaw = t_value.GetFloat();
            }
            if (t_param.HasMember("face_roll")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["face_roll"];
                standerface->face_roll = t_value.GetFloat();
            }
            if (t_param.HasMember("face_pitch")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["face_pitch"];
                standerface->face_pitch = t_value.GetFloat();
            }
            if (t_param.HasMember("design_img_width")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["design_img_width"];
                standerface->design_img_width = t_value.GetInt();
            }
            if (t_param.HasMember("design_img_height")) {
                RAPIDJSON_NAMESPACE::Value &t_value = t_param["design_img_height"];
                standerface->design_img_height = t_value.GetInt();
            }
        }
    }
    if (doc.HasMember("data")) {
        RAPIDJSON_NAMESPACE::Value &t_param = doc["data"];
        if (t_param.IsArray()) {
            for (int i=0; i<t_param.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &v = t_param[i];
                if (v.IsFloat()) {
                    standerface->points[i] = v.GetFloat();
                }else if (v.IsInt()){
                    standerface->points[i] = v.GetInt()*1.0;
                }
            }
        }
    }
    m_standerfacepool.append(SVString(_name),standerface);
    SV_LOG_DEBUG("load standerfacedata end\n");
}

//
StanderFace* SVFaceDataMesh::getStanderFace(cptr8 _name){
    STANDERFACEPOOL::Iterator it = m_standerfacepool.find(_name);
    if(it!=m_standerfacepool.end()){
        return it->data;
    }
    return nullptr;
}

FaceMesh* SVFaceDataMesh::getFaceMesh(cptr8 _name){
    FACEMESHPOOL::Iterator it = m_facemeshpool.find(_name);
    if(it!=m_facemeshpool.end()){
        return it->data;
    }
    return nullptr;
}

//
void SVFaceDataMesh::_clearFaceMeshPool(){
    FACEMESHPOOL::Iterator it = m_facemeshpool.begin();
    while ( it!=m_facemeshpool.end() ) {
        FaceMesh *t_facemesh = it->data;
        if (t_facemesh) {
            delete t_facemesh;
        }
        it++;
    }
    m_facemeshpool.clear();
}

//
void SVFaceDataMesh::_clearStanderFacePool(){
    STANDERFACEPOOL::Iterator it = m_standerfacepool.begin();
    while ( it!=m_standerfacepool.end() ) {
        StanderFace *t_standerface = it->data;
        if (t_standerface) {
            delete t_standerface;
        }
        it++;
    }
    m_standerfacepool.clear();
}
