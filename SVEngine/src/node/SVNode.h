//
// SVNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_NODE_H
#define SV_NODE_H

#include "SVNodeDef.h"
#include "../mtl/SVMtlDeclare.h"
#include "../rendercore/SVRenderDef.h"
#include "../core/SVVertDef.h"
#include "../base/SVMat4d.h"
#include "../base/SVVec3.h"
#include "../base/SVQuat.h"
#include "../base/SVBounds.h"
#include "../base/SVPreDeclare.h"
#include "../event/SVEventProc.h"
#include "../detect/SVPerson.h"
#include "../base/SVDataSwap.h"

namespace sv {
    
    namespace node{
        
        //
        class SVNode : public SVEventProc {
            friend class SVActBase;
        public:
            SVNode(SVInst *_app);
            
            ~SVNode();
            
            bool operator <(const SVNode& rhs) const{
                return m_iZOrder < rhs.m_iZOrder;
            }
            bool operator >(const SVNode& rhs) const{
                return m_iZOrder > rhs.m_iZOrder;
            }

            virtual void enter();
            
            virtual void exit();
            
            void deep_update(f32 dt);
            
            //深度访问
            void deep_visit(SVVisitorBasePtr _visit);
            
            //选择访问 用于子访问
            void select_visit(SVVisitorBasePtr _visit);
            
            FMat4& getLocalMat();
            
            FMat4& getAbsoluteMat();
            
            FMat4& getIAbsoluteMat();
            
            virtual void setPosition(f32 x, f32 y, f32 z);
            
            virtual void setOffset(f32 _x, f32 _y, f32 _z);
            
            virtual void setRotation(f32 x, f32 y, f32 z);
            
            virtual void setQuat(SVQuat& _quat);
            
            virtual void setScale(f32 x, f32 y, f32 z);
            
            virtual void setPosition(FVec3& _pos);
            
            virtual void setOffset(FVec3& _pos);
            
            virtual void setRotation(FVec3& _rot);
            
            virtual void setScale(FVec3& _scale);
            
            virtual void setAlpha(f32 _alpha);
            
            virtual void setBindOffset(FVec3& _offset);
            
            virtual void setBindOffset(f32 _offsetX, f32 _offsetY, f32 _offsetZ);
            
            void setAutoAdaptDesign(bool _adapt);
            
            void setbeSelect(bool _select);
            
            void setcanSelect(bool _select);
            
            void setBindIndex(s32 _index);
            
            s32  getBindIndex();
            
            FVec3& getBindOffset();
            
            void enableMipMap();
            
            void disableMipMap();
            
            void setRSType(RENDERSTREAMTYPE _retype);
            
            RENDERSTREAMTYPE getRSType();
            
            void setdrawAABB(bool _drawaabb);
            
            bool getDrawAABB();
            
            SVNodePtr getParent();
            
            FVec3& getPosition();
            
            FVec3& getOffset();
            
            FVec3& getRotation();
            
            FVec3& getScale();

            SVQuat getQuat();
            
            void setAABB(SVBoundBox& _aabb);
            
            SVBoundBox& getAABB();
            
            SVBoundBox& getAABBSW();
            //设置z顺序
            void setZOrder(s32 _zorder);
            
            void setLocalMatrix(FMat4 &_mat);
            
            inline cptr8 getType(){ return ntype.c_str(); }
            inline s32 getuid(){ return uid; }
            inline cptr8 getname(){return m_name.c_str();}
            inline s32 getZOrder(){ return m_iZOrder;}
            inline bool getcanSelect(){ return m_canSelect; }
            inline bool getbeSelect(){ return m_beSelect; }
            inline bool getcanProcEvent(){ return m_canProcEvent; }
            inline bool getvisible(){ return m_visible; }
            inline bool getIsAutoAdaptDesign(){ return m_adaptDesign; }
            inline void setname(const char* _name){ m_name = _name; }
            inline void setvisible(bool _visible){ m_visible = _visible; }
            inline void setPeronID(s32 _personID){ m_personID = _personID;}
            inline s32  getPersonID(){ return m_personID; }
            inline bool getIsMipMap(){ return m_enableMipMap; }
            //设置材质
            void setMtl(SVMtlCorePtr _mtl);
            
            //获取材质
            SVMtlCorePtr getMtl();
            
        protected:
            //更新
            virtual void update(f32 dt);
            //剔除
            virtual bool _clip();
            //渲染
            virtual void render();
            //
            SVString ntype;         //节点类型
            SVString m_name;        //唯一名称
            s32 uid;                //唯一ID
            s32 m_personID;         //角色ID
            s32 m_iZOrder;          //Z值
            bool m_canSelect;       //是否可以选择
            bool m_canProcEvent;    //是否能处理事件
            bool m_visible;         //是否可见
            bool m_drawBox;         //是否渲染包围盒
            RENDERSTREAMTYPE m_rsType;      //渲染流类型
            FVec3 m_postion;        //位置
            FVec3 m_rotation;       //旋转
            FVec3 m_scale;          //缩放
            FVec3 m_offpos;         //偏移
            s32 m_bindIndex;        //绑定索引
            f32 m_alpha;
            FVec3 m_bindOffset;
            bool m_enableMipMap;     //是否开启mipmap，//先写到这里了。
            //
            bool m_dirty;
            bool m_beSelect;        //是否被选择
            bool m_adaptDesign;
            SVBoundBox m_aabbBox;   //AABB包围盒
            SVBoundBox m_aabbBox_sw;//AABB世界包围盒
            FMat4 m_localMat;       //本地矩阵
            FMat4 m_absolutMat;     //绝对世界矩阵
            FMat4 m_iabsolutMat;    //逆绝对世界矩阵
            SVMtlCorePtr m_pMtl;    //材质

        public:
            //节点模块
            void addChild(SVNodePtr node);
            
            void addChild(SVNodePtr node, s32 iZOrder);
            
            bool removeChild(SVNodePtr node);
            
            bool removeFromParent();
            
            bool hasChild(SVNodePtr node);
            
            void clearChild();
            
            SVNodePtr getNode(cptr8 _name);
            
            u32 getChildNum();
            
            SVNodePtr getChild(u32 _index);
            
        protected:
            //父节点
            SVNodePtr m_parent;
             //子节点池
            typedef SVArray<SVNodePtr> NODEPOOL;
            NODEPOOL m_childNodePool;
            //排序
            void _sort_child();
            bool m_needsort;
            
            //序列化接口
        public:
            SVString m_rootPath;
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
        protected:
            virtual void _toJsonData(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                             RAPIDJSON_NAMESPACE::Value &locationObj);
            
            virtual void _fromJsonData(RAPIDJSON_NAMESPACE::Value &item);

        };
        
    }//!namespace node
    
}//!namespace sv



#endif //SV_NODE_H
