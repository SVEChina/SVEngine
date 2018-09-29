#include "SVObjLoader.h"
#include "../core/SVModel.h"

SVObjLoader::SVObjLoader(SVInst *_app)
:SVFileLoader(_app) {
}

SVModelPtr SVObjLoader::loadModel(cptr8 _fname) {
    SVModelPtr t_model = MakeSharedPtr<SVModel>();
    return t_model;
}
