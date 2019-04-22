//
// SVBackGroundNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BACKGROUNDNODE_H
#define SV_BACKGROUNDNODE_H

#include "SVSpriteNode.h"
#include "../base/SVMap.h"


namespace sv {
    
    namespace node {
        
        /*
         .三维空间绘制
         .屏幕空间绘制
         .形变接口
         .内置纹理显示
         .外设纹理显示
         */
        
        class SVBackGroundNode : public SVNode {
        public:
            SVBackGroundNode(SVInst *_app);
            
            SVBackGroundNode(SVInst *_app,f32 _w,f32 _h);
            
            virtual ~SVBackGroundNode();
            
            void update(f32 dt);
            
            void render();
            
            void setSize(f32 _w,f32 _h);
            
            void syncTexSize();
            
            inline f32 getWidth(){ return m_width; }
            
            inline f32 getHeight(){ return m_height; }
            
            //设置纹理
            void setTexture(cptr8 _path);
            
            void setTexture(SVTEXTYPE _textype);
            
            void setTexture(SVTexturePtr _tex);
            
            SVTEXTYPE getUseInnerTex(){ return m_useTexType; }
            
            //形变相关接口
            bool enableDeform();
            
            void disableDeform();
            
            bool isDeform();
            
            SVDeformImageMovePtr getDeform();
            
            //是否在屏幕空间渲染
            void setInScreen(bool _inscreen);
            
            bool isInScreen(){ return m_inScreen; }
            
        protected:
            SVString m_pTexName;        //纹理名称
            SVTEXTYPE m_useTexType;     //内置纹理类型
            SVDeformImageMovePtr m_pDeform; //形变算法
            bool m_inScreen;        //是否在屏幕空间
            bool m_isSyncTex;
            f32 m_width;
            f32 m_height;
            f32 m_texcoordX;
            f32 m_texcoordY;
            SVTexturePtr m_pTex;
            SVRenderMeshPtr m_pMesh;
            SVRenderObjectPtr m_pRenderObj;
            
        public:
            //序列化接口
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };
        
    }//!namespace node
    
}//!namespace sv

#endif /* SV_BACKGROUNDNODE_H */
