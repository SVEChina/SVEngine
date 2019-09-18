//
// SVPhysicsBodySoft.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICS_BODYSOFT_H
#define SV_PHYSICS_BODYSOFT_H


#include "SVPhysicsBody.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBody.h"
namespace sv {
    namespace logic {
        
        typedef SVArray<btSoftBody::Cluster*> SVSoftBodyClusterArray;
        typedef SVArray<btSoftBody::Note> SVSoftBodyNoteArray;//Note!!
        typedef SVArray<btSoftBody::Node> SVSoftBodyNodeArray;//Node!!
        typedef SVArray<btSoftBody::Link> SVSoftBodyLinkArray;
        typedef SVArray<btSoftBody::Face> SVSoftBodyFaceArray;
        typedef SVArray<btSoftBody::Tetra> SVSoftBodyTetraArray;
        typedef SVArray<btSoftBody::Anchor> SVSoftBodyAnchorArray;
        typedef SVArray<btSoftBody::RContact> SVSoftBodyRContactArray;
        typedef SVArray<btSoftBody::Material*> SVSoftBodyMaterialArray;
        typedef SVArray<btSoftBody::Joint*> SVSoftBodyJointArray;
        typedef SVArray<btSoftBody*> SVSoftBodyArray;
        
        class SVPhysicsBodySoft : public SVPhysicsBody {
        public:
            SVPhysicsBodySoft(SVInst *_app);

            ~SVPhysicsBodySoft();

            virtual void init();

            virtual void destroy();

            virtual void update(f32 _dt);
            
            btSoftBody* getBody();
            
            void setTotalMass(f32 _mass);
            
            void setMass(s32 _bulletnode, f32 _mass);
            
            void setTimeScale(f32 _timescale);
            
            void getNodes(SVSoftBodyNodeArray &_nodearray);
            
            s32 getNodesSize();
            
            void getLinks(SVSoftBodyLinkArray &_linkarray);
            
            s32 getLinksSize();
            
            void getFaces(SVSoftBodyFaceArray &_facearray);
            
            s32 getFacesSize();
            
            void appendAnchor(s32 _node,
                              SVPhysicsBodyRigidPtr _body, bool _disableCollisionBetweenLinkedBodies = false, f32 _influence = 1);
        protected:
            btSoftBody* m_softBody;
        };

    }//!namespace logic

}//!namespace sv
#endif //SV_PHYSICS_BODYSOFT_H
