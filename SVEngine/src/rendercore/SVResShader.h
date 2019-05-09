//
// SVResShader.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RESSHADER_H
#define SV_RESSHADER_H

#include "SVRObjBase.h"
#include "../base/SVPreDeclare.h"

/*
  GL DEF 暂时先放这里
 */

//通道数
#define CHANNEL_POSITION    0
#define CHANNEL_NORMAL      1
#define CHANNEL_TAGENT      2
#define CHANNEL_BTAGENT     3
#define CHANNEL_COLOR0      4
#define CHANNEL_COLOR1      5
#define CHANNEL_TEXCOORD0   6
#define CHANNEL_TEXCOORD1   7
#define CHANNEL_TEXCOORD2   2
#define CHANNEL_TEXCOORD3   3
#define CHANNEL_BONE_ID     3
#define CHANNEL_BONE_WEIGHT 4
#define CHANNEL_INSOFFSET   8
//
#define CHANNEL_ATTRI_0     0
#define CHANNEL_ATTRI_1     1
#define CHANNEL_ATTRI_2     2
#define CHANNEL_ATTRI_3     3
#define CHANNEL_ATTRI_4     4
#define CHANNEL_ATTRI_5     5
#define CHANNEL_ATTRI_6     6
#define CHANNEL_ATTRI_7     7

//内置变量
#define NAME_POSITION   "aPosition"
#define NAME_NORMAL     "aNormal"
#define NAME_TAGENT     "aTagent"
#define NAME_COLOR      "aColor"
#define NAME_COLOR1     "aColor1"
#define NAME_TEXCOORD0  "aTexcoord0"
#define NAME_TEXCOORD1  "aTexcoord1"
#define NAME_TEXCOORD2  "aTexcoord2"
#define NAME_TEXCOORD3  "aTexcoord3"
#define NAME_INSOFFSET  "aInsOffset"
//骨骼动画
#define NAME_BONE_ID     "aBoneID"
#define NAME_BONE_WEIGHT "aBoneWeight"

//
#define NAME_TEX0  "aTexture0"
#define NAME_TEX1  "aTexture1"
#define NAME_TEX2  "aTexture2"
#define NAME_TEX3  "aTexture3"
#define NAME_TEX4  "aTexture4"
#define NAME_TEX5  "aTexture5"
#define NAME_TEX6  "aTexture6"
#define NAME_TEX7  "aTexture7"

//uniform
#define NAME_M_MATRIX   "aMatrixM"
#define NAME_V_MATRIX   "aMatrixV"
#define NAME_P_MATRIX   "aMatrixP"
#define NAME_VP_MATRIX  "aMatrixVP"
#define NAME_MVP_MATRIX "aMatrixMVP"

#define BLEND_FARC      "blendfrac"
#define TEXCOORD_CLIP   "texcoordClip"
#define NAME_TEXSIZE_0  "tex0size"
#define NAME_TEXSIZE_1  "tex1size"
#define NAME_TEXSIZE_2  "tex2size"
#define NAME_TEXSIZE_3  "tex3size"

#define NAME_BLENDMODE  "blendmode"
#define NAME_POINTSIZE  "ptsize"
//渲染资源

namespace sv {
    
    namespace render{
        
        class SVResShader: public SVRObjBase{
        public:
            SVResShader(SVInst* _app);
            
            virtual ~SVResShader();
            
            virtual void create(SVRendererBasePtr _renderer){}
            
            virtual void destroy(SVRendererBasePtr _renderer){}
            
            virtual bool active(SVRendererBasePtr _render){ return false; }
            
            void setProgrammeName(cptr8 _filename);
            
            cptr8 getProgrammeName();
            
            void setTechFName(cptr8 _filename);
            
            void setVSFName(cptr8 _filename);
            
            void setFSFName(cptr8 _filename);
            
            void setGSFName(cptr8 _filename);
            
            void setCSFName(cptr8 _filename);
            
            void setTSCFName(cptr8 _filename);
            
            void setTSEFName(cptr8 _filename);

        protected:
            SVString m_programme_fname;
            
            SVString m_vs_fname;
            
            SVString m_fs_fname;
            
            SVString m_gs_fname;
            
            SVString m_cs_fname;
            
            SVString m_tsc_fname;
            
            SVString m_tse_fname;
        };
        
    }//!namespace render
    
}//!namespace sv


#endif /* SV_RESSHADER_H */
