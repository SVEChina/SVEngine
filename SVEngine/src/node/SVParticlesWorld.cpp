#include "SVParticlesWorld.h"
#include "SVParticlesNode.h"


#define OBJECT_PARTICLES_EPSILON	1e-3f

SVParticlesWorld::SVParticlesWorld() {
    m_particleNode = nullptr;
}

SVParticlesWorld::~SVParticlesWorld() {
    m_particleNode = nullptr;
}

void SVParticlesWorld::setEmitterEnabled(s32 enable) {
    if(!m_particleNode)
        return ;
    for(u32 i = 0; i < m_particleNode->getChildNum(); i++) {
        SVNodePtr t_node = m_particleNode->getChild(i);
        if(strcmp( t_node->getType(), "SVParticlesNode") == 0) {
            SVParticlesNodePtr p = std::static_pointer_cast<SVParticlesNode>(t_node);
            if(p->getEmitterType() == EMITTER_RANDOM) {
                continue;
            }
            if(p->getEmitterType() == EMITTER_SPARK) {
                continue;
            }
            if(p->getEmitterSync() == 0) {
                 continue;
            }
            if(p->isEmitterShift()) {
                continue;
            }
            p->setEmitterEnabled(enable);
        }
    }
}

s32 SVParticlesWorld::getFields(SVArray<WorldField> &fields) {
    fields.clear();
//    SVArrayStack<Node*> nodes;
//    s32 mask = SVParticles->getPhysicalMask();
//    // physical wind
//    engine.world->getIntersection(SVParticles->getWorldSVBoundBox(),Node::PHYSICAL_WIND,nodes);
//    for(s32 i = 0; i < nodes.size(); i++) {
//        PhysicalWind *wind = static_cast<PhysicalWind*>(nodes[i]);
//        if((wind->getPhysicalMask() & mask) == 0) continue;
//        FMat4 transform = FMat4(wind->getIWorldTransform() * translate(SVParticles->getWorldOffset()));
//        FMat4 itransform = FMat4(translate(-SVParticles->getWorldOffset()) * wind->getWorldTransform());
//        Field &f = fields.append();
//        f.transform = transform;
//        mul(f.hsize,wind->getSize(),0.5f);
//        mul3(f.velocity,itransform,wind->getVelocity());
//        f.damping = wind->getLinearDamping();
//    }
//    // physical water
//    engine.world->getIntersection(SVParticles->getWorldSVBoundBox(),Node::PHYSICAL_WATER,nodes);
//    for(s32 i = 0; i < nodes.size(); i++) {
//        PhysicalWater *water = static_cast<PhysicalWater*>(nodes[i]);
//        if((water->getPhysicalMask() & mask) == 0) continue;
//        FMat4 transform = FMat4(water->getIWorldTransform() * translate(SVParticles->getWorldOffset()));
//        FMat4 itransform = FMat4(translate(SVParticles->getWorldOffset()) * water->getWorldTransform());
//        Field &f = fields.append();
//        f.transform = transform;
//        mul(f.hsize,water->getSize(),0.5f);
//        mul3(f.velocity,itransform,water->getVelocity());
//        f.damping = water->getLinearDamping();
//    }
//    return (fields.size() > 0);
    return 0;
}

s32 SVParticlesWorld::getForces(SVArray<WorldForce> &forces) {
    forces.clear();
//    SVArrayStack<Node*> nodes;
//    s32 mask = SVParticles->getPhysicalMask();
//    // physical forces
//    engine.world->getIntersection(SVParticles->getWorldSVBoundBox(),Node::PHYSICAL_FORCE,nodes);
//    for(s32 i = 0; i < nodes.size(); i++) {
//        PhysicalForce *force = static_cast<PhysicalForce*>(nodes[i]);
//        if((force->getPhysicalMask() & mask) == 0) continue;
//        FMat4 itransform = FMat4(translate(-SVParticles->getWorldOffset()) * force->getWorldTransform());
//        Force &f = forces.append();
//        f.position = itransform.getColumn3(3);
//        f.axis = itransform.getColumn3(2);
//        f.iradius = Math::rcp(force->getRadius());
//        f.attenuation = force->getAttenuation();
//        f.attractor = force->getAttractor();
//        f.rotator = force->getRotator();
//    }
    return (forces.size() > 0);
}

s32 SVParticlesWorld::getNoises(SVArray<WorldNoise> &noises) {
    noises.clear();
//    SVArrayStack<Node*> nodes;
//    s32 mask = SVParticles->getPhysicalMask();
//    // physical noises
//    engine.world->getIntersection(SVParticles->getWorldSVBoundBox(),Node::PHYSICAL_NOISE,nodes);
//    for(s32 i = 0; i < nodes.size(); i++) {
//        PhysicalNoise *noise = static_cast<PhysicalNoise*>(nodes[i]);
//        if((noise->getPhysicalMask() & mask) == 0) continue;
//        FMat4 itransform = FMat4(translate(-SVParticles->getWorldOffset()) * noise->getWorldTransform());
//        Noise &n = noises.append();
//        n.itransform = translate(noise->getOffset()) * scale(vec3_one / noise->getStep()) * itransform;
//        n.force = noise->getForce();
//        n.image = noise->getImage();
//    }
    return (noises.size() > 0);
}

s32 SVParticlesWorld::getIntersection(const FVec3 &p0,const FVec3 &p1,WorldContact &contact) {
//    s32 mask = SVParticles->getIntersectionMask(0);
//    // world space points
//    const FVec3 &world_offset = SVParticles->getWorldOffset();
//    FVec3 point_0 = FVec3(p0) + world_offset;
//    FVec3 point_1 = FVec3(p1) + world_offset;
//    // get intersection
//    FVec3 ret_point;
//    s32 ret_surface;
//    Shape *ret_shape;
//    Object *object = engine.physics->getIntersection(point_0,point_1,mask,ret_point,contact.normal,ret_surface,ret_shape);
//    if(object == nullptr)
//        return 0;
//    // SVParticles space points
//    contact.point = FVec3(ret_point - world_offset);
//    contact.roughness = 1.0f;
//    contact.data = object;
//    // restitution
//    if(ret_surface != -1) {
//        Property *property = object->getProperty(ret_surface);
//        s32 t_id = property->findParameter(PROPERTY_RESTITUTION_NAME,PROPERTY_RESTITUTION);
//        if(t_id != -1) {
//            contact.restitution = property->getParameterFloat(t_id);
//        } else {
//            contact.restitution = 0.5f;
//        }
//    } else {
//        contact.restitution = ret_shape->getRestitution();
//    }
    return 1;
}

s32 SVParticlesWorld::getCollision(const FVec3 &point,
                                   const FVec3 &velocity,
                                   f32 radius,
                                   SVArray<WorldContact> &contacts,
                                   f32 ifps) {
    contacts.clear();
    const FVec3 &world_offset = m_particleNode->getWorldOffset();
    s32 ret = 0;
//    s32 collision_mask = m_particleNode->getCollisionMask(0);
//    // shape parameters
//    SVArrayStack<Shape::Contact> shape_contacts;
//    if(m_particleNode->getParticlesType() == SVParticles::TYPE_LENGTH) {
//        f32 length = velocity.length();
//        f32 stretch = m_particleNode->getLengthStretch();
//        if(length > EPSILON && stretch > EPSILON) {
//            FMat4 transform = Mat4_identity;
//            orthoTransform(FVec3(velocity / length),transform);
//            transform.setColumn3(3,FVec3(point + velocity * (stretch * 0.5f)) + world_offset);
//            capsule.setRadius(radius);
//            capsule.setHeight(length * stretch);
//            capsule.setTransform(transform);
//            capsule.setVelocity(velocity,ifps);
//            capsule.setCollisionMask(collision_mask);
//            ret = capsule.getCollision(shape_contacts,ifps);
//        } else {
//            sphere.setRadius(radius);
//            sphere.setTransform(FVec3(point) + world_offset);
//            sphere.setVelocity(velocity,ifps);
//            sphere.setCollisionMask(collision_mask);
//            ret = sphere.getCollision(shape_contacts,ifps);
//        }
//    } else {
//        sphere.setRadius(radius);
//        sphere.setTransform(FVec3(point) + world_offset);
//        sphere.setVelocity(velocity,ifps);
//        sphere.setCollisionMask(collision_mask);
//        ret = sphere.getCollision(shape_contacts,ifps);
//    }

//    // shape contacts
//    for(s32 i = 0; i < shape_contacts.size(); i++) {
//        const Shape::Contact &sc = shape_contacts[i];
//        Contact &c = contacts.append();
//        c.point = FVec3(sc.point - world_offset);
//        c.normal = sc.normal;
//        c.data = sc.object;
//        c.time = sc.time;
//        c.depth = sc.depth;
//        if(sc.shapes[1] != NULL) {
//            c.restitution = shape_contacts[i].shapes[1]->getRestitution();
//        } else {
//            Property *property = sc.object->getProperty(sc.surface);
//            s32 t_id = property->findParameter(PROPERTY_RESTITUTION_NAME,PROPERTY_RESTITUTION);
//            if(t_id != -1) {
//                c.restitution = property->getParameterFloat(t_id);
//            } else {
//                c.restitution = 0.5f;
//            }
//        }
//        c.roughness = 1.0f;
//    }
    return ret;
}

s32 SVParticlesWorld::getRandomPoint(FVec3 &ret_point,FVec3 &ret_normal,FVec3 &ret_velocity) {
//    // parent node
//    Node *parent = SVParticles->getParent();
//    while(parent != NULL && parent->getType() == Node::NODE_DUMMY) {
//        parent = parent->getParent();
//    }
//    // parent node is not an object
//    if(parent == NULL || parent->isObject() == 0) {
//        return SVParticles->getRandomPoint(ret_point,ret_normal,ret_velocity,0);
//    }
//    // parent node is object
//    Object *object = static_cast<Object*>(parent);
//    // parent node is object SVParticles
//    if(object->getType() == Node::OBJECT_PARTICLES) {
//        if(object->isEnabled(0) == 0)
//            return 0;
//        if(object->getMaterial(0) == 0)
//            return 0;
//        SVParticles *p = static_cast<SVParticlesNode*>(object)->getParticles();
//        if(p->getNumParticles() == 0)
//            return 0;
//        s32 num = engine.game->getRandomInt(0,p->getNumParticles());
//        ret_point = p->getParticlePosition(num) + FVec3(static_cast<SVParticlesNode*>(object)->getWorldOffset() - SVParticles->getWorldOffset());
//        ret_normal = vec3_one;
//        ret_velocity = p->getParticleVelocity(num);
//        return 1;
//    }
//
//    // active object surfaces
//    SVArrayStack<s32> surfaces;
//    for(s32 i = 0; i < object->getNumSurfaces(); i++) {
//        if(object->isEnabled(i) == 0)
//            continue;
//        if(object->getMaterial(i) == NULL)
//            continue;
//        surfaces.append(i);
//    }
//    if(surfaces.size() == 0)
//        return 0;
//    // random surface
//    s32 surface = engine.game->getRandomInt(0,surfaces.size());
//    if(object->getRandomPoint(ret_point,ret_normal,ret_velocity,surfaces[surface]) == 0)
//        return 0;
//    // local transformation
//    FMat4 itransform = FMat4(translate(-SVParticles->getWorldOffset()) * object->getWorldTransform());
//    mul(ret_point,itransform,FVec3(ret_point));
//    mul3(ret_normal,itransform,FVec3(ret_normal));
//    mul3(ret_velocity,itransform,FVec3(ret_velocity));
    return 1;
}
