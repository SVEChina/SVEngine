//
// Created by 徐子昱 on 2018/6/29.
//

#include "SVImagerReader.h"

SVImagerReader::SVImagerReader(int _w,int _h, enum AIMAGE_FORMATS format){
//    media_status_t status = AImageReader_new(_w, _h, format,
//                                             4, &m_pReader);
//
//    ASSERT(reader_ && status == AMEDIA_OK, "Failed to create AImageReader");
//
//    AImageReader_ImageListener listener{
//            .context = this, .onImageAvailable = OnImageCallback,
//    };
//    AImageReader_setImageListener(reader_, &listener);
}
SVImagerReader::~SVImagerReader(){

}
ANativeWindow* SVImagerReader::getNativeWindow(void){

}
void SVImagerReader::imageCallback(AImageReader *reader){

}
AImage *SVImagerReader::getNextImage(void){

}
AImage *SVImagerReader::GetLatestImage(void){

}
void SVImagerReader::deleteImage(AImage *image){

}
