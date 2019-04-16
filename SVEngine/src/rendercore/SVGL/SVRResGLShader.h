//
// SVRResGLShader.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RRESGL_SHADER_H
#define SV_RRESGL_SHADER_H

#include "../SVRObjBase.h"
#include "../SVResTex.h"
#include "../SVResFBO.h"
#include "../SVResShader.h"
#include "../../base/SVPreDeclare.h"
#include "../../base/SVRect.h"
#include "../../core/SVVertDef.h"
#include "../../base/SVDataChunk.h"

namespace sv {

    namespace render{

        
        /*
         GL Shader
         */
        
        class SVRResGLShader: public SVResShader {
        public:
            SVRResGLShader(SVInst* _app);

            virtual ~SVRResGLShader();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

            void setTechFName(cptr8 _filename);

        private:
            bool _parseTech();

            u32 _loadTechVS(cptr8 _precision,cptr8 _src);

            u32 _loadTechFS(cptr8 _precision,cptr8 _src);

            u32 _loadTechGS(cptr8 _precision,cptr8 _src);

            u32 _loadVS(cptr8 _filename);

            u32 _loadFS(cptr8 _filename);

            u32 _loadGS(cptr8 _filename);

            u32 _loadCS(cptr8 _filename);

            u32 _loadTSC(cptr8 _filename);

            u32 _loadTSE(cptr8 _filename);

            u32 _createProgram();

            void _clearShaderRes();

            bool m_use_tech;

            SVString m_tech_fname;   //技术

            u32 m_vs;

            u32 m_fs;

            u32 m_gs;

            u32 m_tsc;

            u32 m_tse;

            u32 m_cs;
        };
        

    }//!namespace render

}//!namespace sv



//
#endif //SV_RRESGL_SHADER_H
