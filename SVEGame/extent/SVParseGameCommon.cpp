//
// Created by yinlong on 17/10/20.
//

#include "SVParseGameCommon.h"
#include "../ui/SVUIParamFont.h"
#include "../ui/SVUIParamSpine.h"
#include "../ui/SVUIParamProgress.h"
#include "../ui/SVUIParamSprite.h"
#include "../ui/SVUIParamCombo.h"
#include "../ui/SVUIParamUI.h"
#include "../game/SVGameUIShow.h"
#include "../game/SVGameUIMgr.h"

SVUIParamBase* SVParseGameCommon::parseGameUi(SVInst* _app ,RAPIDJSON_NAMESPACE::Value& GUIItem, cptr8 _path ){
    SVUIParamBase* t_param_ui = NULL;
    if( GUIItem.HasMember("category") ){
        stringc t_category = GUIItem["category"].GetString();
        if(t_category == "SPINE"){
            SVUIParamSpine* t_param_spine = new SVUIParamSpine(_app);
            t_param_spine->m_WidgetName = t_category;
            if( GUIItem.HasMember("name") ){
                t_param_spine->m_secondName = GUIItem["name"].GetString();
            }
            if( GUIItem.HasMember("px") ){
                t_param_spine->m_px = GUIItem["px"].GetFloat();
            }
            if( GUIItem.HasMember("py") ){
                t_param_spine->m_py = GUIItem["py"].GetFloat();
            }
            if( GUIItem.HasMember("loop") ){
                t_param_spine->m_loop = GUIItem["loop"].GetInt() > 0?true:false;
            }
            if( GUIItem.HasMember("spine_name") ){
                t_param_spine->m_SpineName = stringc(_path) + GUIItem["spine_name"].GetString();
            }
            if( GUIItem.HasMember("spine_ani") ){
                t_param_spine->m_AniName = GUIItem["spine_ani"].GetString();
            }
            if (GUIItem.HasMember("spine_ani_loop")){
                t_param_spine->m_AniNameLoop = GUIItem["spine_ani_loop"].GetString();
            }
            if( GUIItem.HasMember("syncPosBoneName") && GUIItem["syncPosBoneName"].IsArray()){
                RAPIDJSON_NAMESPACE::Value &t_syncpos = GUIItem["syncPosBoneName"];
                for(int i=0;i<t_syncpos.Size();i++){
                    stringc t_posname = t_syncpos[i].GetString();
                    t_param_spine->m_exportBonePos.push_back(t_posname);
                }
            }
            if (GUIItem.HasMember("zorder") && GUIItem["zorder"].IsNumber()){
                t_param_spine->m_zorder = GUIItem["zorder"].GetInt();
            }
            t_param_ui = t_param_spine;
        }else if(t_category == "FONT"){
            //
            SVUIParamFont* t_param_font = new SVUIParamFont(_app);
            t_param_font->m_WidgetName = t_category;
            if( GUIItem.HasMember("name") ){
                t_param_font->m_secondName = GUIItem["name"].GetString();
            }
            if( GUIItem.HasMember("px") ){
                t_param_font->m_px = GUIItem["px"].GetFloat();
            }
            if( GUIItem.HasMember("py") ){
                t_param_font->m_py = GUIItem["py"].GetFloat();
            }
            if( GUIItem.HasMember("fontName") ){
                t_param_font->m_FontPngName = stringc(_path) + GUIItem["fontName"].GetString();
            }
            if( GUIItem.HasMember("fontWidth") ){
                t_param_font->m_FontWidth = GUIItem["fontWidth"].GetFloat();
            }
            if( GUIItem.HasMember("spineName") ){
                t_param_font->m_SyncSpineName = GUIItem["spineName"].GetString();
            }
            if( GUIItem.HasMember("boneName") ){
                t_param_font->m_SyncBoneName = GUIItem["boneName"].GetString();
            }
            if (GUIItem.HasMember("zorder") && GUIItem["zorder"].IsNumber()){
                t_param_font->m_zorder = GUIItem["zorder"].GetInt();
            }
            
            t_param_ui = t_param_font;
            
        }else if(t_category == "PROGRESS"){
            //进度条
            SVUIParamProgress* t_param_process = new SVUIParamProgress(_app);
            t_param_process->m_WidgetName = t_category;
            if( GUIItem.HasMember("name") ){
                t_param_process->m_secondName = GUIItem["name"].GetString();
            }
            if( GUIItem.HasMember("px") ){
                t_param_process->m_px = GUIItem["px"].GetFloat();
            }
            if( GUIItem.HasMember("py") ){
                t_param_process->m_py = GUIItem["py"].GetFloat();
            }
            if( GUIItem.HasMember("loop") ){
                t_param_process->m_loop = GUIItem["loop"].GetInt() > 0?true:false;
            }
            if( GUIItem.HasMember("spine_name") ){
                t_param_process->m_SpineName = stringc(_path) + GUIItem["spine_name"].GetString();
            }
            if( GUIItem.HasMember("spine_ani") ){
                t_param_process->m_AniName = GUIItem["spine_ani"].GetString();
            }
            if (GUIItem.HasMember("zorder") && GUIItem["zorder"].IsNumber()){
                t_param_process->m_zorder = GUIItem["zorder"].GetInt();
            }
            t_param_ui = t_param_process;
        }else if(t_category == "SPRITE"){
            //精灵
            SVUIParamSprite* t_param_sprite = new SVUIParamSprite(_app);
            t_param_sprite->m_WidgetName = t_category;
            if( GUIItem.HasMember("name") ){
                t_param_sprite->m_secondName = GUIItem["name"].GetString();
            }
            if( GUIItem.HasMember("px") ){
                t_param_sprite->m_px = GUIItem["px"].GetFloat();
            }
            if( GUIItem.HasMember("py") ){
                t_param_sprite->m_py = GUIItem["py"].GetFloat();
            }
            if( GUIItem.HasMember("width") ){
                t_param_sprite->m_width = GUIItem["width"].GetFloat();
            }
            if( GUIItem.HasMember("height") ){
                t_param_sprite->m_height = GUIItem["height"].GetFloat();
            }
            if( GUIItem.HasMember("texName") ){
                t_param_sprite->m_TextureName = stringc(_path) + GUIItem["texName"].GetString();
            }
            if (GUIItem.HasMember("zorder") && GUIItem["zorder"].IsNumber()){
                t_param_sprite->m_zorder = GUIItem["zorder"].GetInt();
            }
            t_param_ui = t_param_sprite;
        }else if(t_category == "COMBO_COM"){
            SVUIParamCombo* t_param_combo = new SVUIParamCombo(_app);
            t_param_combo->m_WidgetName = t_category;
            if( GUIItem.HasMember("name") ){
                t_param_combo->m_secondName = GUIItem["name"].GetString();
            }
            if( GUIItem.HasMember("px") ){
                t_param_combo->m_px = GUIItem["px"].GetFloat();
            }
            if( GUIItem.HasMember("py") ){
                t_param_combo->m_py = GUIItem["py"].GetFloat();
            }
            if( GUIItem.HasMember("spine_name") ){
                t_param_combo->m_SpineName = stringc(_path) + GUIItem["spine_name"].GetString();
            }
            if( GUIItem.HasMember("fontWidth") ){
                t_param_combo->m_FontWidth = GUIItem["fontWidth"].GetFloat();
            }
            if (GUIItem.HasMember("fontHeight") ){
                t_param_combo->m_FontHeight = GUIItem["fontHeight"].GetFloat();
            }
            if(GUIItem.HasMember("spine_ani") && GUIItem["spine_ani"].IsArray() ){
                RAPIDJSON_NAMESPACE::Value &aniSet = GUIItem["spine_ani"];
                for(int i = 0;i<aniSet.Size();i++){
                    stringc t_ani = aniSet[i].GetString();
                    t_param_combo->m_arrAniName.push_back(t_ani);
                }
            }
            t_param_ui = t_param_combo;
        }
    }
    return t_param_ui;
    
}

