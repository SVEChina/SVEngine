#include "SVGBase.h"
#include "../app/SVInst.h"

SVGBase::SVGBase(SVInst* _app)
:mApp(_app){
}

SVGBase::~SVGBase(){
    mApp = nullptr;
}
