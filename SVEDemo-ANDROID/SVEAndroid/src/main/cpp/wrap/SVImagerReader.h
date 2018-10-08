//
// Created by 徐子昱 on 2018/6/29.
//

#ifndef SV_IMAGERREADER_H
#define SV_IMAGERREADER_H

#include "media/NdkImageReader.h"


class SVImagerReader {
public:
    SVImagerReader(int _w,int _h, enum AIMAGE_FORMATS format);
    ~SVImagerReader();
    ANativeWindow* getNativeWindow(void);
    void imageCallback(AImageReader *reader);
    AImage *getNextImage(void);
    AImage *GetLatestImage(void);
    void deleteImage(AImage *image);
    AImageReader* m_pReader;
};


#endif //ANDROID_SVIMAGERREADER_H
