//
// Created by 徐子昱 on 2018/6/28.
//

#ifndef SV_ENGINESYSTEM_H
#define SV_ENGINESYSTEM_H


#include "app/SVInst.h"
#include "filter/SVFilterExtDef.h"


using namespace sv;

class SVEngineSystem {
public:
    SVEngineSystem();
    ~SVEngineSystem();
    void initEngine();
    void setWindows(ANativeWindow *_windows);
    void addResPath(SVString _path);
    void startEngine();
    void stopEngine();
    void suspend();
    void resume();
    void createRenderGL(int glVer,EGLContext _context,int _w,int _h);
    void destoryRenderGL();
    void createRenderTarget(int _fboid,int _colorid,int _w,int _h);
    void createRenderTextureTarget(int _texId,int _w,int _h);
    void updateFilter(int type,int smooth);
    void destoryRenderTarget();
    void createScene(cb_func_op _cb,char* _msg);
    void destoryScene();
    void createInStream(SVString _name,int _type,int _format,int _w,int _h,int _angle);
    void pushStreamData(SVString _name,char* _srcPtr,int _w,int _h,int _pixelFormat,int _angle);
    void createOutTexture(SVString _name,int _format,int _type,int _w,int _h,int _angle);
    void createWatermark(void* rgba,int  width, int height);
    void openFaceBeauty(int lows);
    void touchPos(float _x,float _y);
    void destoryInStream(SVString _name);
    void destoryOutstream(SVString _name);

    int getTexId();
private:
    SVInst  *m_pApp;
    ANativeWindow *m_pWindows;
    SVWatermarkPtr m_watermark;
    SVString m_path;
};


#endif //SV_ENGINESYSTEM_H
