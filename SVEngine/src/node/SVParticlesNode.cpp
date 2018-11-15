#include "SVParticlesNode.h"
#include "SVParticlesWorld.h"
#include "SVCameraNode.h"
#include "../base/SVDataSwap.h"
#include "../core/SVParticles.h"
#include "../basesys/SVCameraMgr.h"
#include "../mtl/SVMtlNocolor.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderObject.h"

#define OBJECT_PARTICLES_EPSILON	1e-3f

//********************************* SVParticlesNode ********************************************
SVParticlesNode::SVParticlesNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVParticlesNode";
    m_drawBox = true;
    m_canSelect = true;
    m_rsType = RST_ANIMATE;
    m_pParticles = MakeSharedPtr<SVParticles>();
    m_pParticlesWorld = MakeSharedPtr<SVParticlesWorld>();
    m_pParticles->setSeed(mApp->m_pGlobalParam->getRandom());
    m_pParticles->setWorld(m_pParticlesWorld);
    physical_mask = 1;
    emitter_sync = 1;
    emitter_velocity = Vec3_zero;
    world_offset = Vec3_zero;
    num_triangles = 0;
    bound_box = m_pParticles->getSVBoundBox();
    //
    m_mtl_particle = MakeSharedPtr<SVMtlParticleAni>(mApp);
    m_pMesh = _app->getRenderMgr()->createMeshRObj();
    m_pMesh->createMesh(true);
    m_pMesh->setVertexType(E_VF_V3_PARTICLE);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
    //
    m_pVertData = MakeSharedPtr<SVDataSwap>();
    m_pIndexData = MakeSharedPtr<SVDataSwap>();
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_atten = mApp->getTexMgr()->getTexture("svres/textures/particles_base_attenuation.png",true);
    m_diffuse = mApp->getTexMgr()->getTexture("svres/textures/particles_base_diffuse.png",true);
}

SVParticlesNode::~SVParticlesNode() {
    m_pParticlesWorld = nullptr;
    m_pParticles = nullptr;
    m_pRenderObj = nullptr;
    m_pIndexData = nullptr;
    m_pVertData = nullptr;
    m_pMesh = nullptr;
    m_mtl_particle = nullptr;
}

//// 粒子类型
//enum PARTICLETYPE {
//    TYPE_BILLBOARD = 0,
//    TYPE_FLAT,
//    TYPE_POINT,
//    TYPE_LENGTH,
//    TYPE_RANDOM,
//    TYPE_ROUTE,
//    TYPE_CHAIN,
//    NUM_TYPES,
//};

//// 粒子发射器类型
//enum EMITTERTYPE {
//    EMITTER_POINT = 0,
//    EMITTER_SPHERE,
//    EMITTER_CYLINDER,
//    EMITTER_BOX,
//    EMITTER_RANDOM,
//    EMITTER_SPARK,
//    NUM_EMITTERS,
//};

void SVParticlesNode::testInit() {
    if( m_pParticles) {
        //设置粒子类型
        m_pParticles->setType(2);
        //开启发射器
        m_pParticles->setEmitterEnabled(1);
        //发射器类型
        m_pParticles->setEmitterType(0);
        //发射器尺寸
        m_pParticles->setEmitterSize(FVec3(100.0f,100.0f,100.0f));
        //发射器连续
        m_pParticles->setEmitterContinuous(1);
        //发射器速度
        setEmitterVelocity(FVec3(0.0f,0.0f,0.0f));
        //
        m_pParticles->setEmitterSpread(FVec3(0.0f,0.0f,0.0f));
        //发射器方向
        m_pParticles->setEmitterDirection(FVec3(0.0f,1.0f,0.0f));
        //设置增长
        m_pParticles->setGrowth(10.0f, 0.0f);
        //重力速度
        m_pParticles->setGravity(FVec3(0.0f,0.0f,0.0f));
        //
        m_pParticles->setVelocity(100.0,0.0);
        //
        m_pParticles->setRadius(5.0f,0.0f);
        //
        m_pParticles->setLife(6.0f,0.0f);
    }
}

SVParticlesPtr SVParticlesNode::getParticles() {
    return m_pParticles;
}

//    EMITTER_POINT = 0,
//    EMITTER_SPHERE,
//    EMITTER_CYLINDER,
//    EMITTER_BOX,
//    EMITTER_RANDOM,
//    EMITTER_SPARK,

void SVParticlesNode::update(f32 ifps) {
    SVNode::update(ifps);
    //更新矩阵
    update_transform();
    //粒子系统更新
    m_pParticles->update(ifps);
    //强制设置一个基础包围盒
    m_aabbBox.set(FVec3(-10.0f,-10.0f,-10.0f), FVec3(10.0f,10.0f,10.0f));
//    //contact points
//    if(m_pParticles->getNumContacts()) {
//        // childs interaction
//        s32 num_childs = getChildNum();
//        for(s32 i = 0; i < num_childs; i++) {
//            SVNodePtr t_node = getChild(i);
//            //            if(node->isEnabledSelf() == 0) {
//            //                continue;
//            //            }
//            // generate SVParticles
//            SVString type = t_node->getType();
//            if(type == "SVParticlesNode") {
//                //粒子上挂粒子
//                SVParticlesNodePtr p = std::static_pointer_cast<SVParticlesNode>(t_node);
//                if(p->getEmitterType() == SVParticles::EMITTER_SPARK) {
//                    //火舌
//                    for(s32 j = 0; j < m_pParticles->getNumContacts(); j++) {
//                        FVec3 point = FVec3(m_pParticles->getContactPoint(j)) + world_offset;
//                        const FVec3 &normal = m_pParticles->getContactNormal(j);
//                        const FVec3 &velocity = m_pParticles->getContactVelocity(j);
//                        p->addEmitterSpark(point,normal,velocity);
//                    }
//                }
//            } else if(type == "SVDecalNode") {
//                //粒子上挂贴花
//                //                // generate decals
//                //                FMat4 transform = Mat4_identity;
//                //                DecalObjectOrtho *decal = static_cast<DecalObjectOrtho*>(node);
//                //                for(s32 j = 0; j < m_pParticles->getNumContacts(); j++) {
//                //                    FVec3 point = FVec3(m_pParticles->getContactPoint(j)) + world_offset;
//                //                    FVec3 direction = normalize(m_pParticles->getContactVelocity(j));
//                //                    orthoTransform(FVec3(-direction),transform);
//                //                    transform.setColumn3(3,point - FVec3(direction * (decal->getRadius() * 0.5f)));
//                //                    decal->setWorldTransform(transform * FMat4(rotateZ(engine.game->getRandomFloat(0.0f,360.0f))));
//                //                    decal->create();
//                //                    decal->release();
//                //                }
//            }
//        }
//    }
//
//    //子求交
//    s32 num_particles = 0;
//    s32 num_childs = getChildNum();
//    for(s32 i = 0; i < num_childs; i++) {
//        SVNodePtr t_node = getChild(i);
//        //        if(node->isEnabledSelf() == 0) {
//        //            continue;
//        //        }
//        // shift emitters
//        SVString type = t_node->getType();
//        if(type == "SVParticlesNode") {
//            SVParticlesNodePtr p = std::static_pointer_cast<SVParticlesNode>(t_node);
//            if(p->isEmitterShift() && p->getEmitterSync()) {
//                if(num_particles < m_pParticles->getNumParticles()) {
//                    FMat4 transform = Mat4_identity;
//                    FVec3 delta = FVec3(m_pParticles->getParticleOldPosition(num_particles)) + world_offset - p->getAbsoluteMat().getColumn3(3);
//                    s32 continuous = (Math::abs(delta.x) < OBJECT_PARTICLES_EPSILON && Math::abs(delta.y) < OBJECT_PARTICLES_EPSILON && Math::abs(delta.z) < OBJECT_PARTICLES_EPSILON);
//                    orthoTransform(FVec3(normalize(m_pParticles->getParticleVelocity(num_particles))),transform);
//                    transform.setColumn3(3,FVec3(m_pParticles->getParticlePosition(num_particles)) + world_offset);
//                    if(continuous == 0) {
//                        p->setEmitterEnabled(0);
//                    }
//                    //p->setWorldTransform(transform);
//                    if(continuous != 0) {
//                        p->setEmitterEnabled(1);
//                    }
//                } else {
//                    p->setEmitterEnabled(0);
//                }
//                num_particles++;
//            }
//        } else if(type == "SVMeshClusterNode") {
//            //            // mesh cluster
//            //            SVArray<FMat4> transforms;
//            //            getParticleTransforms(transforms);
//            //            ObjectMeshCluster *cluster = static_cast<ObjectMeshCluster*>(node);
//            //            cluster->createMeshes(transforms);
//        }
//    }
//    //    // update bounds
//    //    FMat4 transform = FMat4(getIWorldTransform() * translate(world_offset));
//    //    bound_box = transform * SVParticles->getSVBoundBox();
}

void SVParticlesNode::render() {
    if (!m_visible)
        return;
    SVCameraNodePtr t_camera = mApp->getCameraMgr()->getMainCamera();
    if(!t_camera)
        return ;
    const FVec3 &camera_position = t_camera->getPosition();
    const FMat4 &modelview = t_camera->getViewMatObj();// m_absolutMat*t_camera->getViewMatObj(); 粒子发射出来就是world 所以就不需要乘节点的矩阵了
    // level of detail
    f32 distance = (camera_position - m_postion).length();
    f32 t_max_dis = 1000.0f;
    f32 t_min_dis = 10.0f;
    f32 fade = 0.5f; //根据视点到目标的距离来计算 fade值
    if(fade < EPSILON) {
        return ;
    }
    // SVParticles transformation
    m_mtl_particle->m_p_transform = modelview * translate(world_offset);
    // SVParticles radius
    m_mtl_particle->m_p_radius = m_pParticles->getRadiusMean();
    // SVParticles fade
    m_mtl_particle->m_p_fade = fade * fade;
    //设置纹理
    m_mtl_particle->setTexture(0,m_diffuse);
    m_mtl_particle->setTexture(1,m_atten);
    //设置环境光
    m_mtl_particle->m_ambient_color = FVec4(1.0f,1.0f,1.0f,1.0f);
    //设置漫反色
    m_mtl_particle->m_diffuse_color = FVec4(1.0f,0.0f,0.0f,1.0f);
    //设置融合
    m_mtl_particle->setBlendEnable(true);
    m_mtl_particle->setBlendState(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    //生产粒子数据
    if( m_pParticles && m_pMesh) {
        m_pParticles->render(modelview, camera_position);
        //计算-更新索引数据
        s32 t_p_num = m_pParticles->getParticleNum();
        if(t_p_num == 0)
            return ;
        s32 t_index_num = t_p_num*6;
        s32 t_index_size = t_index_num*sizeof(u16);
        s32 t_ex_size = t_index_size * 2;
        m_pIndexData->extendSize(t_ex_size);
        m_pIndexData->lockData();
        u16 *t_indices  = (u16 *)(m_pIndexData->getData());
        for(s32 i=0;i<t_p_num;i++) {
            t_indices[0] = i*4 + 0;
            t_indices[1] = i*4 + 1;
            t_indices[2] = i*4 + 2;
            t_indices[3] = i*4 + 2;
            t_indices[4] = i*4 + 3;
            t_indices[5] = i*4 + 0;
            t_indices += 6;
        }
        m_pIndexData->unlockData(t_index_size);
        m_pMesh->setIndexData(m_pIndexData,t_index_num);
        //更新顶点数据
        m_pVertData->writeData(m_pParticles->pVertex,
                               m_pParticles->m_vertexBufNum*sizeof(V3_PARTICLE));
        m_pMesh->setVertexDataNum(m_pParticles->m_vertexBufNum);
        m_pMesh->setVertexData(m_pVertData);
    }
    //
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (m_pRenderObj) {
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(m_mtl_particle);
        m_pRenderObj->pushCmd(t_rs, m_rsType, "SVParticlesNode");
    }
    //渲染虚拟的东西
    renderVisualizer();
    //
    SVNode::render();
}

//************************************ Transformation **************************************
const FVec3 &SVParticlesNode::getWorldOffset() const {
    return world_offset;
}

//这个函数是用来更新发射器和粒子的变换矩阵
void SVParticlesNode::update_transform() {
    // 发射器速度
    if(emitter_velocity != Vec3_zero) {
        m_pParticles->setEmitterVelocity(emitter_velocity);
    } else {
        //getworldVelocity 如果粒子本身没有速度方向，就设置为这个世界的速度方向(这个世界是物理属性的)
        emitter_velocity = FVec3(0.0f,1.0f,0.0f);
        m_pParticles->setEmitterVelocity(emitter_velocity);
    }
    // 发射器偏移
    if(m_pParticles->getNumParticles() == 0) {
        FVec3 old_world_offset = world_offset;
        if(m_pParticles->isEmitterShift() == 0){
            world_offset = getAbsoluteMat().getColumn3(3);
        } else if(m_pParticles->isEmitterEnabled() == 0) {
            world_offset = getAbsoluteMat().getColumn3(3);
        }
        //粒子本身的offset与矩阵中offset不一致的处理逻辑
        if(world_offset != old_world_offset) {
            FMat4 transform = FMat4(translate(old_world_offset - world_offset));
            for(s32 i = 0; i < m_pParticles->getNumForces(); i++) {
                if(m_pParticles->isForceAttached(i)) {
                    continue;
                }
                //给粒子设置force矩阵
                m_pParticles->setForceTransform(i,transform * m_pParticles->getForceTransform(i));
            }
            //
            for(s32 i = 0; i < m_pParticles->getNumDeflectors(); i++) {
                if(m_pParticles->isDeflectorAttached(i)) {
                    continue;
                }
                //偏导，变流矩阵
                m_pParticles->setDeflectorTransform(i,transform * m_pParticles->getDeflectorTransform(i));
            }
        }
    }
    //设置发射器矩阵 粒子的位置 实际上就是发射器的位置
    FMat4 transform = getAbsoluteMat();
    transform.m03 -= world_offset.x;
    transform.m13 -= world_offset.y;
    transform.m23 -= world_offset.z;
    m_pParticles->setEmitterTransform(FMat4(transform));
}

FVec3 SVParticlesNode::getParticlePosition(s32 num) const {
    return world_offset + FVec3(m_pParticles->getParticlePosition(num));
}

void SVParticlesNode::getParticleTransforms(SVArray<FMat4> &transforms) const {
    FMat4 transform = Mat4_identity;
    s32 num_particles = m_pParticles->getNumParticles();
    f32 iradius = 1.0f / (m_pParticles->getRadiusMean() + m_pParticles->getRadiusSpread());
    transforms.resize(num_particles);
    if(m_pParticles->getType() == TYPE_FLAT || m_pParticles->getType() == TYPE_RANDOM) {
        for(s32 i = 0; i < num_particles; i++) {
            orthoTransform(FVec3(m_pParticles->getParticleParameters(i)),transform);
            transform.setColumn3(3,FVec3(m_pParticles->getParticlePosition(i)) + world_offset);
            mul(transforms[i],transform,FMat4(scale(FVec3(m_pParticles->getParticleRadius(i) * iradius))));
        }
    } else {
        for(s32 i = 0; i < num_particles; i++) {
            orthoTransform(FVec3(normalize(m_pParticles->getParticleVelocity(i))),transform);
            transform.setColumn3(3,FVec3(m_pParticles->getParticlePosition(i)) + world_offset);
            mul(transforms[i],transform,FMat4(scale(FVec3(m_pParticles->getParticleRadius(i) * iradius))));
        }
    }
}

void SVParticlesNode::setPhysicalMask(s32 mask) {
    physical_mask = mask;
}

s32 SVParticlesNode::getPhysicalMask() const {
    return physical_mask;
}

//************************************** Emitter *****************************
void SVParticlesNode::setEmitterSync(s32 sync) {
    emitter_sync = sync;
}

s32 SVParticlesNode::getEmitterSync() const {
    return emitter_sync;
}

void SVParticlesNode::setEmitterVelocity(const FVec3 &velocity) {
    emitter_velocity = velocity;
}

const FVec3 &SVParticlesNode::getEmitterVelocity() const {
    return emitter_velocity;
}

void SVParticlesNode::addEmitterSpark(const FVec3 &point,const FVec3 &normal,const FVec3 &velocity) const {
    return m_pParticles->addEmitterSpark(FVec3(point - world_offset),normal,velocity);
}

//********************************** Forces ************************************
void SVParticlesNode::setForceTransform(s32 num,const FMat4 &transform) const {
    if(m_pParticles->isForceAttached(num)) {
        m_pParticles->setForceTransform(num,FMat4(transform));
    } else {
         m_pParticles->setForceTransform(num,FMat4(translate(-world_offset) * transform));
    }
}

FMat4 SVParticlesNode::getForceTransform(s32 num) const {
    if(m_pParticles->isForceAttached(num)) {
        return FMat4(m_pParticles->getForceTransform(num));
    }
    return translate(world_offset) * FMat4(m_pParticles->getForceTransform(num));
}

//****************************** Noises********************************************
void SVParticlesNode::setNoiseTransform(s32 num,const FMat4 &transform) const {
    if(m_pParticles->isNoiseAttached(num)) {
        m_pParticles->setNoiseTransform(num,FMat4(transform));
    } else {
        m_pParticles->setNoiseTransform(num,FMat4(translate(-world_offset) * transform));
    }
}

FMat4 SVParticlesNode::getNoiseTransform(s32 num) const {
    if(m_pParticles->isNoiseAttached(num)) {
        return FMat4(m_pParticles->getNoiseTransform(num));
    }
    return translate(world_offset) * FMat4(m_pParticles->getNoiseTransform(num));
}

//        Image *SVParticlesNode::getNoiseImage(s32 num) const {
//            return m_pParticles->getNoiseImage(num);
//        }

//********************************* Deflectors *********************************
void SVParticlesNode::setDeflectorTransform(s32 num,const FMat4 &transform) const {
    if(m_pParticles->isDeflectorAttached(num)) {
        m_pParticles->setDeflectorTransform(num,FMat4(transform));
    } else {
        m_pParticles->setDeflectorTransform(num,FMat4(translate(-world_offset) * transform));
    }
}

FMat4 SVParticlesNode::getDeflectorTransform(s32 num) const {
    if(m_pParticles->isDeflectorAttached(num)) {
        return FMat4(m_pParticles->getDeflectorTransform(num));
    }
    return translate(world_offset) * FMat4(m_pParticles->getDeflectorTransform(num));
}

//******************************* Contact points ******************************
s32 SVParticlesNode::getRandomPoint(FVec3 &ret_point,FVec3 &ret_normal,FVec3 &ret_velocity,s32 surface) {
    s32 num_particles = m_pParticles->getNumParticles();
    if(num_particles) {
        s32 num = mApp->m_pGlobalParam->getRandomInt(0,num_particles);
        FMat4 transform;// = FMat4(getIWorldTransform() * translate(world_offset));
        mul(ret_point,transform,m_pParticles->getParticlePosition(num));
        ret_normal = Vec3_zero;
        mul3(ret_velocity,transform,m_pParticles->getParticleVelocity(num));
        return 1;
    }
    return 0;
}

s32 SVParticlesNode::getNumTriangles(s32 surface) const {
    return num_triangles;
}

//********************************* Bounds *******************************
const SVBoundBox &SVParticlesNode::getSVBoundBox() const {
    return bound_box;
}

void SVParticlesNode::renderVisualizer() {
    //    //渲染发射器范围
    //    s32 t_emit_type = m_pParticles->getEmitterType();
    //    if(t_emit_type == EMITTER_POINT) {
    //
    //    }else if(t_emit_type == EMITTER_SPHERE) {
    //
    //    }else if(t_emit_type == EMITTER_CYLINDER) {
    //
    //    }else if(t_emit_type == EMITTER_BOX) {
    //
    //    }else if(t_emit_type == EMITTER_RANDOM) {
    //
    //    }else if(t_emit_type == EMITTER_SPARK) {
    //
    //    }
}

void SVParticlesNode::testRandomPos(){
    f32 t_sc = 1.0 + rand()%300*0.01f;
    t_sc = 1.0f;
    setScale(t_sc, t_sc, t_sc);
    //
    f32 t_x = rand()%600 - 360.0f;
    f32 t_y = rand()%1000 - 640.0f;
    f32 t_z = rand()%1000 - 500.0f;
    setPosition(t_x, t_y, -t_z);
    //
//    f32 t_rotx = rand()%360;
//    f32 t_roty = rand()%360;
//    f32 t_rotz = rand()%360;
//    setRotation(t_rotx, t_roty, t_rotz);
}

void SVParticlesNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                             RAPIDJSON_NAMESPACE::Value &_objValue) {
    
}

void SVParticlesNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
    
}

//********************************* Clone **************************************
//        SVParticlesNode *SVParticlesNode::clone() const {
//            return copy(new SVParticlesNode());
//        }
//
//        SVParticlesNode *SVParticlesNode::copy(SVParticlesNode *node) const {
//            // parameters
//            node->setParticlesType(getParticlesType());
//            node->setWarming(getWarming());
//            node->setDepthSort(getDepthSort());
//            node->setVariationX(getVariationX());
//            node->setVariationY(getVariationY());
//            node->setTextureAtlas(getTextureAtlas());
//            node->setIntersection(getIntersection());
//            node->setCollision(getCollision());
//            node->setCulling(getCulling());
//            node->setSpawnRate(getSpawnRate());
//            node->setSpawnScale(getSpawnScale());
//            node->setSpawnThreshold(getSpawnThreshold());
//            node->setPhysicalMask(getPhysicalMask());
//            node->setPhysicalMass(getPhysicalMass());
//            node->setLengthStretch(getLengthStretch());
//            node->setLinearDamping(getLinearDamping());
//            node->setAngularDamping(getAngularDamping());
//            node->setGrowthDamping(getGrowthDamping());
//            node->setRestitution(getRestitution());
//            node->setRoughness(getRoughness());
//            // emitter
//            node->setEmitterType(getEmitterType());
//            node->setEmitterEnabled(isEmitterEnabled());
//            node->setEmitterBased(isEmitterBased());
//            node->setEmitterShift(isEmitterShift());
//            node->setEmitterContinuous(isEmitterContinuous());
//            node->setEmitterSequence(getEmitterSequence());
//            node->setEmitterLimit(getEmitterLimit());
//            node->setEmitterSync(getEmitterSync());
//            node->setEmitterSize(getEmitterSize());
//            node->setEmitterDirection(getEmitterDirection());
//            node->setEmitterSpread(getEmitterSpread());
//            node->setEmitterVelocity(getEmitterVelocity());
//
//            // dynamic
//            node->setDelay(getDelayMean(),getDelaySpread());
//            node->setPeriod(getPeriodMean(),getPeriodSpread());
//            node->setDuration(getDurationMean(),getDurationSpread());
//            node->setLife(getLifeMean(),getLifeSpread());
//            node->setVelocity(getVelocityMean(),getVelocitySpread());
//            node->setAngle(getAngleMean(),getAngleSpread());
//            node->setRotation(getRotationMean(),getRotationSpread());
//            node->setRadius(getRadiusMean(),getRadiusSpread());
//            node->setGrowth(getGrowthMean(),getGrowthSpread());
//
//            // gravity
//            node->setGravity(getGravity());
//
//            // forces
//            for(s32 i = 0; i < getNumForces(); i++) {
//                node->addForce();
//                node->setForceAttached(i,isForceAttached(i));
//                node->setForceTransform(i,getForceTransform(i));
//                node->setForceRadius(i,getForceRadius(i));
//                node->setForceAttenuation(i,getForceAttenuation(i));
//                node->setForceAttractor(i,getForceAttractor(i));
//                node->setForceRotator(i,getForceRotator(i));
//            }
//
//            // deflectors
//            for(s32 i = 0; i < getNumDeflectors(); i++) {
//                node->addDeflector();
//                node->setDeflectorType(i,getDeflectorType(i));
//                node->setDeflectorAttached(i,isDeflectorAttached(i));
//                node->setDeflectorTransform(i,getDeflectorTransform(i));
//                node->setDeflectorSize(i,getDeflectorSize(i));
//                node->setDeflectorRestitution(i,getDeflectorRestitution(i));
//                node->setDeflectorRoughness(i,getDeflectorRoughness(i));
//            }
//
//            // copy object
//            Object::copy(node);
//
//            return node;
//        }

////********************************** Load/save world ****************************************
//s32 SVParticlesNode::loadWorld(const Xml *xml) {
//
//    // parameters
//    if(xml->isChild("particles_type")) setParticlesType(xml->getChild("particles_type")->getIntData());
//    if(xml->isChild("warming")) setWarming(xml->getChild("warming")->getBoolData());
//    if(xml->isChild("depth_sort")) setDepthSort(xml->getChild("depth_sort")->getBoolData());
//    if(xml->isChild("variation_x")) setVariationX(xml->getChild("variation_x")->getBoolData());
//    if(xml->isChild("variation_y")) setVariationY(xml->getChild("variation_y")->getBoolData());
//    if(xml->isChild("texture_atlas")) setTextureAtlas(xml->getChild("texture_atlas")->getBoolData());
//    if(xml->isChild("intersection")) setIntersection(xml->getChild("intersection")->getBoolData());
//    if(xml->isChild("collision")) setCollision(xml->getChild("collision")->getBoolData());
//    if(xml->isChild("culling")) setCulling(xml->getChild("culling")->getBoolData());
//    if(xml->isChild("spawn_rate")) setSpawnRate(xml->getChild("spawn_rate")->getFloatData());
//    if(xml->isChild("spawn_scale")) setSpawnScale(xml->getChild("spawn_scale")->getFloatData());
//    if(xml->isChild("spawn_threshold")) setSpawnThreshold(xml->getChild("spawn_threshold")->getFloatData());
//    if(xml->isChild("physical_mask")) setPhysicalMask(xml->getChild("physical_mask")->getIntData());
//    if(xml->isChild("physical_mass")) setPhysicalMass(xml->getChild("physical_mass")->getFloatData());
//    if(xml->isChild("length_stretch")) setLengthStretch(xml->getChild("length_stretch")->getFloatData());
//    if(xml->isChild("linear_damping")) setLinearDamping(xml->getChild("linear_damping")->getFloatData());
//    if(xml->isChild("angular_damping")) setAngularDamping(xml->getChild("angular_damping")->getFloatData());
//    if(xml->isChild("growth_damping")) setGrowthDamping(xml->getChild("growth_damping")->getFloatData());
//    if(xml->isChild("restitution")) setRestitution(xml->getChild("restitution")->getFloatData());
//    if(xml->isChild("roughness")) setRoughness(xml->getChild("roughness")->getFloatData());
//
//    // emitter
//    if(xml->isChild("emitter_type")) setEmitterType(xml->getChild("emitter_type")->getIntData());
//    if(xml->isChild("emitter_enabled")) setEmitterEnabled(xml->getChild("emitter_enabled")->getBoolData());
//    if(xml->isChild("emitter_based")) setEmitterBased(xml->getChild("emitter_based")->getBoolData());
//    if(xml->isChild("emitter_shift")) setEmitterShift(xml->getChild("emitter_shift")->getBoolData());
//    if(xml->isChild("emitter_continuous")) setEmitterContinuous(xml->getChild("emitter_continuous")->getBoolData());
//    if(xml->isChild("emitter_sequence")) setEmitterSequence(xml->getChild("emitter_sequence")->getIntData());
//    if(xml->isChild("emitter_limit")) setEmitterLimit(xml->getChild("emitter_limit")->getIntData());
//    if(xml->isChild("emitter_sync")) setEmitterSync(xml->getChild("emitter_sync")->getBoolData());
//    FVec3 size = getEmitterSize();
//    if(xml->isChild("emitter_size")) xml->getChild("emitter_size")->getFloatArrayData(size,3);
//    setEmitterSize(size);
//    FVec3 direction = getEmitterDirection();
//    if(xml->isChild("emitter_direction")) xml->getChild("emitter_direction")->getFloatArrayData(direction,3);
//    setEmitterDirection(direction);
//    FVec3 spread = getEmitterSpread();
//    if(xml->isChild("emitter_spread")) xml->getChild("emitter_spread")->getFloatArrayData(spread,3);
//    setEmitterSpread(spread);
//    FVec3 velocity = getEmitterVelocity();
//    if(xml->isChild("emitter_velocity")) xml->getChild("emitter_velocity")->getFloatArrayData(velocity,3);
//    setEmitterVelocity(velocity);
//    // dynamic
//    if(xml->isChild("delay_mean") && xml->isChild("delay_spread")) {
//        setDelay(xml->getChild("delay_mean")->getFloatData(),xml->getChild("delay_spread")->getFloatData());
//    }
//    if(xml->isChild("period_mean") && xml->isChild("period_spread")) {
//        setPeriod(xml->getChild("period_mean")->getFloatData(),xml->getChild("period_spread")->getFloatData());
//    }
//    if(xml->isChild("duration_mean") && xml->isChild("duration_spread")) {
//        setDuration(xml->getChild("duration_mean")->getFloatData(),xml->getChild("duration_spread")->getFloatData());
//    }
//    if(xml->isChild("life_mean") && xml->isChild("life_spread")) {
//        setLife(xml->getChild("life_mean")->getFloatData(),xml->getChild("life_spread")->getFloatData());
//    }
//    if(xml->isChild("velocity_mean") && xml->isChild("velocity_spread")) {
//        setVelocity(xml->getChild("velocity_mean")->getFloatData(),xml->getChild("velocity_spread")->getFloatData());
//    }
//    if(xml->isChild("angle_mean") && xml->isChild("angle_spread")) {
//        setAngle(xml->getChild("angle_mean")->getFloatData(),xml->getChild("angle_spread")->getFloatData());
//    }
//    if(xml->isChild("rotation_mean") && xml->isChild("rotation_spread")) {
//        setRotation(xml->getChild("rotation_mean")->getFloatData(),xml->getChild("rotation_spread")->getFloatData());
//    }
//    if(xml->isChild("radius_mean") && xml->isChild("radius_spread")) {
//        setRadius(xml->getChild("radius_mean")->getFloatData(),xml->getChild("radius_spread")->getFloatData());
//    }
//    if(xml->isChild("growth_mean") && xml->isChild("growth_spread")) {
//        setGrowth(xml->getChild("growth_mean")->getFloatData(),xml->getChild("growth_spread")->getFloatData());
//    }
//
//    // gravity
//    FVec3 force = getGravity();
//    if(xml->isChild("gravity")) xml->getChild("gravity")->getFloatArrayData(force,3);
//    setGravity(force);
//
//    for(s32 i = 0; i < xml->getNumChilds(); i++) {
//        const Xml *x = xml->getChild(i);
//        const String &name = x->getName();
//
//        // force
//        if(name == "force") {
//            s32 j = addForce();
//            if(x->isChild("attached")) setForceAttached(j,x->getChild("attached")->getBoolData());
//            FMat4 transform = getForceTransform(j);
//            if(x->isChild("transform")) engine.editor->loadTransform(x->getChild("transform"),transform);
//            setForceTransform(j,transform);
//            if(x->isChild("radius")) setForceRadius(j,x->getChild("radius")->getFloatData());
//            if(x->isChild("attenuation")) setForceAttenuation(j,x->getChild("attenuation")->getFloatData());
//            if(x->isChild("attractor")) setForceAttractor(j,x->getChild("attractor")->getFloatData());
//            if(x->isChild("rotator")) setForceRotator(j,x->getChild("rotator")->getFloatData());
//        }
//
//        // noise
//        else if(name == "noise") {
//            s32 j = addNoise();
//            if(x->isChild("attached")) setNoiseAttached(j,x->getChild("attached")->getBoolData());
//            FMat4 transform = getNoiseTransform(j);
//            if(x->isChild("transform")) engine.editor->loadTransform(x->getChild("transform"),transform);
//            setNoiseTransform(j,transform);
//            FVec3 offset = getNoiseOffset(j);
//            if(x->isChild("offset")) x->getChild("offset")->getFloatArrayData(size,3);
//            setNoiseOffset(j,offset);
//            FVec3 step = getNoiseStep(j);
//            if(x->isChild("step")) x->getChild("step")->getFloatArrayData(size,3);
//            setNoiseStep(j,step);
//            if(x->isChild("force")) setNoiseForce(j,x->getChild("force")->getFloatData());
//            if(x->isChild("scale")) setNoiseScale(j,x->getChild("scale")->getFloatData());
//            if(x->isChild("frequency")) setNoiseFrequency(j,x->getChild("frequency")->getIntData());
//            if(x->isChild("size")) setNoiseSize(j,x->getChild("size")->getIntData());
//        }
//
//        // deflector
//        else if(name == "deflector") {
//            s32 j = addDeflector();
//            if(x->isChild("type")) setDeflectorType(j,x->getChild("type")->getIntData());
//            if(x->isChild("attached")) setDeflectorAttached(j,x->getChild("attached")->getBoolData());
//            FMat4 transform = getDeflectorTransform(j);
//            if(x->isChild("transform")) engine.editor->loadTransform(x->getChild("transform"),transform);
//            setDeflectorTransform(j,transform);
//            FVec3 size = getDeflectorSize(j);
//            if(x->isChild("size")) x->getChild("size")->getFloatArrayData(size,3);
//            setDeflectorSize(j,size);
//            if(x->isChild("restitution")) setDeflectorRestitution(j,x->getChild("restitution")->getFloatData());
//            if(x->isChild("roughness")) setDeflectorRoughness(j,x->getChild("roughness")->getFloatData());
//        }
//    }
//
//    return Object::loadWorld(xml);
//}
//
//s32 SVParticlesNode::saveWorld(Xml *xml) const {
//
//    // parameters
//    if(getParticlesType() != SVParticles::TYPE_BILLBOARD) xml->addChild("particles_type")->setIntData(getParticlesType());
//    if(getWarming() != 1) xml->addChild("warming")->setBoolData(getWarming());
//    if(getDepthSort() != 0) xml->addChild("depth_sort")->setBoolData(getDepthSort());
//    if(getVariationX() != 0) xml->addChild("variation_x")->setBoolData(getVariationX());
//    if(getVariationY() != 0) xml->addChild("variation_y")->setBoolData(getVariationY());
//    if(getTextureAtlas() != 0) xml->addChild("texture_atlas")->setBoolData(getTextureAtlas());
//    if(getIntersection() != 0) xml->addChild("intersection")->setBoolData(getIntersection());
//    if(getCollision() != 0) xml->addChild("collision")->setBoolData(getCollision());
//    if(getCulling() != 0) xml->addChild("culling")->setBoolData(getCulling());
//    if(getSpawnRate() != 20.0f) xml->addChild("spawn_rate")->setFloatData(getSpawnRate());
//    if(getSpawnScale() != 1.0f) xml->addChild("spawn_scale")->setFloatData(getSpawnScale());
//    if(getSpawnThreshold() != 0.0f) xml->addChild("spawn_threshold")->setFloatData(getSpawnThreshold());
//    if(getPhysicalMask() != 1) xml->addChild("physical_mask")->setIntData(getPhysicalMask());
//    if(getPhysicalMass() != 0.0f) xml->addChild("physical_mass")->setFloatData(getPhysicalMass());
//    if(getLengthStretch() != 0.0f) xml->addChild("length_stretch")->setFloatData(getLengthStretch());
//    if(getLinearDamping() != 0.0f) xml->addChild("linear_damping")->setFloatData(getLinearDamping());
//    if(getAngularDamping() != 0.0f) xml->addChild("angular_damping")->setFloatData(getAngularDamping());
//    if(getGrowthDamping() != 0.0f) xml->addChild("growth_damping")->setFloatData(getGrowthDamping());
//    if(getRestitution() != 0.5f) xml->addChild("restitution")->setFloatData(getRestitution());
//    if(getRoughness() != 0.0f) xml->addChild("roughness")->setFloatData(getRoughness());
//
//    // emitter
//    if(getEmitterType() != SVParticles::EMITTER_POINT) xml->addChild("emitter_type")->setIntData(getEmitterType());
//    if(isEmitterEnabled() != 0) xml->addChild("emitter_enabled")->setBoolData(isEmitterEnabled());
//    if(isEmitterBased() != 0) xml->addChild("emitter_based")->setBoolData(isEmitterBased());
//    if(isEmitterShift() != 0) xml->addChild("emitter_shift")->setBoolData(isEmitterShift());
//    if(isEmitterContinuous() != 1) xml->addChild("emitter_continuous")->setBoolData(isEmitterContinuous());
//    if(getEmitterSequence() != 0) xml->addChild("emitter_sequence")->setIntData(getEmitterSequence());
//    if(getEmitterLimit() != 10000) xml->addChild("emitter_limit")->setIntData(getEmitterLimit());
//    if(getEmitterSync() != 1) xml->addChild("emitter_sync")->setBoolData(getEmitterSync());
//    if(getEmitterSize() != vec3_zero) xml->addChild("emitter_size")->setFloatArrayData(getEmitterSize(),3);
//    if(getEmitterDirection() != vec3_zero) xml->addChild("emitter_direction")->setFloatArrayData(getEmitterDirection(),3);
//    if(getEmitterSpread() != vec3_one) xml->addChild("emitter_spread")->setFloatArrayData(getEmitterSpread(),3);
//    if(getEmitterVelocity() != vec3_zero) xml->addChild("emitter_velocity")->setFloatArrayData(getEmitterVelocity(),3);
//
//    // dynamic
//    if(getDelayMean() != 0.0f || getDelaySpread() != 0.0f) {
//        xml->addChild("delay_mean")->setFloatData(getDelayMean());
//        xml->addChild("delay_spread")->setFloatData(getDelaySpread());
//    }
//    if(getPeriodMean() != 0.0f || getPeriodSpread() != 0.0f) {
//        xml->addChild("period_mean")->setFloatData(getPeriodMean());
//        xml->addChild("period_spread")->setFloatData(getPeriodSpread());
//    }
//    if(getDurationMean() != INFINITY || getDurationSpread() != 0.0f) {
//        xml->addChild("duration_mean")->setFloatData(getDurationMean());
//        xml->addChild("duration_spread")->setFloatData(getDurationSpread());
//    }
//    if(getLifeMean() != 1.0f || getLifeSpread() != 0.5f) {
//        xml->addChild("life_mean")->setFloatData(getLifeMean());
//        xml->addChild("life_spread")->setFloatData(getLifeSpread());
//    }
//    if(getVelocityMean() != 1.0f || getVelocitySpread() != 0.5f) {
//        xml->addChild("velocity_mean")->setFloatData(getVelocityMean());
//        xml->addChild("velocity_spread")->setFloatData(getVelocitySpread());
//    }
//    if(getAngleMean() != 0.0f || getAngleSpread() != 180.0f) {
//        xml->addChild("angle_mean")->setFloatData(getAngleMean());
//        xml->addChild("angle_spread")->setFloatData(getAngleSpread());
//    }
//    if(getRotationMean() != 0.0f || getRotationSpread() != 0.0f) {
//        xml->addChild("rotation_mean")->setFloatData(getRotationMean());
//        xml->addChild("rotation_spread")->setFloatData(getRotationSpread());
//    }
//    if(getRadiusMean() != 1.0f || getRadiusSpread() != 0.5f) {
//        xml->addChild("radius_mean")->setFloatData(getRadiusMean());
//        xml->addChild("radius_spread")->setFloatData(getRadiusSpread());
//    }
//    if(getGrowthMean() != 0.0f || getGrowthSpread() != 0.0f) {
//        xml->addChild("growth_mean")->setFloatData(getGrowthMean());
//        xml->addChild("growth_spread")->setFloatData(getGrowthSpread());
//    }
//
//    // gravity
//    if(getGravity() != vec3_zero) xml->addChild("gravity")->setFloatArrayData(getGravity(),3);
//
//    // forces
//    for(s32 i = 0; i < getNumForces(); i++) {
//        Xml *x = xml->addChild("force");
//        x->addChild("attached")->setBoolData(isForceAttached(i));
//        engine.editor->saveTransform(x->addChild("transform"),getForceTransform(i));
//        x->addChild("radius")->setFloatData(getForceRadius(i));
//        x->addChild("attenuation")->setFloatData(getForceAttenuation(i));
//        x->addChild("attractor")->setFloatData(getForceAttractor(i));
//        x->addChild("rotator")->setFloatData(getForceRotator(i));
//    }
//
//    // noises
//    for(s32 i = 0; i < getNumNoises(); i++) {
//        Xml *x = xml->addChild("force");
//        x->addChild("attached")->setBoolData(isNoiseAttached(i));
//        engine.editor->saveTransform(x->addChild("transform"),getNoiseTransform(i));
//        x->addChild("offset")->setFloatArrayData(getNoiseOffset(i),3);
//        x->addChild("step")->setFloatArrayData(getNoiseStep(i),3);
//        x->addChild("force")->setFloatData(getNoiseForce(i));
//        x->addChild("scale")->setFloatData(getNoiseScale(i));
//        x->addChild("frequency")->setIntData(getNoiseFrequency(i));
//        x->addChild("size")->setIntData(getNoiseSize(i));
//    }
//
//    // deflectors
//    for(s32 i = 0; i < getNumDeflectors(); i++) {
//        Xml *x = xml->addChild("deflector");
//        x->addChild("type")->setIntData(getDeflectorType(i));
//        x->addChild("attached")->setBoolData(isDeflectorAttached(i));
//        engine.editor->saveTransform(x->addChild("transform"),getDeflectorTransform(i));
//        x->addChild("size")->setFloatArrayData(getDeflectorSize(i),3);
//        x->addChild("restitution")->setFloatData(getDeflectorRestitution(i));
//        x->addChild("roughness")->setFloatData(getDeflectorRoughness(i));
//    }
//
//    return Object::saveWorld(xml);
//}
//
////********************************** Save/restore state *****************************************
//
//s32 SVParticlesNode::saveState(const Stream &stream) const {
//
//    // save SVParticles
//    if(m_pParticles->saveState(stream) == 0) {
//        Log::error("SVParticlesNode::saveState(): can't save SVParticles state\n");
//        return 0;
//    }
//
//    // physical mask
//    stream.writeInt(getPhysicalMask());
//
//    // emitter synchronization
//    stream.writeUChar(getEmitterSync());
//
//    // emitter velocity
//    stream.writeFloatArray(getEmitterVelocity(),3);
//
//    // world offset
//    stream.writeScalarArray(world_offset,3);
//
//    return Object::saveState(stream);
//}
//
//s32 SVParticlesNode::restoreState(const Stream &stream) {
//
//    // restore SVParticles
//    if(m_pParticles->restoreState(stream) == 0) {
//        //Log::error("SVParticlesNode::restoreState(): can't restore SVParticles state\n");
//        return 0;
//    }
//
//    // physical mask
//    setPhysicalMask(stream.readInt());
//
//    // emitter synchronization
//    setEmitterSync(stream.readUChar());
//
//    // emitter velocity
//    stream.readFloatArray(emitter_velocity,3);
//
//    // world offset
//    stream.readScalarArray(world_offset,3);
//
//    return Object::restoreState(stream);
//}
        

