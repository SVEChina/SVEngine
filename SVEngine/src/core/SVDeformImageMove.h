//
// SVDeformImageMove.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DEFORMIMAGEMOVE_H
#define SV_DEFORMIMAGEMOVE_H

#include "../base/SVGBase.h"
#include "../base/SVMap.h"
#include "../core/SVImageUsingMove.h"
#include "../mtl/SVTexture.h"
#include "../base/SVArray.h"

namespace sv {
    
    namespace util{
        
        class SVParamDeform{
        public:
            SVParamDeform();
            ~SVParamDeform();
            void reset(){
                m_pointMap.clear();
                m_areaPoint.clear();
            }
            void copy(SVParamDeformPtr _param){
                m_pointMap=_param->m_pointMap;
                m_areaPoint=_param->m_areaPoint;
            }
            
            void addTagPoint(u32 _postion,V2 _point){
                m_pointMap.append(_postion, _point);
            }
            SVMap<u32, V2> m_pointMap;
            SVMap<u32,V2> m_areaPoint;
            
        };
        
        //deform imagemove 算法
        class SVDeformImageMove : public SVGBase {
        public:
            SVDeformImageMove(SVInst *_app);
            
            ~SVDeformImageMove();
            
            void reset();
            
            void init(SVTexturePtr _tex,SVTexturePtr _texout);

            void update(f32 dt);
            
            void render();
            
            void setParam(SVParamDeformPtr _param){
                m_param->copy(_param);
            }
            
            void setTagPoint(u32 _postion,V2 _point);
            
            void setTagAreaPoint(u32 _postion,V2 _point);
            
            void pointMove(V2 *_data);
            
            void setPoint(V2 *_data);
            
            inline void setFlip(bool _flip){
                m_flip=_flip;
            }
            
            inline void setIsDetect(bool _isdetect){
                is_detect=_isdetect;
            }
            
            s32 getWidth();
            
            s32 getHeight();
            
            V2 MSL(V2 point);
            
            void updatePointMSL();
            
            void setDeformSwitch(bool _swith);
            
            void updatePointMesh( V2* _facepoint);
            
            void setvisiPoint(bool _ispoint){
                m_is_point = _ispoint;
            }
            
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        
        public:
            s32 m_rule;
        
        protected:
            
            void _updateControl(V2 *t_data);
            
            void _updateMesh();
            
            void _refreshScreenRectMesh(V2 *t_data,V2 *t_targetData);
            
            void _initPoint();
            
            SVTexturePtr m_pTexBg;
            SVTexturePtr m_pPointTex;
            SVMtlCorePtr m_pMtlBg;
            SVRenderMeshPtr m_pMeshBg;
            SVRenderMeshPtr m_pMeshPoint;
            SVRenderTexturePtr m_fbo;
            SVImageUsingMovePtr m_pIUMP;
            SVPassPtr m_passDeform;
            SVPassPtr m_passPoint;
            SVPassPtr m_passBack;
            V2 m_pointScreen[51*64];     //31*81
            SVParamDeformPtr m_param;

            V2 *m_dataPoint;
            s32 m_wPointCount;
            s32 m_hPointCont;
            
            bool is_swith;
            bool m_flip;
            bool m_is_point;
            bool is_detect;
    
            s32 m_tt_w;
            s32 m_tt_h;
            s32 m_inw;
            s32 m_inh;
       
        };

    }//!namespace util
    
}//!namespace sv

#endif /* SV_DEFORMIMAGEMOVE_H */
