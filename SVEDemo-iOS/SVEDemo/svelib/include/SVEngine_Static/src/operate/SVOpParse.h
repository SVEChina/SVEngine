//
// SVOpParse.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OPERATEPARSE_H
#define SV_OPERATEPARSE_H

#include "SVOpBase.h"
#include "../file/SVParseAnimate.h"
#include "../file/SVParse.h"
#include "../base/SVPreDeclare.h"
#include "../mtl/SVMtlDeclare.h"

//解析 加载资源文件

namespace sv {
    
    class SVOpParse : public SVOpBase {
    public:
        SVOpParse(SVInst *_app, s32 _resid,cptr8 _path);
        
        SVOpParse(SVInst *_app, s32 _resid,cptr8 _path, s32 _restype);
        
        
    protected:
        void _process(f32 dt);
        
        s32 resid;      //资源ID
        s32 restype;    //资源类型
        SVString path;
    };
    
    class SVOperateDelEft : public SVOpBase {
    public:
        SVOperateDelEft(SVInst *_app, s32 _resid);
        
    protected:
        void _process(f32 dt);
        
        s32 resid;
        SVString scenename;
    };
    
    //
    class SVOpSaveEft : public SVOpBase {
    public:
        SVOpSaveEft(SVInst *_app, s32 _resid, cptr8 _path);
        
    protected:
        void _process(f32 dt);
        
        int resid;
        SVString scenename;
        SVString path;
    };
    
    //
    class SVOpFilterEft : public SVOpBase {
    public:
        SVOpFilterEft(SVInst *_app, s32 _resType, s32 _resId);
        
    protected:
        void _process(f32 dt);
        
        s32 resId;      //资源ID
        s32 resType;    //资源类型
    };
    
    //生成节点
    class SVOpGenerate : public SVOpBase {
    public:
        SVOpGenerate(SVInst *_app, SVParseDataPtr _parsedataPtr, s32 _resid,cptr8 _path);
        
    protected:
        void _process(f32 dt);
        
        SVParseDataPtr mParseDataPtr;
        s32 mResid;
        SVString mPath;
    };
    
    //纹理加载
    class SVOpTexLoad : public SVOpBase {
    public:
        SVOpTexLoad(SVInst *_app, SVTexturePtr _tex,cptr8 _fname);
        
        ~SVOpTexLoad();
        
    protected:
        void _process(f32 dt);
        
        SVTexturePtr mTex;
        SVString mFilename;
    };
    
}//!namespace sv



#endif //SV_OPERATEPARSE_H
