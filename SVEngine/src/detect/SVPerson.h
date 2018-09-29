//
// SVPerson.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PERSON_H
#define SV_PERSON_H

#include "../base/SVListenBase.h"
#include "../base/SVDatatDef.h"
#include "../app/SVInst.h"
#include "SVPersonTracker.h"
#include "../base/SVLock.h"

//最大支持面部400个点
#define MAX_FACE_PT_NUM 200

namespace sv {
    
    namespace detect{
        
        class SVPerson : public SVListenBase {
        public:
            SVPerson(SVInst* _app);
            
            ~SVPerson();
            
            SVPersonTrackerPtr getTracker();
            
            void setDetectType(DETECTTYPE type);
            
            void update(f32 _dt);
            
            virtual void listenData(SVObjectPtr datagen);
            
            void setFaceData(void* _pdata,s32 _len);
            
            void setFaceDataOriginal(void* _pdata,s32 _len);
            
            void setFaceRect(f32 _left,f32 _top,f32 _right,f32 _bottom);
            
            SVRect getFaceRect();
            
            void setFaceRot(f32 _yaw,f32 _pitch,f32 _roll);
            
            FVec3 getFaceRot();
            
            f32 *getFaceData();
            
            f32 *getFaceDataOriginal();
            
            f32 getFaceDataX(s32 _index);
            
            f32 getFaceDataY(s32 index);
            
            s32 getFacePtNum();
            
            void setExist(bool _exist){ m_exist = _exist; }
            
            bool getExist(){ return m_exist; }
            
            void setPersonID(s32 _id);
            
            s32  getPersonID();
            
        protected:
            void _listenData_ST(SVDetectSTPtr detect);
            bool m_dirty;
            SVPersonTrackerPtr m_pTracker;
            DETECTTYPE m_detectType;    //识别数据类型
            bool m_exist;
            bool m_personDirty;
            s32 m_facePtNum;    //识别面部顶点数
            f32 *m_pFaceData;   //识别数据
            f32 *m_pFaceDataOriginal; 
            s32 m_personID;
            FVec3 m_facerot;
            SVRect m_facerect;
            SVLock m_dataLock;
        };
        
    }//!namespace detect
    
}//!namespace sv



//
#endif //SV_PERSON_H
