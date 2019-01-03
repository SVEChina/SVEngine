#include "SVParticles.h"
#include "../base/SVSimdLib.h"
#include "../base/SVUtils.h"

#define PARTICLES_IFPS		(1.0f / 30.0f)

SVParticles::SVParticles() {
	m_pWorld = nullptr;
	frame = 0;
	type = TYPE_BILLBOARD;
	warming = 1;
	depth_sort = 0;
	variation_x = 0;
	variation_y = 0;
	texture_atlas = 0;
	intersection = 0;
	collision = 0;
	culling = 0;
	spawn_rate = 20.0f;
	spawn_count = 0.0f;
	spawn_scale = 1.0f;
	spawn_threshold = 0.0f;
	delay_time = 0.0f;
	period_time = 0.0f;
	duration_time = 0.0f;
    duration_mean = 0.0f;
    fade = 0.0f;
    
	setWorldMass(0.0f);
	setLengthStretch(0.0f);
	setLinearDamping(0.0f);
	setAngularDamping(0.0f);
	setGrowthDamping(0.0f);
	setRestitution(0.5f);
	setRoughness(0.0f);
	
	setEmitterType(EMITTER_POINT);
	setEmitterEnabled(0);
	setEmitterBased(0);
	setEmitterShift(0);
	setEmitterContinuous(1);
	setEmitterSequence(0);
	setEmitterLimit(10000);
	setEmitterSize(FVec3_zero);
	setEmitterDirection(FVec3_zero);
	setEmitterSpread(FVec3_zero);
	setEmitterVelocity(FVec3_zero);
	setEmitterTransform(FMat4_identity);
	
	setDelay(0.0f,0.0f);
	setPeriod(0.0f,0.0f);
	setDuration(INFINITY,0.0f);
	setLife(1.0f,0.5f);
	setVelocity(1.0f,0.5f);
	setAngle(0.0f,180.0f);
	setRotation(0.0f,0.0f);
	setRadius(1.0f,0.5f);
	setGrowth(0.0f,0.0f);
	setGravity(FVec3_zero);
	update_bounds();
    //
    pVertex = nullptr;
    FVec3 color1(255/255.0f, 146/255.0f, 218/255.0f);
    FVec3 color2(126/255.0f, 168/255.0f, 255/255.0f);
    m_vetexColor.append(color1);
    m_vetexColor.append(color2);
}

SVParticles::~SVParticles() {
    for(s32 i = 0; i < noises.size(); i++) {
        //delete noises[i].image;
    }
}

//*************************** Clear ******************************************
void SVParticles::clear() {
	distances.clear();
	m_particles.clear();
	sparks.clear();
	contacts.clear();
	old_contacts.clear();
}

s32 SVParticles::getParticleNum() {
    return m_particles.size();
}

//*********************************** Update ****************************
void SVParticles::update(f32 ifps) {
	f32 time = ifps;
	// warming particles
	if(frame++ == 0 && warming) {
		time += life_mean + life_spread;
	}
	// 切换碰撞队列 switch collision arrays
	contacts.swap(old_contacts);
	contacts.clear();
    //获取世界中与粒子交互的区域和对象
    SVArray<WorldField> world_fields;   //获取世界中的防守
    SVArray<WorldForce> world_forces;   //获取世界中的强制区域
    SVArray<WorldNoise> world_noises;   //获取世界中的噪声区域
    if(m_pWorld && world_mass > EPSILON) {
        m_pWorld->getFields(world_fields);
        m_pWorld->getForces(world_forces);
        m_pWorld->getNoises(world_noises);
    }
	// update particles
	s32 iteration = 0;
	f32 offset = 0.0f;
	s32 has_particles = 0;
	do {
		//保证按照30帧每秒的速率 进行生产
		f32 ifps = min(time - offset,PARTICLES_IFPS);
		//大量生产粒子(spawn)
		s32 num_particles = 0;
		if(emitter_shift) {
            //发射器如果运动的话 根据距离来计算生产的粒子数目
			FVec3 position = emitter_transform.getColumn3(3);
			FVec3 old_position = old_emitter_transform.getColumn3(3);
			spawn_count += length(position - old_position) * spawn_rate * spawn_scale;
			num_particles = Math::ftoi(spawn_count);
			spawn_count -= Math::itof(num_particles);
		} else if(emitter_type == EMITTER_SPARK) {
            //火花发射器下的粒子数目
			num_particles = Math::ftoi(spawn_rate * spawn_scale);
		} else {
            //普通状态下
			spawn_count += spawn_rate * spawn_scale * ifps;
			num_particles = Math::ftoi(spawn_count);
			spawn_count -= Math::itof(num_particles);
		}
        if(num_particles > emitter_limit){
            num_particles = emitter_limit;
        }
        if(num_particles > 0) {
            has_particles = 1;
        }
		//创建新的粒子
		if(emitter_enabled && delay_time <= 0.0f && duration_time >= 0.0f) {
            if(time > EPSILON) {
                spawn_particles(num_particles,offset,ifps,time);
            }
            if(iteration == 0 && m_pWorld){
                m_pWorld->setEmitterEnabled(1);
            }
		} else {
			old_emitter_transform = emitter_transform;
            if(iteration == 0 && m_pWorld) {
                m_pWorld->setEmitterEnabled(0);
            }
		}
		//重新计算 delay_time，duration_time，period_time
		if(delay_time <= 0.0f) {
			duration_time -= ifps;
			if(duration_time <= 0.0f) {
				period_time -= ifps;
				if(period_time <= 0.0f) {
					period_time = period_mean + random.getFloat(-period_spread,period_spread);
					duration_time = duration_mean + random.getFloat(-duration_spread,duration_spread);
				}
			}
		} else {
			delay_time -= ifps;
		}
        //更新各种干扰器的矩阵
        // force parameters
        for(s32 i = 0; i < forces.size(); i++) {
            Force &f = forces[i];
            FMat4 transform = (f.attached) ? emitter_transform * f.transform : f.transform;
            f.position = transform.getColumn3(3);
            f.axis = transform.getColumn3(2);
            f.iradius = Math::rcp(f.radius);
        }
        // noise parameters
        for(s32 i = 0; i < noises.size(); i++) {
            Noise &n = noises[i];
            FMat4 transform = (n.attached) ? emitter_transform * n.transform : n.transform;
            n.itransform = translate(n.offset) * scale(FVec3_one / n.step) * transform;
        }
		//更新例子
		update_particles(world_fields,world_forces,world_noises,iteration,ifps);
		iteration++;
		offset += ifps;
	} while(time - offset > EPSILON);
    
	// 有粒子产生 就更新发射器位置
    if(has_particles) {
        old_emitter_transform = emitter_transform;
    }
    
    //////////////// 粒子反射和裁剪 /////////////////
    //弯曲 偏离 倾斜
    for(s32 i = 0; i < deflectors.size(); i++) {
        const Deflector &d = deflectors[i];
        //修正矩阵
        FMat4 transform = (d.attached) ? emitter_transform * d.transform : d.transform;
        //修正位置，切线，副法线
        FVec3 position = transform.getColumn3(3);
        FVec3 tangent = transform.getColumn3(0);
        FVec3 binormal = transform.getColumn3(1);
        //修正法线和平面
        FVec3 normal = normalize(transform.getColumn3(2));
        FVec4 plane = FVec4(normal,-dot(normal,position));
        if(d.type == DEFLECTOR_REFLECTOR) {
            //反射
            FVec3 point,direction,x,y;
            for(s32 j = 0; j < m_particles.size(); j++) {
                Particle &t_p = m_particles[j];
                f32 normal_velocity = dot(normal,t_p.velocity);
                if(normal_velocity >= 0.0f)
                    continue;
                sub(direction,t_p.position,t_p.old_position);
                f32 k = -dot(plane,t_p.old_position) / dot3(plane,direction);
                if(k < 0.0f || k > 1.0f)
                    continue;
                mad(point,direction,k,t_p.old_position);
                sub(direction,point,position);
                if(Math::abs(dot(direction,tangent)) > d.size.x)
                    continue;
                if(Math::abs(dot(direction,binormal)) > d.size.y)
                    continue;
                //生成接触点(位置，法线，速度)
                Contact &c = contacts.append();
                c.point = point;
                c.normal = normal;
                c.velocity = t_p.velocity;
                c.data = NULL;
                if(culling) {
                    m_particles.removeFast(j--);
                    continue;
                }
                f32 ifps = length(t_p.position - point) / length(t_p.velocity);
                mad(t_p.velocity,normal,-(1.0f + restitution * d.restitution) * normal_velocity,t_p.velocity);
                if(roughness * d.roughness > EPSILON) {
                    //moca
                    cross(x,normal,t_p.velocity);
                    cross(y,x,t_p.velocity);
                    x.normalize();
                    y.normalize();
                    f32 velocity = length(t_p.velocity);
                    f32 scattering = velocity * roughness * d.roughness;
                    mad(t_p.velocity,x,scattering * random.getFloat(-0.5f,0.5f),t_p.velocity);
                    mad(t_p.velocity,y,scattering * random.getFloat(-0.5f,0.5f),t_p.velocity);
                    mul(t_p.velocity,t_p.velocity,velocity / length(t_p.velocity));
                }
                mad(t_p.position,t_p.velocity,ifps,point);
                //最后计算位置和速度
                mad(t_p.position,normal,1e-3f,t_p.position);
                mad(t_p.velocity,gravity,ifps,t_p.velocity);
            }
        } else if(d.type == DEFLECTOR_CLIPPER) {
            // 裁剪剔除
            FVec3 point,direction,p,x,y;
            for(s32 j = 0; j < m_particles.size(); j++) {
                Particle &t_p = m_particles[j];
                f32 normal_velocity = dot(normal,t_p.velocity);
                if(normal_velocity >= 0.0f)
                    continue;
                sub(direction,t_p.position,t_p.old_position);
                f32 k = -dot(plane,t_p.old_position) / dot3(plane,direction);
                if(k < 0.0f || k > 1.0f)
                    continue;
                mad(point,direction,k,t_p.old_position);
                sub(direction,point,position);
                if(Math::abs(dot(direction,tangent)) > d.size.x)
                    continue;
                if(Math::abs(dot(direction,binormal)) > d.size.y)
                    continue;
                m_particles.removeFast(j--);
            }
        }
    }
    //////////////// 和世界求交 /////////////////
    if(m_pWorld && intersection) {
        FVec3 x,y;
        WorldContact contact;
        for(s32 i = 0; i < m_particles.size(); i++) {
            Particle &p = m_particles[i];
            if(p.old_position == p.position)
                continue;
            if(m_pWorld->getIntersection(p.old_position,p.position,contact) == 0)
                continue;
            f32 normal_velocity = dot(contact.normal,p.velocity);
            if(normal_velocity >= 0.0f)
                continue;
            Contact &c = contacts.append();
            c.point = contact.point;
            c.normal = contact.normal;
            c.velocity = p.velocity;
            c.data = contact.data;
            if(culling) {
                m_particles.removeFast(i--);
                continue;
            }
            f32 ifps = length(p.position - contact.point) / length(p.velocity);
            mad(p.velocity,contact.normal,
                -(1.0f + restitution * contact.restitution) * normal_velocity,
                p.velocity);
            if(roughness * contact.roughness > EPSILON) {
                cross(x,contact.normal,p.velocity);
                cross(y,x,p.velocity);
                x.normalize();
                y.normalize();
                f32 velocity = length(p.velocity);
                f32 scattering = velocity * roughness * contact.roughness;
                mad(p.velocity,x,scattering * random.getFloat(-0.5f,0.5f),p.velocity);
                mad(p.velocity,y,scattering * random.getFloat(-0.5f,0.5f),p.velocity);
                mul(p.velocity,p.velocity,velocity / length(p.velocity));
            }
            //计算位置和速度
            mad(p.position,p.velocity,ifps,contact.point);
            mad(p.position,contact.normal,1e-3f,p.position);
            mad(p.velocity,gravity,ifps,p.velocity);
        }
    }

    //////////////////  世界碰撞 ////////////////////////
    if(m_pWorld && collision) {
        SVArray<WorldContact> world_contacts;
        for(s32 i = 0; i < m_particles.size(); i++) {
            Particle &p = m_particles[i];
            if( m_pWorld->getCollision(p.position,p.velocity,p.radius,world_contacts,ifps) == 0 )
                continue;
            if(culling) {
                for(s32 j = 0; j < world_contacts.size(); j++) {
                    const WorldContact &contact = world_contacts[j];
                    f32 normal_velocity = dot(contact.normal,p.velocity);
                    if(normal_velocity >= 0.0f)
                        continue;
                    Contact &c = contacts.append();
                    c.point = contact.point;
                    c.normal = contact.normal;
                    c.velocity = p.velocity;
                    c.data = contact.data;
                }
                m_particles.removeFast(i--);
            } else {
                for(s32 j = 0; j < world_contacts.size(); j++) {
                    const WorldContact &contact = world_contacts[j];
                    f32 normal_velocity = dot(contact.normal,p.velocity);
                    if(normal_velocity >= 0.0f)
                        continue;
                    Contact &c = contacts.append();
                    c.point = contact.point;
                    c.normal = contact.normal;
                    c.velocity = p.velocity;
                    c.data = contact.data;
                    mad(p.position,p.velocity,contact.time,p.position);
                    mad(p.position,contact.normal,contact.depth,p.position);
                    mad(p.velocity,contact.normal,
                        -(1.0f + restitution * contact.restitution) * normal_velocity,
                        p.velocity);
                }
            }
        }
    }
	//更新粒子包围盒
	update_particles_bounds();
	//
	update_bounds();
}

// render destroy
void SVParticles::destroy() {
    if(pVertex) {
        delete [] pVertex;
        pVertex = nullptr;
    }
}

// render particles
s32 SVParticles::render(const FMat4 &modelview,const FVec3 &camera) {
    // number of particles
    s32 num_particles = m_particles.size();
    s32 particles_num_vertex = num_particles * 4;
    s32 particles_num_indices = num_particles * 6;
    if(num_particles == 0) {
        return 0;
    }
    //创建粒子啊
    if(!pVertex) {
        //创建2倍大小的空间
        m_vertexBufNum = particles_num_vertex * 2;
        pVertex = new V3_PARTICLE[m_vertexBufNum];
    }else {
        if(particles_num_vertex>m_vertexBufNum) {
            //重新分配粒子
            delete [] pVertex;
            m_vertexBufNum = particles_num_vertex * 2;
            pVertex = new V3_PARTICLE[m_vertexBufNum];
        }
    }
    create_particles(pVertex,modelview,camera);
    SV_LOG_INFO("particle num %d \n",particles_num_vertex*2);
    return num_particles;
}

//发射粒子
void SVParticles::spawn_particle(Particle &p,f32 k,f32 ifps) {
	//随机一个初始速度
	FVec3 velocity;
	velocity.x = random.getFloat(-1.0f,1.0f);
	velocity.y = random.getFloat(-1.0f,1.0f);
	velocity.z = random.getFloat(-1.0f,1.0f);
    //速度归一化
	mul(velocity,velocity,Math::rsqrt(length2(velocity)));
    //发射器 对于速度的传播的影响 mul
	mul(velocity,velocity,emitter_spread);
    //发射器方向的影响 aadd
	add(velocity,velocity,emitter_direction);
    //平均速度的影响
	mul(velocity,velocity,Math::rsqrtFast(length2(velocity)) * (velocity_mean + random.getFloat(-velocity_spread,velocity_spread)));
    //粒子速度 = 发射器矩形 * 生成的速度
	mul3(p.velocity,emitter_transform,velocity);
    //叠加发射器本身的速度
	add(p.velocity,emitter_velocity,p.velocity);
    //叠加重力的影响 最后计算出粒子的速度
	mad(p.velocity,gravity,-k * ifps,p.velocity);
	//角度 = -45.0f + 平均角度 + 随机角度
	p.angle = (-45.0f + angle_mean + random.getFloat(-angle_spread,angle_spread)) * DEG2RAD;
    //旋转 = 平均旋转 + 随机旋转
	p.rotation = (rotation_mean + random.getFloat(-rotation_spread,rotation_spread)) * DEG2RAD;
	//半径 = 平均半径 + 随机半径
	do {
		p.radius = radius_mean + random.getFloat(-radius_spread,radius_spread);
	} while(p.radius < EPSILON);
	//增长值 = 平均增长值 + 随机增长值
	p.growth = growth_mean + random.getFloat(-growth_spread,growth_spread);
	//生命周期 = 平均生命周期 + 随机生命周期
	do {
		p.life = life_mean + random.getFloat(-life_spread,life_spread);
	} while(p.life < EPSILON);
    // ilife
	p.ilife = 65535.0f / p.life;
	//方向...
    if(texture_atlas) {
        p.orientation = random.getInt(12,28);
    } else if(variation_x && variation_y) {
        p.orientation = random.getInt(0,8);
    } else if(variation_x) {
        p.orientation = random.getInt(8,10);
    } else if(variation_y) {
        p.orientation = random.getInt(10,12);
    } else {
        p.orientation = 0;
    }
    //随机一个顶点颜色
    _getRandomVextexColor(p.color);
    //生成params
	switch(type) {
		case TYPE_FLAT:
            //发射器矩阵 Z轴变换
			p.parameters = emitter_transform.getColumn3(2);
			break;
		case TYPE_LENGTH:
            //发射器速度(Fvec3)
			p.parameters = emitter_velocity;
			break;
		case TYPE_RANDOM:
            //随机值 并归一化
			p.parameters.x = random.getFloat(-1.0f,1.0f);
			p.parameters.y = random.getFloat(-1.0f,1.0f);
			p.parameters.z = random.getFloat(-1.0f,1.0f);
			p.parameters.normalizeFast();
			break;
		case TYPE_ROUTE:
            //发射器矩阵 X轴变换 并乘根号2
			p.parameters = emitter_transform.getColumn3(0);
			mul(p.parameters,p.parameters,SQRT2);
			break;
	}
    
	//生成位置
	if(emitter_type == EMITTER_POINT) {
        // 点发射器
        if(emitter_continuous) {
            //连续 插值
            lerp(p.position,old_emitter_transform.getColumn3(3),emitter_transform.getColumn3(3),k);
        }else{
            //非连续
            p.position = emitter_transform.getColumn3(3);
        }
	} else if(emitter_type == EMITTER_SPHERE) {
        //球行发射器
		while(1) {
			p.position.x = random.getFloat(-1.0f,1.0f);
			p.position.y = random.getFloat(-1.0f,1.0f);
			p.position.z = random.getFloat(-1.0f,1.0f);
            if(dot(p.position,p.position) < 1.0f){
                break;
            }
		}
		p.position *= emitter_size.x;   //发射器尺寸 x代表半径
        if(emitter_continuous) {
            //连续
            lerp(p.position,old_emitter_transform * p.position,emitter_transform * p.position,k);
        } else {
            //非连续
            p.position = emitter_transform * p.position;
        }
	} else if(emitter_type == EMITTER_CYLINDER) {
        // 圆柱发射器
        // emitter_size.x 半径
        // emitter_size.y 高度
		while(1) {
			p.position.x = random.getFloat(-1.0f,1.0f);
			p.position.y = random.getFloat(-1.0f,1.0f);
			p.position.z = 0.0f;
            if(dot(p.position,p.position) < 1.0f) {
                break;
            }
		}
		p.position *= emitter_size.x;
		p.position.z = random.getFloat(-0.5f,0.5f) * emitter_size.y;
        if(emitter_continuous) {
            //连续
            lerp(p.position,old_emitter_transform * p.position,emitter_transform * p.position,k);
        } else {
            //非连续
            p.position = emitter_transform * p.position;
        }
	} else if(emitter_type == EMITTER_BOX) {
        // 盒子发射器
		p.position.x = random.getFloat(-0.5f,0.5f) * emitter_size.x;
		p.position.y = random.getFloat(-0.5f,0.5f) * emitter_size.y;
		p.position.z = random.getFloat(-0.5f,0.5f) * emitter_size.z;
        if(emitter_continuous) {
            //连续
            lerp(p.position,old_emitter_transform * p.position,emitter_transform * p.position,k);
        } else {
            //非连续
            p.position = emitter_transform * p.position;
        }
	}
	//位置
	mad(p.position,p.velocity,-k * ifps,p.position);
	p.old_position = p.position;
}

//spawn 产卵 大量产生
void SVParticles::spawn_particles(s32 num_particles,f32 offset,f32 ifps,f32 time) {
	offset /= time;
	f32 inum_particles = 0.0f;
    if(num_particles) {
        inum_particles = ifps * Math::rcp(Math::itof(num_particles)) / time;
    }
	
	if(emitter_type == EMITTER_RANDOM) {
        //随机发射器
		if(m_pWorld) {
			FVec3 point,normal,velocity;
			FVec3 old_emitter_direction = emitter_direction;
			FMat3 emitter_irotation = inverse(FMat3(emitter_transform));
			for(s32 i = 0; i < num_particles; i++) {
				if(m_pWorld->getRandomPoint(point,normal,velocity)) {
					if(length2(velocity) < spawn_threshold * spawn_threshold)
                        continue;
                    Particle &p = m_particles.append();
                    p.position = point;
                    mul(emitter_direction,emitter_irotation,normal);
                    mul(emitter_direction,emitter_direction,old_emitter_direction);
                    spawn_particle(p,offset + Math::itof(i) * inum_particles,ifps);
                    add(p.velocity,p.velocity,velocity);
				}
			}
			emitter_direction = old_emitter_direction;
		}
	} else if(emitter_type == EMITTER_SPARK) {
        //火舌发射器
		FVec3 tangent,binormal;
		f32 spawn_ithreshold = INFINITY;
		if(spawn_threshold > EPSILON) spawn_ithreshold = Math::rcp(spawn_threshold);
		FMat4 old_emitter_transform = emitter_transform;
		for(s32 i = 0; i < sparks.size(); i++) {
			const Spark &s = sparks[i];
			s32 spark_num_particles = Math::ftoi(num_particles * saturate(length(s.velocity) * spawn_ithreshold));
			if(spark_num_particles == 0)
                continue;
			orthoTransform(s.normal,emitter_transform);
			emitter_transform.setColumn3(3,s.point);
            for(s32 j = 0; j < spark_num_particles; j++) {
                Particle &p = m_particles.append();
                spawn_particle(p,1.0f,ifps);
            }
		}
		emitter_transform = old_emitter_transform;
		sparks.clear();
	} else {
        //默认发射器
		for(s32 i = 0; i < num_particles; i++) {
			spawn_particle(m_particles.append(),offset + Math::itof(i) * inum_particles,ifps);
		}
	}
}

//
void SVParticles::update_particles(SVArray<WorldField> &world_fields,
                                   SVArray<WorldForce> &world_forces,
                                   SVArray<WorldNoise> &world_noises,
                                   s32 iteration,
                                   f32 ifps){
	
	// update parameters
	FVec3 position,direction,rotation;
	f32 world_imass_ifps = world_imass * ifps;
    //线程阻尼 影响线程缩放参数
	f32 linear_scale = Math::exp(-linear_damping * ifps);
    //角度阻尼 影响角度缩放参数
	f32 angular_scale = Math::exp(-angular_damping * ifps);
    //生长阻尼 影响生长缩放参数
	f32 growth_scale = Math::exp(-growth_damping * ifps);
    //重力加速度与线性阻尼 影响速度
	FVec3 velocity = gravity * linear_scale * ifps;
    //遍历所有粒子 每个粒子被所有的forces处理一遍
    if(forces.size()) {
        for(s32 i = 0; i < m_particles.size(); i++) {
            Particle &p = m_particles[i];
            for(s32 j = 0; j < forces.size(); j++) {
                const Force &f = forces[j];
                f32 radius = sub(direction,p.position,f.position).length();
                if(radius < EPSILON)
                    continue;
                f32 force = Math::pow(max(1.0f - radius * f.iradius,0.0f),f.attenuation) * ifps;
                if(force < EPSILON)
                    continue;
                cross(rotation,direction,f.axis);
                f32 l = length2(rotation);
                mad(p.velocity,direction,f.attractor * force * Math::rcp(radius),p.velocity);
                if(l > EPSILON) {
                    mad(p.velocity,rotation,f.rotator * force * Math::rsqrt(l),p.velocity);
                }
            }
        }
    }
    if(noises.size()) {
        for(s32 i = 0; i < noises.size(); i++) {
//            const Noise &n = noises[i];
//            Image *image = getNoiseImage(i);
//            const FMat4 &itransform = n.itransform;
//            f32 force = n.force * ifps / 127.5f;
//            if(Math::abs(force) < EPSILON) {
//                continue;
//            }
//            for(s32 j = 0; j < m_particles.size(); j++) {
//                Particle &p = m_particles[j];
//                mul(position,itransform,p.position);
//                Image::Pixel pixel = image->get3D(position.x,position.y,position.z);
//                direction.x = Math::itof(pixel.i.r) - 127.5f;
//                direction.y = Math::itof(pixel.i.g) - 127.5f;
//                direction.z = Math::itof(pixel.i.b) - 127.5f;
//                mad(p.velocity,direction,p.radius * p.radius * force,p.velocity);
//            }
        }
    }
    //世界中区域，强制和扰动(噪声)
    // world fields
    if(world_fields.size()) {
        for(s32 i = 0; i < m_particles.size(); i++) {
            Particle &p = m_particles[i];
            for(s32 j = 0; j < world_fields.size(); j++) {
                const WorldField &f = world_fields[j];
                mul(position,f.transform,p.position);
                if(position.x < -f.hsize.x || position.x > f.hsize.x)
                    continue;
                if(position.y < -f.hsize.y || position.y > f.hsize.y)
                    continue;
                if(position.z < -f.hsize.z || position.z > f.hsize.z)
                    continue;
                f32 impulse = PI * p.radius * p.radius * f.damping * world_imass_ifps;
                sub(direction,f.velocity,p.velocity);
                mul(direction,direction,min(impulse,1.0f));
                add(p.velocity,p.velocity,direction);
            }
        }
    }
    // world forces
    if(world_forces.size()) {
        for(s32 i = 0; i < m_particles.size(); i++) {
            Particle &p = m_particles[i];
            for(s32 j = 0; j < world_forces.size(); j++) {
                const WorldForce &f = world_forces[j];
                f32 radius = sub(direction,p.position,f.position).length();
                if(radius < EPSILON)
                    continue;
                f32 force = Math::pow(max(1.0f - radius * f.iradius,0.0f),f.attenuation) * world_imass_ifps;
                if(force < EPSILON)
                    continue;
                cross(rotation,direction,f.axis);
                f32 l = length2(rotation);
                mad(p.velocity,direction,f.attractor * force * Math::rcp(radius),p.velocity);
                if(l > EPSILON) {
                    mad(p.velocity,rotation,f.rotator * force * Math::rsqrt(l),p.velocity);
                }
            }
        }
    }
    // world noises
    if(world_noises.size()) {
        for(s32 i = 0; i < world_noises.size(); i++) {
            const WorldNoise &n = world_noises[i];
            const FMat4 &itransform = n.itransform;
            f32 force = n.force * world_imass_ifps / 127.5f;
            if(Math::abs(force) < EPSILON)
                continue;
//            for(s32 j = 0; j < m_particles.size(); j++) {
//                Particle &p = m_particles[j];
//                mul(position,itransform,p.position);
//                Image::Pixel pixel = n.image->get3D(position.x,position.y,position.z);
//                direction.x = Math::itof(pixel.i.r) - 127.5f;
//                direction.y = Math::itof(pixel.i.g) - 127.5f;
//                direction.z = Math::itof(pixel.i.b) - 127.5f;
//                mad(p.velocity,direction,p.radius * p.radius * force,p.velocity);
//            }
        }
    }
	/////////////////////// integrate particles //////////
    // 使用一体化
	// clear remove buffer
	remove.clear();
	s32 num_particles = m_particles.size();
	Particle *src = m_particles.get();
	#ifdef USE_SSE
		__m128 linear_scale_vec = _mm_set1_ps(linear_scale);
		__m128 ifps_vec = _mm_set1_ps(ifps);
	#elif USE_ALTIVEC
		vec_float4 linear_scale_vec = vec_splats(linear_scale);
		vec_float4 ifps_vec = vec_splats(ifps);
	#elif USE_NEON
		float32x4_t linear_scale_vec = vdupq_n_f32(linear_scale);
		float32x4_t ifps_vec = vdupq_n_f32(ifps);
	#endif
	while(num_particles >= 4) {
		Particle &p0 = src[0];
		Particle &p1 = src[1];
		Particle &p2 = src[2];
		Particle &p3 = src[3];
		// save old positions
		if(iteration == 0) {
			p0.old_position = p0.position;
			p1.old_position = p1.position;
			p2.old_position = p2.position;
			p3.old_position = p3.position;
		}
        //计算粒子速率和位置
		#ifdef USE_SSE
			p0.velocity.vec = _mm_add_ps(_mm_mul_ps(p0.velocity.vec,linear_scale_vec),velocity.vec);
			p1.velocity.vec = _mm_add_ps(_mm_mul_ps(p1.velocity.vec,linear_scale_vec),velocity.vec);
			p2.velocity.vec = _mm_add_ps(_mm_mul_ps(p2.velocity.vec,linear_scale_vec),velocity.vec);
			p3.velocity.vec = _mm_add_ps(_mm_mul_ps(p3.velocity.vec,linear_scale_vec),velocity.vec);
			
			p0.position.vec = _mm_add_ps(_mm_mul_ps(p0.velocity.vec,ifps_vec),p0.position.vec);
			p1.position.vec = _mm_add_ps(_mm_mul_ps(p1.velocity.vec,ifps_vec),p1.position.vec);
			p2.position.vec = _mm_add_ps(_mm_mul_ps(p2.velocity.vec,ifps_vec),p2.position.vec);
			p3.position.vec = _mm_add_ps(_mm_mul_ps(p3.velocity.vec,ifps_vec),p3.position.vec);
		#elif USE_ALTIVEC
			p0.velocity.vec = vec_madd(p0.velocity.vec,linear_scale_vec,velocity.vec);
			p1.velocity.vec = vec_madd(p1.velocity.vec,linear_scale_vec,velocity.vec);
			p2.velocity.vec = vec_madd(p2.velocity.vec,linear_scale_vec,velocity.vec);
			p3.velocity.vec = vec_madd(p3.velocity.vec,linear_scale_vec,velocity.vec);
			
			p0.position.vec = vec_madd(p0.velocity.vec,ifps_vec,p0.position.vec);
			p1.position.vec = vec_madd(p1.velocity.vec,ifps_vec,p1.position.vec);
			p2.position.vec = vec_madd(p2.velocity.vec,ifps_vec,p2.position.vec);
			p3.position.vec = vec_madd(p3.velocity.vec,ifps_vec,p3.position.vec);
		#elif USE_NEON
			p0.velocity.vec = vmlaq_f32(velocity.vec,p0.velocity.vec,linear_scale_vec);
			p1.velocity.vec = vmlaq_f32(velocity.vec,p1.velocity.vec,linear_scale_vec);
			p2.velocity.vec = vmlaq_f32(velocity.vec,p2.velocity.vec,linear_scale_vec);
			p3.velocity.vec = vmlaq_f32(velocity.vec,p3.velocity.vec,linear_scale_vec);
			
			p0.position.vec = vmlaq_f32(p0.position.vec,p0.velocity.vec,ifps_vec);
			p1.position.vec = vmlaq_f32(p1.position.vec,p1.velocity.vec,ifps_vec);
			p2.position.vec = vmlaq_f32(p2.position.vec,p2.velocity.vec,ifps_vec);
			p3.position.vec = vmlaq_f32(p3.position.vec,p3.velocity.vec,ifps_vec);
		#else
            //计算速度 向量加法
            //v = v*x + v1
			mad(p0.velocity,p0.velocity,linear_scale,velocity);
			mad(p1.velocity,p1.velocity,linear_scale,velocity);
			mad(p2.velocity,p2.velocity,linear_scale,velocity);
			mad(p3.velocity,p3.velocity,linear_scale,velocity);
            //计算位置 p = p + vt
			mad(p0.position,p0.velocity,ifps,p0.position);
			mad(p1.position,p1.velocity,ifps,p1.position);
			mad(p2.position,p2.velocity,ifps,p2.position);
			mad(p3.position,p3.velocity,ifps,p3.position);
		#endif
        //角速度(影响角度)
		p0.rotation *= angular_scale;
		p1.rotation *= angular_scale;
		p2.rotation *= angular_scale;
		p3.rotation *= angular_scale;
        //角度
        p0.angle += p0.rotation * ifps;
        p1.angle += p1.rotation * ifps;
        p2.angle += p2.rotation * ifps;
        p3.angle += p3.rotation * ifps;
		//增长(影响半径)
		p0.growth *= growth_scale;
		p1.growth *= growth_scale;
		p2.growth *= growth_scale;
		p3.growth *= growth_scale;
		//半径
		p0.radius += p0.growth * ifps;
		p1.radius += p1.growth * ifps;
		p2.radius += p2.growth * ifps;
		p3.radius += p3.growth * ifps;
		//生命
		p0.life -= ifps;
		p1.life -= ifps;
		p2.life -= ifps;
		p3.life -= ifps;
        //更新后 符合条件的粒子，进入移除
        if(p0.radius < EPSILON || p0.life < EPSILON) {
            remove.append((s32)(&p0 - m_particles.get()));
        }
        if(p1.radius < EPSILON || p1.life < EPSILON) {
            remove.append((s32)(&p1 - m_particles.get()));
        }
        if(p2.radius < EPSILON || p2.life < EPSILON) {
            remove.append((s32)(&p2 - m_particles.get()));
        }
        if(p3.radius < EPSILON || p3.life < EPSILON) {
            remove.append((s32)(&p3 - m_particles.get()));
        }
		src += 4;
		num_particles -= 4;
	}
    
	//计算残余粒子
	while(num_particles >= 1) {
		Particle &p = src[0];
		// save old positions
		if(iteration == 0) {
			p.old_position = p.position;
		}
		//计算速率，位置，旋转，增长，角度，半径和生命周期
		#ifdef USE_SSE
			p.velocity.vec = _mm_add_ps(_mm_mul_ps(p.velocity.vec,linear_scale_vec),velocity.vec);
			p.position.vec = _mm_add_ps(_mm_mul_ps(p.velocity.vec,ifps_vec),p.position.vec);
		#elif USE_ALTIVEC
			p.velocity.vec = vec_madd(p.velocity.vec,linear_scale_vec,velocity.vec);
			p.position.vec = vec_madd(p.velocity.vec,ifps_vec,p.position.vec);
		#elif USE_NEON
			p.velocity.vec = vmlaq_f32(velocity.vec,p.velocity.vec,linear_scale_vec);
			p.position.vec = vmlaq_f32(p.position.vec,p.velocity.vec,ifps_vec);
		#else
			mad(p.velocity,p.velocity,linear_scale,velocity);
			mad(p.position,p.velocity,ifps,p.position);
		#endif
		p.rotation *= angular_scale;
		p.angle += p.rotation * ifps;
        p.growth *= growth_scale;
		p.radius += p.growth * ifps;
		p.life -= ifps;
        //移除粒子
        if(p.radius < EPSILON || p.life < EPSILON) {
            remove.append((s32)(&p - m_particles.get()));
        }
		src += 1;
		num_particles -= 1;
	}
    // 移除粒子
    if(type == TYPE_CHAIN) {
        for(s32 i = remove.size() - 1; i >= 0; i--) {
            m_particles.remove(remove[i]);
        }
    } else {
        for(s32 i = remove.size() - 1; i >= 0; i--) {
            m_particles.removeFast(remove[i]);
        }
    }
}

//更新粒子包围盒
void SVParticles::update_particles_bounds() {
	if(m_particles.size()) {
		FVec3 min = FVec3_infinity;
		FVec3 max = -FVec3_infinity;
		s32 num_particles = m_particles.size();
        Particle *src = m_particles.get();
		///////////// length particles bounds ///////////////////
		if(type == TYPE_LENGTH) {
			#ifdef USE_SSE
				__m128 length_stretch_vec = _mm_set1_ps(length_stretch);
			#elif USE_ALTIVEC
				vec_float4 length_stretch_vec = vec_splats(length_stretch);
			#elif USE_NEON
				float32x4_t length_stretch_vec = vdupq_n_f32(length_stretch);
			#else
				FVec3 stretch_0,stretch_1,stretch_2,stretch_3;
				FVec3 position_0,position_1,position_2,position_3;
			#endif
			
            //遍历粒子(4个，4个处理)
			while(num_particles >= 4) {
				const Particle &p0 = src[0];
				const Particle &p1 = src[1];
				const Particle &p2 = src[2];
				const Particle &p3 = src[3];
				
				#ifdef USE_SSE
					min.vec = _mm_min_ps(min.vec,p0.position.vec);
					min.vec = _mm_min_ps(min.vec,p1.position.vec);
					min.vec = _mm_min_ps(min.vec,p2.position.vec);
					min.vec = _mm_min_ps(min.vec,p3.position.vec);
					
					max.vec = _mm_max_ps(max.vec,p0.position.vec);
					max.vec = _mm_max_ps(max.vec,p1.position.vec);
					max.vec = _mm_max_ps(max.vec,p2.position.vec);
					max.vec = _mm_max_ps(max.vec,p3.position.vec);
					
					__m128 stretch_0 = _mm_sub_ps(p0.velocity.vec,p0.parameters.vec);
					__m128 stretch_1 = _mm_sub_ps(p1.velocity.vec,p1.parameters.vec);
					__m128 stretch_2 = _mm_sub_ps(p2.velocity.vec,p2.parameters.vec);
					__m128 stretch_3 = _mm_sub_ps(p3.velocity.vec,p3.parameters.vec);
					
					__m128 position_0 = _mm_add_ps(_mm_mul_ps(stretch_0,length_stretch_vec),p0.position.vec);
					__m128 position_1 = _mm_add_ps(_mm_mul_ps(stretch_1,length_stretch_vec),p1.position.vec);
					__m128 position_2 = _mm_add_ps(_mm_mul_ps(stretch_2,length_stretch_vec),p2.position.vec);
					__m128 position_3 = _mm_add_ps(_mm_mul_ps(stretch_3,length_stretch_vec),p3.position.vec);
					
					min.vec = _mm_min_ps(min.vec,position_0);
					min.vec = _mm_min_ps(min.vec,position_1);
					min.vec = _mm_min_ps(min.vec,position_2);
					min.vec = _mm_min_ps(min.vec,position_3);
					
					max.vec = _mm_max_ps(max.vec,position_0);
					max.vec = _mm_max_ps(max.vec,position_1);
					max.vec = _mm_max_ps(max.vec,position_2);
					max.vec = _mm_max_ps(max.vec,position_3);
				#elif USE_ALTIVEC
					min.vec = vec_min(min.vec,p0.position.vec);
					min.vec = vec_min(min.vec,p1.position.vec);
					min.vec = vec_min(min.vec,p2.position.vec);
					min.vec = vec_min(min.vec,p3.position.vec);
					
					max.vec = vec_max(max.vec,p0.position.vec);
					max.vec = vec_max(max.vec,p1.position.vec);
					max.vec = vec_max(max.vec,p2.position.vec);
					max.vec = vec_max(max.vec,p3.position.vec);
					
					vec_float4 stretch_0 = vec_sub(p0.velocity.vec,p0.parameters.vec);
					vec_float4 stretch_1 = vec_sub(p1.velocity.vec,p1.parameters.vec);
					vec_float4 stretch_2 = vec_sub(p2.velocity.vec,p2.parameters.vec);
					vec_float4 stretch_3 = vec_sub(p3.velocity.vec,p3.parameters.vec);
					
					vec_float4 position_0 = vec_madd(stretch_0,length_stretch_vec,p0.position.vec);
					vec_float4 position_1 = vec_madd(stretch_1,length_stretch_vec,p1.position.vec);
					vec_float4 position_2 = vec_madd(stretch_2,length_stretch_vec,p2.position.vec);
					vec_float4 position_3 = vec_madd(stretch_3,length_stretch_vec,p3.position.vec);
					
					min.vec = vec_min(min.vec,position_0);
					min.vec = vec_min(min.vec,position_1);
					min.vec = vec_min(min.vec,position_2);
					min.vec = vec_min(min.vec,position_3);
					
					max.vec = vec_max(max.vec,position_0);
					max.vec = vec_max(max.vec,position_1);
					max.vec = vec_max(max.vec,position_2);
					max.vec = vec_max(max.vec,position_3);
				#elif USE_NEON
					min.vec = vminq_f32(min.vec,p0.position.vec);
					min.vec = vminq_f32(min.vec,p1.position.vec);
					min.vec = vminq_f32(min.vec,p2.position.vec);
					min.vec = vminq_f32(min.vec,p3.position.vec);
					
					max.vec = vmaxq_f32(max.vec,p0.position.vec);
					max.vec = vmaxq_f32(max.vec,p1.position.vec);
					max.vec = vmaxq_f32(max.vec,p2.position.vec);
					max.vec = vmaxq_f32(max.vec,p3.position.vec);
					
					float32x4_t stretch_0 = vsubq_f32(p0.velocity.vec,p0.parameters.vec);
					float32x4_t stretch_1 = vsubq_f32(p1.velocity.vec,p1.parameters.vec);
					float32x4_t stretch_2 = vsubq_f32(p2.velocity.vec,p2.parameters.vec);
					float32x4_t stretch_3 = vsubq_f32(p3.velocity.vec,p3.parameters.vec);
					
					float32x4_t position_0 = vmlaq_f32(p0.position.vec,stretch_0,length_stretch_vec);
					float32x4_t position_1 = vmlaq_f32(p1.position.vec,stretch_1,length_stretch_vec);
					float32x4_t position_2 = vmlaq_f32(p2.position.vec,stretch_2,length_stretch_vec);
					float32x4_t position_3 = vmlaq_f32(p3.position.vec,stretch_3,length_stretch_vec);
					
					min.vec = vminq_f32(min.vec,position_0);
					min.vec = vminq_f32(min.vec,position_1);
					min.vec = vminq_f32(min.vec,position_2);
					min.vec = vminq_f32(min.vec,position_3);
					
					max.vec = vmaxq_f32(max.vec,position_0);
					max.vec = vmaxq_f32(max.vec,position_1);
					max.vec = vmaxq_f32(max.vec,position_2);
					max.vec = vmaxq_f32(max.vec,position_3);
				#else
					min = ::min(min,p0.position);
					min = ::min(min,p1.position);
					min = ::min(min,p2.position);
					min = ::min(min,p3.position);
					
					max = ::max(max,p0.position);
					max = ::max(max,p1.position);
					max = ::max(max,p2.position);
					max = ::max(max,p3.position);
					
					sub(stretch_0,p0.velocity,p0.parameters);
					sub(stretch_1,p1.velocity,p1.parameters);
					sub(stretch_2,p2.velocity,p2.parameters);
					sub(stretch_3,p3.velocity,p3.parameters);
					
					mad(position_0,stretch_0,length_stretch,p0.position);
					mad(position_1,stretch_1,length_stretch,p1.position);
					mad(position_2,stretch_2,length_stretch,p2.position);
					mad(position_3,stretch_3,length_stretch,p3.position);
					
					min = ::min(min,position_0);
					min = ::min(min,position_1);
					min = ::min(min,position_2);
					min = ::min(min,position_3);
					
					max = ::max(max,position_0);
					max = ::max(max,position_1);
					max = ::max(max,position_2);
					max = ::max(max,position_3);
				#endif
				
				src += 4;
				num_particles -= 4;
			}
			
            //处理剩余的小于4个例子 单个处理
			while(num_particles >= 1) {
				const Particle &p = src[0];
				
				#ifdef USE_SSE
					min.vec = _mm_min_ps(min.vec,p.position.vec);
					max.vec = _mm_max_ps(max.vec,p.position.vec);
					__m128 stretch = _mm_sub_ps(p.velocity.vec,p.parameters.vec);
					__m128 position = _mm_add_ps(_mm_mul_ps(stretch,length_stretch_vec),p.position.vec);
					min.vec = _mm_min_ps(min.vec,position);
					max.vec = _mm_max_ps(max.vec,position);
				#elif USE_ALTIVEC
					min.vec = vec_min(min.vec,p.position.vec);
					max.vec = vec_max(max.vec,p.position.vec);
					vec_float4 stretch = vec_sub(p.velocity.vec,p.parameters.vec);
					vec_float4 position = vec_madd(stretch,length_stretch_vec,p.position.vec);
					min.vec = vec_min(min.vec,position);
					max.vec = vec_max(max.vec,position);
				#elif USE_NEON
					min.vec = vminq_f32(min.vec,p.position.vec);
					max.vec = vmaxq_f32(max.vec,p.position.vec);
					float32x4_t stretch = vsubq_f32(p.velocity.vec,p.parameters.vec);
					float32x4_t position = vmlaq_f32(p.position.vec,stretch,length_stretch_vec);
					min.vec = vminq_f32(min.vec,position);
					max.vec = vmaxq_f32(max.vec,position);
				#else
					min = ::min(min,p.position);
					max = ::max(max,p.position);
					sub(stretch_0,p.velocity,p.parameters);
					mad(position_0,stretch_0,length_stretch,p.position);
					min = ::min(min,position_0);
					max = ::max(max,position_0);
				#endif
				
				src += 1;
				num_particles -= 1;
			}
		} else {
            // particles bounds
			Simd::minMaxVec3(min,
                             max,m_particles[0].position,
                             sizeof(m_particles),
                             m_particles.size());
		}
		// set particles bounds
		particles_bound_box.set(min,max);
	}
}

//更新发射器包围
void SVParticles::update_bounds() {
	// life time
	f32 life = life_mean + life_spread;
	// emitter size
	FVec3 min = emitter_transform.getColumn3(3);
	FVec3 max = emitter_transform.getColumn3(3);
	f32 radius = length(emitter_size) * 0.5f;
	min -= FVec3(radius);
	max += FVec3(radius);
	// particles bounds
	if( m_particles.size()>0 ) {
		min = ::min(min,particles_bound_box.getMin());
		max = ::max(max,particles_bound_box.getMax());
	} else {
		SVBoundBox emitter_bounds;
		FMat3 emitter_rotation = FMat3(emitter_transform);
		f32 velocity = (velocity_mean + velocity_spread) * life;
		emitter_bounds.expand(emitter_rotation * normalize(emitter_direction + FVec3( emitter_spread.x, emitter_spread.y, emitter_spread.z)));
		emitter_bounds.expand(emitter_rotation * normalize(emitter_direction + FVec3(-emitter_spread.x, emitter_spread.y, emitter_spread.z)));
		emitter_bounds.expand(emitter_rotation * normalize(emitter_direction + FVec3( emitter_spread.x,-emitter_spread.y, emitter_spread.z)));
		emitter_bounds.expand(emitter_rotation * normalize(emitter_direction + FVec3(-emitter_spread.x,-emitter_spread.y, emitter_spread.z)));
		emitter_bounds.expand(emitter_rotation * normalize(emitter_direction + FVec3( emitter_spread.x, emitter_spread.y,-emitter_spread.z)));
		emitter_bounds.expand(emitter_rotation * normalize(emitter_direction + FVec3(-emitter_spread.x, emitter_spread.y,-emitter_spread.z)));
		emitter_bounds.expand(emitter_rotation * normalize(emitter_direction + FVec3( emitter_spread.x,-emitter_spread.y,-emitter_spread.z)));
		emitter_bounds.expand(emitter_rotation * normalize(emitter_direction + FVec3(-emitter_spread.x,-emitter_spread.y,-emitter_spread.z)));
		min = ::min(min,min + emitter_bounds.getMin() * velocity);
		max = ::max(max,max + emitter_bounds.getMax() * velocity);
		// maximum particle radius
		radius = radius_mean + radius_spread + ::max(growth_mean + growth_spread,0.0f) * life;
		min -= FVec3(radius);
		max += FVec3(radius);
		// maximum gravity shift
		FVec3 gravity_shift = gravity * (life * life * 0.5f);
		min += ::min(gravity_shift,FVec3_zero);
		max += ::max(gravity_shift,FVec3_zero);
	}
    
	// spark points 火花(火舌)
	for(s32 i = 0; i < sparks.size(); i++) {
		const Spark &s = sparks[i];
		min = ::min(min,s.point);
		max = ::max(max,s.point);
	}
	// 最大粒子半径 （半径值+生长值+速率）
	radius = radius_mean + radius_spread + ::max(growth_mean + growth_spread,0.0f) * life + (velocity_mean + velocity_spread) * PARTICLES_IFPS * 2.0f;
	min -= FVec3(radius);
	max += FVec3(radius);
	//设置包围盒
	bound_box.set(min,max);
	bound_sphere.set(bound_box);
}

//******************************** Render **********************************************

static const u16 orientations[28][4] = {
    //65280 0xff00
    //65535 0xffff
    //255   0x00ff
    //0     0x0000
	// two axis variation(变量)
	{ 65280, 65535, 255,   0     },
	{ 0,     65280, 65535, 255,  },
	{ 255,   0,     65280, 65535 },
	{ 65535, 255,   0,     65280 },
	{ 65280, 0,     255,   65535 },
	{ 0,     255,   65535, 65280 },
	{ 255,   65535, 65280, 0     },
	{ 65535, 65280, 0,     255   },
	
	// x axis variation
	{ 65280, 65535, 255,   0     },
	{ 65535, 65280, 0,     255   },
	
	// y axis variation
	{ 65280, 65535, 255,   0     },
	{ 0,     255,   65535, 65280 },
	
	// texture atlas(16个)
    //16128 0xff00
    //16191 0x3f3f
    //63    0x003f
    // ...... 等等
	{ 16128, 16191, 63,    0     },
	{ 32512, 32575, 16447, 16384 },
	{ 48896, 48959, 32831, 32768 },
	{ 65280, 65343, 49215, 49152 },
	{ 16192, 16255, 127,   64    },
	{ 32576, 32639, 16511, 16448 },
	{ 48960, 49023, 32895, 32832 },
	{ 65344, 65407, 49279, 49216 },
	{ 16256, 16319, 191,   128   },
	{ 32640, 32703, 16575, 16512 },
	{ 49024, 49087, 32959, 32896 },
	{ 65408, 65471, 49343, 49280 },
	{ 16320, 16383, 255,   192   },
	{ 32704, 32767, 16639, 16576 },
	{ 49088, 49151, 33023, 32960 },
	{ 65472, 65535, 49407, 49344 },
};

//
struct ParticlesDistanceCompare {
	inline s32 operator()(const Distance &d0,const Distance &d1) const {
		return (d0.distance > d1.distance);
	}
};

void SVParticles::sort_particles(const FVec3 &camera) {
    distances.resize(m_particles.size());
    FVec3 t_direction;
    s32 num_particles = m_particles.size();
    for(s32 i = 0; i < num_particles; i++) {
        Distance &t_d = distances[i];
        sub(t_direction,m_particles[i].position,camera);
        t_d.distance = t_direction.length2();
        t_d.index = i;
    }
    ParticlesDistanceCompare compare;
    sv::util::quickSort(distances.begin(),distances.end(),compare);
}

//
void SVParticles::create_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera) {
	switch(type) {
        case TYPE_BILLBOARD: {
            create_billboard_particles(vertex,modelview,camera);
            break;
        }
        case TYPE_FLAT:{
            create_oriented_particles(vertex,modelview,camera);
            break;
        }
        case TYPE_POINT:{
            create_point_particles(vertex,modelview,camera);
            break;
        }
        case TYPE_LENGTH:{
            create_length_particles(vertex,modelview,camera);
            break;
        }
        case TYPE_RANDOM:{
            create_oriented_particles(vertex,modelview,camera);
            break;
        }
        case TYPE_ROUTE:{
            create_route_particles(vertex,modelview,camera);
            break;
        }
        case TYPE_CHAIN:{
            create_chain_particles(vertex,modelview,camera);
            break;
        }
        default:{
            assert(0 && "SVParticles::create_particles(): unknown particles type");
        }
	}
}

//
void SVParticles::create_billboard_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera) {
    assert(type == TYPE_BILLBOARD && "SVParticles::create_billboard_particles(): bad particles type");
    FVec3 dxc,dxs,dyc,dys,temp;
    FVec3 dx = modelview.getRow3(0) * SQRT2;
    FVec3 dy = modelview.getRow3(1) * SQRT2;
    FVec3 dz = modelview.getRow3(2);
    if(dot(cross(dx,dy),dz) < 0.0f) {
        dy = -dy;
    }
    //
    V3_PARTICLE *v = vertex;
    s32 num_particles = m_particles.size();
    if(depth_sort) {
        sort_particles(camera);
    }
    for(s32 i = 0; i < num_particles; i++) {
        const Particle &p = (depth_sort) ? m_particles[distances[i].index] : m_particles[i];
        f32 s,c;
        Math::sincosFast(p.angle,s,c);
        s *= p.radius;
        c *= p.radius;
#ifdef USE_SSE
        __m128 c_vec = _mm_set1_ps(c);
        __m128 s_vec = _mm_set1_ps(s);
        dxc.vec = _mm_mul_ps(dx.vec,c_vec);
        dxs.vec = _mm_mul_ps(dx.vec,s_vec);
        dyc.vec = _mm_mul_ps(dy.vec,c_vec);
        dys.vec = _mm_mul_ps(dy.vec,s_vec);
#elif USE_ALTIVEC
        vec_float4 c_vec = vec_splats(c);
        vec_float4 s_vec = vec_splats(s);
        vec_float4 zero = vec_splats(0.0f);
        dxc.vec = vec_madd(dx.vec,c_vec,zero);
        dxs.vec = vec_madd(dx.vec,s_vec,zero);
        dyc.vec = vec_madd(dy.vec,c_vec,zero);
        dys.vec = vec_madd(dy.vec,s_vec,zero);
#elif USE_NEON
        float32x4_t c_vec = vdupq_n_f32(c);
        float32x4_t s_vec = vdupq_n_f32(s);
        dxc.vec = vmulq_f32(dx.vec,c_vec);
        dxs.vec = vmulq_f32(dx.vec,s_vec);
        dyc.vec = vmulq_f32(dy.vec,c_vec);
        dys.vec = vmulq_f32(dy.vec,s_vec);
#else
        mul(dxc,dx,c);
        mul(dxs,dx,s);
        mul(dyc,dy,c);
        mul(dys,dy,s);
#endif
        //
        const u16 *orientation = orientations[p.orientation];
        u32 color = (65535 - Math::round(p.life * p.ilife)) << 16;
        // v[0].xyz = p.position - dxc + dys
        // v[1].xyz = p.position - dxs - dyc
        // v[2].xyz = p.position + dxc - dys
        // v[3].xyz = p.position + dxs + dyc
#ifdef USE_SSE
        v[0].vec = _mm_sub_ps(p.position.vec,_mm_sub_ps(dxc.vec,dys.vec));
        v[0].parameters = color | orientation[0];
        v[1].vec = _mm_sub_ps(p.position.vec,_mm_add_ps(dxs.vec,dyc.vec));
        v[1].parameters = color | orientation[1];
        v[2].vec = _mm_add_ps(p.position.vec,_mm_sub_ps(dxc.vec,dys.vec));
        v[2].parameters = color | orientation[2];
        v[3].vec = _mm_add_ps(p.position.vec,_mm_add_ps(dxs.vec,dyc.vec));
        v[3].parameters = color | orientation[3];
#elif USE_ALTIVEC
        v[0].vec = vec_sub(p.position.vec,vec_sub(dxc.vec,dys.vec));
        v[0].parameters = color | orientation[0];
        v[1].vec = vec_sub(p.position.vec,vec_add(dxs.vec,dyc.vec));
        v[1].parameters = color | orientation[1];
        v[2].vec = vec_add(p.position.vec,vec_sub(dxc.vec,dys.vec));
        v[2].parameters = color | orientation[2];
        v[3].vec = vec_add(p.position.vec,vec_add(dxs.vec,dyc.vec));
        v[3].parameters = color | orientation[3];
#elif USE_NEON
        v[0].vec = vsubq_f32(p.position.vec,vsubq_f32(dxc.vec,dys.vec));
        v[0].parameters = color | orientation[0];
        v[1].vec = vsubq_f32(p.position.vec,vaddq_f32(dxs.vec,dyc.vec));
        v[1].parameters = color | orientation[1];
        v[2].vec = vaddq_f32(p.position.vec,vsubq_f32(dxc.vec,dys.vec));
        v[2].parameters = color | orientation[2];
        v[3].vec = vaddq_f32(p.position.vec,vaddq_f32(dxs.vec,dyc.vec));
        v[3].parameters = color | orientation[3];
#else
        sub3(v[0].xyz,p.position,sub(temp,dxc,dys));
        v[0].parameters = color | orientation[0];
        v[0].rgb[0] = p.color.x;
        v[0].rgb[1] = p.color.y;
        v[0].rgb[2] = p.color.z;
        sub3(v[1].xyz,p.position,add(temp,dxs,dyc));
        v[1].parameters = color | orientation[1];
        v[1].rgb[0] = p.color.x;
        v[1].rgb[1] = p.color.y;
        v[1].rgb[2] = p.color.z;
        add3(v[2].xyz,p.position,sub(temp,dxc,dys));
        v[2].parameters = color | orientation[2];
        v[2].rgb[0] = p.color.x;
        v[2].rgb[1] = p.color.y;
        v[2].rgb[2] = p.color.z;
        add3(v[3].xyz,p.position,add(temp,dxs,dyc));
        v[3].parameters = color | orientation[3];
        v[3].rgb[0] = p.color.x;
        v[3].rgb[1] = p.color.y;
        v[3].rgb[2] = p.color.z;
#endif
        v += 4;
    }
}

//
void SVParticles::create_oriented_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera) {
	assert( (type == TYPE_FLAT || type == TYPE_RANDOM) );
	FVec3 dx,dy;
	FVec3 dxc,dxs,dyc,dys,temp;
	V3_PARTICLE *v = vertex;
	s32 num_particles = m_particles.size();
    if(depth_sort) {
        //根据相机位置进行深度排序
        sort_particles(camera);
    }
	for(s32 i = 0; i < num_particles; i++) {
		const Particle &p = (depth_sort) ? m_particles[distances[i].index] : m_particles[i];
        //根据角度计算x,y
		f32 s,c;
		Math::sincosFast(p.angle,s,c);
		s *= p.radius * SQRT2;
		c *= p.radius * SQRT2;
        //计算切线和副法线
		orthoBasis(p.parameters,dx,dy);
		dx = FVec3(dx.v);
		dy = FVec3(dy.v);
		//分离出切线(cos,sin) 副法线(cos,sin) 并根据半径扩展他们的长度
		#ifdef USE_SSE
			__m128 c_vec = _mm_set1_ps(c);
			__m128 s_vec = _mm_set1_ps(s);
			dxc.vec = _mm_mul_ps(dx.vec,c_vec);
			dxs.vec = _mm_mul_ps(dx.vec,s_vec);
			dyc.vec = _mm_mul_ps(dy.vec,c_vec);
			dys.vec = _mm_mul_ps(dy.vec,s_vec);
		#elif USE_ALTIVEC
			vec_float4 c_vec = vec_splats(c);
			vec_float4 s_vec = vec_splats(s);
			vec_float4 zero = vec_splats(0.0f);
			dxc.vec = vec_madd(dx.vec,c_vec,zero);
			dxs.vec = vec_madd(dx.vec,s_vec,zero);
			dyc.vec = vec_madd(dy.vec,c_vec,zero);
			dys.vec = vec_madd(dy.vec,s_vec,zero);
		#elif USE_NEON
			float32x4_t c_vec = vdupq_n_f32(c);
			float32x4_t s_vec = vdupq_n_f32(s);
			dxc.vec = vmulq_f32(dx.vec,c_vec);
			dxs.vec = vmulq_f32(dx.vec,s_vec);
			dyc.vec = vmulq_f32(dy.vec,c_vec);
			dys.vec = vmulq_f32(dy.vec,s_vec);
		#else
			mul(dxc,dx,c);
			mul(dxs,dx,s);
			mul(dyc,dy,c);
			mul(dys,dy,s);
		#endif
		//从查找表中 查到orient(低16代表朝向)
		const u16 *orientation = orientations[p.orientation];
        //根据生命 计算颜色（高16代表颜色）
		u32 color = (65535 - Math::round(p.life * p.ilife)) << 16;
		//计算4个顶点
        /**
        v[0].xyz = p.position - dxc + dys
		v[1].xyz = p.position - dxs - dyc
        v[2].xyz = p.position + dxc - dys
		v[3].xyz = p.position + dxs + dyc
        **/
		#ifdef USE_SSE
			v[0].vec = _mm_sub_ps(p.position.vec,_mm_sub_ps(dxc.vec,dys.vec));
			v[0].parameters = color | orientation[0];
			v[1].vec = _mm_sub_ps(p.position.vec,_mm_add_ps(dxs.vec,dyc.vec));
			v[1].parameters = color | orientation[1];
			v[2].vec = _mm_add_ps(p.position.vec,_mm_sub_ps(dxc.vec,dys.vec));
			v[2].parameters = color | orientation[2];
			v[3].vec = _mm_add_ps(p.position.vec,_mm_add_ps(dxs.vec,dyc.vec));
			v[3].parameters = color | orientation[3];
		#elif USE_ALTIVEC
			v[0].vec = vec_sub(p.position.vec,vec_sub(dxc.vec,dys.vec));
			v[0].parameters = color | orientation[0];
			v[1].vec = vec_sub(p.position.vec,vec_add(dxs.vec,dyc.vec));
			v[1].parameters = color | orientation[1];
			v[2].vec = vec_add(p.position.vec,vec_sub(dxc.vec,dys.vec));
			v[2].parameters = color | orientation[2];
			v[3].vec = vec_add(p.position.vec,vec_add(dxs.vec,dyc.vec));
			v[3].parameters = color | orientation[3];
		#elif USE_NEON
			v[0].vec = vsubq_f32(p.position.vec,vsubq_f32(dxc.vec,dys.vec));
			v[0].parameters = color | orientation[0];
			v[1].vec = vsubq_f32(p.position.vec,vaddq_f32(dxs.vec,dyc.vec));
			v[1].parameters = color | orientation[1];
			v[2].vec = vaddq_f32(p.position.vec,vsubq_f32(dxc.vec,dys.vec));
			v[2].parameters = color | orientation[2];
			v[3].vec = vaddq_f32(p.position.vec,vaddq_f32(dxs.vec,dyc.vec));
			v[3].parameters = color | orientation[3];
		#else
			sub3(v[0].xyz,p.position,sub(temp,dxc,dys));
			v[0].parameters = color | orientation[0];
			sub3(v[1].xyz,p.position,add(temp,dxs,dyc));
			v[1].parameters = color | orientation[1];
			add3(v[2].xyz,p.position,sub(temp,dxc,dys));
			v[2].parameters = color | orientation[2];
			add3(v[3].xyz,p.position,add(temp,dxs,dyc));
			v[3].parameters = color | orientation[3];
		#endif
		v += 4;
	}
}

//
void SVParticles::create_point_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera) {
	assert(type == TYPE_POINT);
	FVec3 dx,dy,axy,sxy;
	FVec3 x = modelview.getRow3(0);
	FVec3 y = modelview.getRow3(1);
	FVec3 z = modelview.getRow3(2);
    if(dot(cross(x,y),z) < 0.0f) {
        y = -y;
    }
	V3_PARTICLE *v = vertex;
	s32 num_particles = m_particles.size();
    if(depth_sort){
        sort_particles(camera);
    }
	for(s32 i = 0; i < num_particles; i++) {
		const Particle &p = (depth_sort) ? m_particles[distances[i].index] : m_particles[i];
		
		#ifdef USE_SSE
			__m128 r_vec = _mm_set1_ps(p.radius);
			dx.vec = _mm_mul_ps(x.vec,r_vec);
			dy.vec = _mm_mul_ps(y.vec,r_vec);
			axy.vec = _mm_add_ps(dx.vec,dy.vec);
			sxy.vec = _mm_sub_ps(dx.vec,dy.vec);
		#elif USE_ALTIVEC
			vec_float4 zero = vec_splats(0.0f);
			vec_float4 r_vec = vec_splats(p.radius);
			dx.vec = vec_madd(x.vec,r_vec,zero);
			dy.vec = vec_madd(y.vec,r_vec,zero);
			axy.vec = vec_add(dx.vec,dy.vec);
			sxy.vec = vec_sub(dx.vec,dy.vec);
		#elif USE_NEON
			float32x4_t r_vec = vdupq_n_f32(p.radius);
			dx.vec = vmulq_f32(x.vec,r_vec);
			dy.vec = vmulq_f32(y.vec,r_vec);
			axy.vec = vaddq_f32(dx.vec,dy.vec);
			sxy.vec = vsubq_f32(dx.vec,dy.vec);
		#else
			mul(dx,x,p.radius);
			mul(dy,y,p.radius);
			add(axy,dx,dy);
			sub(sxy,dx,dy);
		#endif
		
		const u16 *orientation = orientations[p.orientation];
		u32 color = (65535 - Math::round(p.life * p.ilife)) << 16;
		// v[0].xyz = p.position - dx - dy
		// v[1].xyz = p.position + dx - dy
		// v[2].xyz = p.position + dx + dy
		// v[3].xyz = p.position - dx + dy
		#ifdef USE_SSE
			v[0].vec = _mm_sub_ps(p.position.vec,axy.vec);
			v[0].parameters = color | orientation[0];
			v[1].vec = _mm_add_ps(p.position.vec,sxy.vec);
			v[1].parameters = color | orientation[1];
			v[2].vec = _mm_add_ps(p.position.vec,axy.vec);
			v[2].parameters = color | orientation[2];
			v[3].vec = _mm_sub_ps(p.position.vec,sxy.vec);
			v[3].parameters = color | orientation[3];
		#elif USE_ALTIVEC
			v[0].vec = vec_sub(p.position.vec,axy.vec);
			v[0].parameters = color | orientation[0];
			v[1].vec = vec_add(p.position.vec,sxy.vec);
			v[1].parameters = color | orientation[1];
			v[2].vec = vec_add(p.position.vec,axy.vec);
			v[2].parameters = color | orientation[2];
			v[3].vec = vec_sub(p.position.vec,sxy.vec);
			v[3].parameters = color | orientation[3];
		#elif USE_NEON
			v[0].vec = vsubq_f32(p.position.vec,axy.vec);
			v[0].parameters = color | orientation[0];
			v[1].vec = vaddq_f32(p.position.vec,sxy.vec);
			v[1].parameters = color | orientation[1];
			v[2].vec = vaddq_f32(p.position.vec,axy.vec);
			v[2].parameters = color | orientation[2];
			v[3].vec = vsubq_f32(p.position.vec,sxy.vec);
			v[3].parameters = color | orientation[3];
		#else
			sub3(v[0].xyz,p.position,axy);
			v[0].parameters = color | orientation[0];
			add3(v[1].xyz,p.position,sxy);
			v[1].parameters = color | orientation[1];
			add3(v[2].xyz,p.position,axy);
			v[2].parameters = color | orientation[2];
			sub3(v[3].xyz,p.position,sxy);
			v[3].parameters = color | orientation[3];
		#endif
		
		v += 4;
	}
}

//
void SVParticles::create_length_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera) {
	assert(type == TYPE_LENGTH );
	
	FVec3 direction,stretch;
	FVec3 dx,dy,axy,sxy,temp;
	
	V3_PARTICLE *v = vertex;
	s32 num_particles = m_particles.size();
    if(depth_sort){
        sort_particles(camera);
    }
	for(s32 i = 0; i < num_particles; i++) {
		const Particle &p = (depth_sort) ? m_particles[distances[i].index] : m_particles[i];
		
		#ifdef USE_SSE
			__m128 r_vec = _mm_set1_ps(p.radius);
			direction.vec = _mm_sub_ps(camera.vec,p.position.vec);
			stretch.vec = _mm_sub_ps(p.velocity.vec,p.parameters.vec);
			dx.vec = _mm_cross_ps(stretch.vec,direction.vec);
			dy.vec = _mm_cross_ps(direction.vec,dx.vec);
			dx.vec = _mm_mul_ps(_mm_normalize3_ps(dx.vec),r_vec);
			dy.vec = _mm_mul_ps(_mm_normalize3_ps(dy.vec),r_vec);
			axy.vec = _mm_add_ps(dx.vec,dy.vec);
			sxy.vec = _mm_sub_ps(dx.vec,dy.vec);
			stretch.vec = _mm_mul_ps(stretch.vec,_mm_set1_ps(length_stretch));
		#elif USE_ALTIVEC
			vec_float4 zero = vec_splats(0.0f);
			vec_float4 r_vec = vec_splats(p.radius);
			direction.vec = vec_sub(camera.vec,p.position.vec);
			stretch.vec = vec_sub(p.velocity.vec,p.parameters.vec);
			dx.vec = vec_cross(stretch.vec,direction.vec);
			dy.vec = vec_cross(direction.vec,dx.vec);
			dx.vec = vec_madd(vec_normalize3(dx.vec),r_vec,zero);
			dy.vec = vec_madd(vec_normalize3(dy.vec),r_vec,zero);
			axy.vec = vec_add(dx.vec,dy.vec);
			sxy.vec = vec_sub(dx.vec,dy.vec);
			stretch.vec = vec_madd(stretch.vec,vec_splats(length_stretch),zero);
		#elif USE_NEON
			float32x4_t r_vec = vdupq_n_f32(p.radius);
			direction.vec = vsubq_f32(camera.vec,p.position.vec);
			stretch.vec = vsubq_f32(p.velocity.vec,p.parameters.vec);
			dx.vec = vcrossq_f32(stretch.vec,direction.vec);
			dy.vec = vcrossq_f32(direction.vec,dx.vec);
			dx.vec = vmulq_f32(vnormalize3q_f32(dx.vec),r_vec);
			dy.vec = vmulq_f32(vnormalize3q_f32(dy.vec),r_vec);
			axy.vec = vaddq_f32(dx.vec,dy.vec);
			sxy.vec = vsubq_f32(dx.vec,dy.vec);
			stretch.vec = vmulq_f32(stretch.vec,vdupq_n_f32(length_stretch));
		#else
			sub(direction,camera,p.position);
			sub(stretch,p.velocity,p.parameters);
			cross(dx,stretch,direction);
			cross(dy,direction,dx);
			mul(dx,dx,Math::rsqrtFast(length2(dx)) * p.radius);
			mul(dy,dy,Math::rsqrtFast(length2(dy)) * p.radius);
			add(axy,dx,dy);
			sub(sxy,dx,dy);
			mul(stretch,stretch,length_stretch);
		#endif
		
		const u16 *orientation = orientations[p.orientation];
		u32 color = (65535 - Math::round(p.life * p.ilife)) << 16;
		
		// v[0].xyz = p.position - dx - dy
		// v[1].xyz = p.position + dx - dy
		// v[2].xyz = p.position + dx + dy + stretch
		// v[3].xyz = p.position - dx + dy + stretch
		#ifdef USE_SSE
			v[0].vec = _mm_sub_ps(p.position.vec,axy.vec);
			v[0].parameters = color | orientation[0];
			v[1].vec = _mm_add_ps(p.position.vec,sxy.vec);
			v[1].parameters = color | orientation[1];
			v[2].vec = _mm_add_ps(_mm_add_ps(p.position.vec,axy.vec),stretch.vec);
			v[2].parameters = color | orientation[2];
			v[3].vec = _mm_add_ps(_mm_sub_ps(p.position.vec,sxy.vec),stretch.vec);
			v[3].parameters = color | orientation[3];
		#elif USE_ALTIVEC
			v[0].vec = vec_sub(p.position.vec,axy.vec);
			v[0].parameters = color | orientation[0];
			v[1].vec = vec_add(p.position.vec,sxy.vec);
			v[1].parameters = color | orientation[1];
			v[2].vec = vec_add(vec_add(p.position.vec,axy.vec),stretch.vec);
			v[2].parameters = color | orientation[2];
			v[3].vec = vec_add(vec_sub(p.position.vec,sxy.vec),stretch.vec);
			v[3].parameters = color | orientation[3];
		#elif USE_NEON
			v[0].vec = vsubq_f32(p.position.vec,axy.vec);
			v[0].parameters = color | orientation[0];
			v[1].vec = vaddq_f32(p.position.vec,sxy.vec);
			v[1].parameters = color | orientation[1];
			v[2].vec = vaddq_f32(vaddq_f32(p.position.vec,axy.vec),stretch.vec);
			v[2].parameters = color | orientation[2];
			v[3].vec = vaddq_f32(vsubq_f32(p.position.vec,sxy.vec),stretch.vec);
			v[3].parameters = color | orientation[3];
		#else
			sub3(v[0].xyz,p.position,axy);
			v[0].parameters = color | orientation[0];
			add3(v[1].xyz,p.position,sxy);
			v[1].parameters = color | orientation[1];
			add3(v[2].xyz,add3(temp,p.position,axy),stretch);
			v[2].parameters = color | orientation[2];
			add3(v[3].xyz,sub3(temp,p.position,sxy),stretch);
			v[3].parameters = color | orientation[3];
		#endif
		
		v += 4;
	}
}

//
void SVParticles::create_route_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera){
	assert(type == TYPE_ROUTE);
	FVec3 dx,dy;
	FVec3 dxc,dxs,dyc,dys,temp;
	FVec3 dz = emitter_transform.getColumn3(2);
	
	V3_PARTICLE *v = vertex;
	s32 num_particles = m_particles.size();
    if(depth_sort) {
        sort_particles(camera);
    }
	for(s32 i = 0; i < num_particles; i++) {
		const Particle &p = (depth_sort) ? m_particles[distances[i].index] : m_particles[i];
		f32 s,c;
		Math::sincosFast(p.angle,s,c);
		s *= p.radius;
		c *= p.radius;
		dx = p.parameters;
		#ifdef USE_SSE
			__m128 c_vec = _mm_set1_ps(c);
			__m128 s_vec = _mm_set1_ps(s);
			dy.vec = _mm_cross_ps(dz.vec,dx.vec);
			dy.vec = _mm_mul_ps(_mm_normalize3_ps(dy.vec),_mm_set1_ps(SQRT2));
			dxc.vec = _mm_mul_ps(dx.vec,c_vec);
			dxs.vec = _mm_mul_ps(dx.vec,s_vec);
			dyc.vec = _mm_mul_ps(dy.vec,c_vec);
			dys.vec = _mm_mul_ps(dy.vec,s_vec);
		#elif USE_ALTIVEC
			vec_float4 c_vec = vec_splats(c);
			vec_float4 s_vec = vec_splats(s);
			vec_float4 zero = vec_splats(0.0f);
			dy.vec = vec_cross(dz.vec,dx.vec);
			dy.vec = vec_madd(vec_normalize3(dy.vec),vec_splats(SQRT2),zero);
			dxc.vec = vec_madd(dx.vec,c_vec,zero);
			dxs.vec = vec_madd(dx.vec,s_vec,zero);
			dyc.vec = vec_madd(dy.vec,c_vec,zero);
			dys.vec = vec_madd(dy.vec,s_vec,zero);
		#elif USE_NEON
			float32x4_t c_vec = vdupq_n_f32(c);
			float32x4_t s_vec = vdupq_n_f32(s);
			dy.vec = vcrossq_f32(dz.vec,dx.vec);
			dy.vec = vmulq_f32(vnormalize3q_f32(dy.vec),vdupq_n_f32(SQRT2));
			dxc.vec = vmulq_f32(dx.vec,c_vec);
			dxs.vec = vmulq_f32(dx.vec,s_vec);
			dyc.vec = vmulq_f32(dy.vec,c_vec);
			dys.vec = vmulq_f32(dy.vec,s_vec);
		#else
			cross(dy,dz,dx);
			mul(dy,dy,Math::rsqrtFast(length2(dy)) * SQRT2);
			mul(dxc,dx,c);
			mul(dxs,dx,s);
			mul(dyc,dy,c);
			mul(dys,dy,s);
		#endif
		const u16 *orientation = orientations[p.orientation];
		u32 color = (65535 - Math::round(p.life * p.ilife)) << 16;
		// v[0].xyz = p.position - dxc + dys
		// v[1].xyz = p.position - dxs - dyc
		// v[2].xyz = p.position + dxc - dys
		// v[3].xyz = p.position + dxs + dyc
		#ifdef USE_SSE
			v[0].vec = _mm_sub_ps(p.position.vec,_mm_sub_ps(dxc.vec,dys.vec));
			v[0].parameters = color | orientation[0];
			v[1].vec = _mm_sub_ps(p.position.vec,_mm_add_ps(dxs.vec,dyc.vec));
			v[1].parameters = color | orientation[1];
			v[2].vec = _mm_add_ps(p.position.vec,_mm_sub_ps(dxc.vec,dys.vec));
			v[2].parameters = color | orientation[2];
			v[3].vec = _mm_add_ps(p.position.vec,_mm_add_ps(dxs.vec,dyc.vec));
			v[3].parameters = color | orientation[3];
		#elif USE_ALTIVEC
			v[0].vec = vec_sub(p.position.vec,vec_sub(dxc.vec,dys.vec));
			v[0].parameters = color | orientation[0];
			v[1].vec = vec_sub(p.position.vec,vec_add(dxs.vec,dyc.vec));
			v[1].parameters = color | orientation[1];
			v[2].vec = vec_add(p.position.vec,vec_sub(dxc.vec,dys.vec));
			v[2].parameters = color | orientation[2];
			v[3].vec = vec_add(p.position.vec,vec_add(dxs.vec,dyc.vec));
			v[3].parameters = color | orientation[3];
		#elif USE_NEON
			v[0].vec = vsubq_f32(p.position.vec,vsubq_f32(dxc.vec,dys.vec));
			v[0].parameters = color | orientation[0];
			v[1].vec = vsubq_f32(p.position.vec,vaddq_f32(dxs.vec,dyc.vec));
			v[1].parameters = color | orientation[1];
			v[2].vec = vaddq_f32(p.position.vec,vsubq_f32(dxc.vec,dys.vec));
			v[2].parameters = color | orientation[2];
			v[3].vec = vaddq_f32(p.position.vec,vaddq_f32(dxs.vec,dyc.vec));
			v[3].parameters = color | orientation[3];
		#else
			sub3(v[0].xyz,p.position,sub(temp,dxc,dys));
			v[0].parameters = color | orientation[0];
			sub3(v[1].xyz,p.position,add(temp,dxs,dyc));
			v[1].parameters = color | orientation[1];
			add3(v[2].xyz,p.position,sub(temp,dxc,dys));
			v[2].parameters = color | orientation[2];
			add3(v[3].xyz,p.position,add(temp,dxs,dyc));
			v[3].parameters = color | orientation[3];
		#endif
		
		v += 4;
	}
}

//
void SVParticles::create_chain_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera) {
	assert(type == TYPE_CHAIN && "SVParticles::create_chain_particles(): bad particles type");
	s32 num_particles = m_particles.size();
	if(num_particles < 3) {
        // empty particles
		V3_PARTICLE *v = vertex;
		s32 num_vertex = num_particles * 4;
		for(s32 i = 0; i < num_vertex; i++) {
			v->xyz[0] = 0.0f;
			v->xyz[1] = 0.0f;
			v->xyz[2] = 0.0f;
			v->parameters = 0;
			v++;
		}
	} else {
		// create particles
		FVec3 direction_0;
		FVec3 direction_1;
		FVec3 dx_0,dx_1;
		
		const Particle *p0 = &m_particles[0];
		const Particle *p1 = &m_particles[1];
		
		sub(direction_0,p1->position,p0->position);
		sub(direction_1,camera,p0->position);
		cross(dx_0,direction_1,direction_0);
		
		mul(dx_0,dx_0,Math::rsqrtFast(length2(dx_0)) * p0->radius);
		
		u32 color_0 = (65535 - Math::round(p0->life * p0->ilife)) << 16;
		u32 orientation_0 = 0;
		
		V3_PARTICLE *v = vertex;
		for(s32 i = 2; i < num_particles; i++) {
			const Particle *p2 = &m_particles[i];
			#ifdef USE_SSE
				direction_0.vec = _mm_sub_ps(p2->position.vec,p1->position.vec);
				direction_1.vec = _mm_sub_ps(camera.vec,p1->position.vec);
				dx_1.vec = _mm_cross_ps(direction_1.vec,direction_0.vec);
				dx_1.vec = _mm_mul_ps(_mm_normalize3_ps(dx_1.vec),_mm_set1_ps(p1->radius));
			#elif USE_ALTIVEC
				direction_0.vec = vec_sub(p2->position.vec,p1->position.vec);
				direction_1.vec = vec_sub(camera.vec,p1->position.vec);
				dx_1.vec = vec_cross(direction_1.vec,direction_0.vec);
				dx_1.vec = vec_madd(vec_normalize3(dx_1.vec),vec_splats(p1->radius),vec_splats(0.0f));
			#elif USE_NEON
				direction_0.vec = vsubq_f32(p2->position.vec,p1->position.vec);
				direction_1.vec = vsubq_f32(camera.vec,p1->position.vec);
				dx_1.vec = vcrossq_f32(direction_1.vec,direction_0.vec);
				dx_1.vec = vmulq_f32(vnormalize3q_f32(dx_1.vec),vdupq_n_f32(p1->radius));
			#else
				sub(direction_0,p2->position,p1->position);
				sub(direction_1,camera,p1->position);
				cross(dx_1,direction_1,direction_0);
				mul(dx_1,dx_1,Math::rsqrtFast(length2(dx_1)) * p1->radius);
			#endif
			u32 color_1 = (65535 - Math::round(p1->life * p1->ilife)) << 16;
			u32 orientation_1 = clamp(255 * (i - 1) / num_particles,0,255);
			// v->xyz = p0->position + dx_0;
			// v->xyz = p0->position - dx_0;
			// v->xyz = p1->position - dx_1;
			// v->xyz = p1->position + dx_1;
			#ifdef USE_SSE
				v[0].vec = _mm_add_ps(p0->position.vec,dx_0.vec);
				v[0].parameters = color_0 | orientation_0;
				v[1].vec = _mm_sub_ps(p0->position.vec,dx_0.vec);
				v[1].parameters = color_0 | orientation_0 | 65280;
				v[2].vec = _mm_sub_ps(p1->position.vec,dx_1.vec);
				v[2].parameters = color_1 | orientation_1 | 65280;
				v[3].vec = _mm_add_ps(p1->position.vec,dx_1.vec);
				v[3].parameters = color_1 | orientation_1;
			#elif USE_ALTIVEC
				v[0].vec = vec_add(p0->position.vec,dx_0.vec);
				v[0].parameters = color_0 | orientation_0;
				v[1].vec = vec_sub(p0->position.vec,dx_0.vec);
				v[1].parameters = color_0 | orientation_0 | 65280;
				v[2].vec = vec_sub(p1->position.vec,dx_1.vec);
				v[2].parameters = color_1 | orientation_1 | 65280;
				v[3].vec = vec_add(p1->position.vec,dx_1.vec);
				v[3].parameters = color_1 | orientation_1;
			#elif USE_NEON
				v[0].vec = vaddq_f32(p0->position.vec,dx_0.vec);
				v[0].parameters = color_0 | orientation_0;
				v[1].vec = vsubq_f32(p0->position.vec,dx_0.vec);
				v[1].parameters = color_0 | orientation_0 | 65280;
				v[2].vec = vsubq_f32(p1->position.vec,dx_1.vec);
				v[2].parameters = color_1 | orientation_1 | 65280;
				v[3].vec = vaddq_f32(p1->position.vec,dx_1.vec);
				v[3].parameters = color_1 | orientation_1;
			#else
				add3(v[0].xyz,p0->position,dx_0);
				v[0].parameters = color_0 | orientation_0;
				sub3(v[1].xyz,p0->position,dx_0);
				v[1].parameters = color_0 | orientation_0 | 65280;
				sub3(v[2].xyz,p1->position,dx_1);
				v[2].parameters = color_1 | orientation_1 | 65280;
				add3(v[3].xyz,p1->position,dx_1);
				v[3].parameters = color_1 | orientation_1;
			#endif
			
			v += 4;
			
			p0 = p1;
			p1 = p2;
			dx_0 = dx_1;
			color_0 = color_1;
			orientation_0 = orientation_1;
		}
		
		for(s32 i = 0; i < 8; i++) {
			add3(v->xyz,p0->position,dx_0);
			v->parameters = color_0 | orientations[p0->orientation][2];
			v++;
		}
	}
}

//**********************************  Parameters ********************************************
void SVParticles::setSeed(u32 seed) {
	random.setSeed(seed);
}

u32 SVParticles::getSeed() const {
	return random.getSeed();
}

void SVParticles::setWorld(SVParticlesWorldBasePtr w) {
	//m_pWorld = w;
}

SVParticlesWorldBasePtr SVParticles::getWorld() {
	return m_pWorld;
}

void SVParticles::setType(s32 t) {
    if(t>=0 && t<NUM_TYPES) {
        type = t;
        update_bounds();
    }
}

s32 SVParticles::getType() const {
	return type;
}

void SVParticles::setWarming(s32 w) {
	warming = w;
}

s32 SVParticles::getWarming() const {
	return warming;
}

void SVParticles::setDepthSort(s32 sort) {
	depth_sort = sort;
}
	
s32 SVParticles::getDepthSort() const {
	return depth_sort;
}

void SVParticles::setVariationX(s32 v) {
	variation_x = v;
}
	
s32 SVParticles::getVariationX() const {
	return variation_x;
}

void SVParticles::setVariationY(s32 v) {
	variation_y = v;
}
	
s32 SVParticles::getVariationY() const {
	return variation_y;
}

void SVParticles::setTextureAtlas(s32 atlas) {
	texture_atlas = atlas;
}
	
s32 SVParticles::getTextureAtlas() const {
	return texture_atlas;
}

void SVParticles::setIntersection(s32 i) {
	intersection = i;
}

s32 SVParticles::getIntersection() const {
	return intersection;
}

void SVParticles::setCollision(s32 c) {
	collision = c;
}

s32 SVParticles::getCollision() const {
	return collision;
}

void SVParticles::setCulling(s32 c) {
	culling = c;
}

s32 SVParticles::getCulling() const {
	return culling;
}

void SVParticles::setSpawnRate(f32 spawn) {
	spawn_rate = max(spawn,0.0f);
}

f32 SVParticles::getSpawnRate() const {
	return spawn_rate;
}

void SVParticles::setSpawnScale(f32 scale) {
	spawn_scale = saturate(scale);
}

f32 SVParticles::getSpawnScale() const {
	return spawn_scale;
}

void SVParticles::setSpawnThreshold(f32 threshold) {
	spawn_threshold = max(threshold,0.0f);
}

f32 SVParticles::getSpawnThreshold() const {
	return spawn_threshold;
}

void SVParticles::setWorldMass(f32 mass) {
	world_mass = max(mass,0.0f);
	if(world_mass != 0.0f) world_imass = Math::rcp(world_mass);
	else world_imass = 0.0f;
}

f32 SVParticles::getWorldMass() const {
	return world_mass;
}

void SVParticles::setLengthStretch(f32 stretch) {
	length_stretch = max(stretch,0.0f);
	update_bounds();
}

f32 SVParticles::getLengthStretch() const {
	return length_stretch;
}

void SVParticles::setLinearDamping(f32 damping) {
	linear_damping = max(damping,0.0f);
}

f32 SVParticles::getLinearDamping() const {
	return linear_damping;
}

void SVParticles::setAngularDamping(f32 damping) {
	angular_damping = max(damping,0.0f);
}

f32 SVParticles::getAngularDamping() const {
	return angular_damping;
}

void SVParticles::setGrowthDamping(f32 damping) {
	growth_damping = max(damping,0.0f);
}

f32 SVParticles::getGrowthDamping() const {
	return growth_damping;
}

void SVParticles::setRestitution(f32 r) {
	restitution = saturate(r);
}

f32 SVParticles::getRestitution() const {
	return restitution;
}

void SVParticles::setRoughness(f32 r) {
	roughness = saturate(r);
}

f32 SVParticles::getRoughness() const {
	return roughness;
}

//******************************* Emitter **********************************************
void SVParticles::updateEmitter() {
	delay_time = delay_mean + random.getFloat(-delay_spread,delay_spread);
	period_time = period_mean + random.getFloat(-period_spread,period_spread);
	duration_time = duration_mean + random.getFloat(-duration_spread,duration_spread);
}

void SVParticles::setEmitterType(s32 type) {
    if(type>=0 && type<NUM_EMITTERS) {
       emitter_type = type;
    }
}

s32 SVParticles::getEmitterType() const {
	return emitter_type;
}

void SVParticles::setEmitterEnabled(s32 enable) {
    if(emitter_enabled != enable){
        updateEmitter();
    }
	emitter_enabled = enable;
}

s32 SVParticles::isEmitterEnabled() const {
	return emitter_enabled;
}

void SVParticles::setEmitterBased(s32 based) {
	emitter_based = based;
}

s32 SVParticles::isEmitterBased() const {
	return emitter_based;
}

void SVParticles::setEmitterShift(s32 shift) {
	emitter_shift = shift;
}

s32 SVParticles::isEmitterShift() const {
	return emitter_shift;
}

void SVParticles::setEmitterContinuous(s32 continuous) {
	emitter_continuous = continuous;
}

s32 SVParticles::isEmitterContinuous() const {
	return emitter_continuous;
}

void SVParticles::setEmitterSequence(s32 sequence) {
	emitter_sequence = max(sequence,0);
}

s32 SVParticles::getEmitterSequence() const {
	return emitter_sequence;
}

void SVParticles::setEmitterLimit(s32 limit) {
	emitter_limit = max(limit,0);
}

s32 SVParticles::getEmitterLimit() const {
	return emitter_limit;
}

void SVParticles::setEmitterSize(const FVec3 &size) {
	emitter_size = max(size,FVec3_zero);
	update_bounds();
}

const FVec3 &SVParticles::getEmitterSize() const {
	return emitter_size;
}

void SVParticles::setEmitterDirection(const FVec3 &direction) {
	emitter_direction = direction;
	update_bounds();
}

const FVec3 &SVParticles::getEmitterDirection() const {
	return emitter_direction;
}

void SVParticles::setEmitterSpread(const FVec3 &spread) {
	emitter_spread = max(spread,FVec3_zero);
	update_bounds();
}

const FVec3 &SVParticles::getEmitterSpread() const {
	return emitter_spread;
}

void SVParticles::setEmitterVelocity(const FVec3 &velocity) {
	emitter_velocity = velocity;
}

const FVec3 &SVParticles::getEmitterVelocity() const {
	return emitter_velocity;
}

void SVParticles::setEmitterTransform(const FMat4 &transform) {
	// update particle transformation
	if(emitter_based) {
		FMat4 t = transform * inverse(emitter_transform);
		if(type == TYPE_FLAT || type == TYPE_LENGTH || type == TYPE_RANDOM || type == TYPE_ROUTE) {
			for(s32 i = 0; i < m_particles.size(); i++) {
				Particle &p = m_particles[i];
				mul(p.position,t,FVec3(p.position));
				mul(p.old_position,t,FVec3(p.old_position));
				mul3(p.parameters,t,FVec3(p.parameters));
				mul3(p.velocity,t,FVec3(p.velocity));
			}
		} else {
			for(s32 i = 0; i < m_particles.size(); i++) {
				Particle &p = m_particles[i];
				mul(p.position,t,FVec3(p.position));
				mul(p.old_position,t,FVec3(p.old_position));
				mul3(p.velocity,t,FVec3(p.velocity));
			}
		}
		particles_bound_box.setTransform(t);
	}
	// set emitter transform
	emitter_transform = transform;
	if(frame == 0 || emitter_enabled == 0 || emitter_continuous == 0 || spawn_rate * spawn_scale < EPSILON) {
		old_emitter_transform = emitter_transform;
	}
	update_bounds();
}

const FMat4 &SVParticles::getEmitterTransform() const {
	return emitter_transform;
}

void SVParticles::addEmitterSpark(const FVec3 &point,const FVec3 &normal,const FVec3 &velocity) {
    //fyz
    Spark &s = sparks.append();
    s.point = point;
    s.normal = normal;
    s.velocity = velocity;
    bound_box.expand(point);
    bound_sphere.set(bound_box);
}

//*********************************** Dynamic n***************************
void SVParticles::setDelay(f32 mean,f32 spread) {
	delay_mean = max(mean,0.0f);
	delay_spread = spread;
}

f32 SVParticles::getDelayMean() const {
	return delay_mean;
}

f32 SVParticles::getDelaySpread() const {
	return delay_spread;
}

void SVParticles::setPeriod(f32 mean,f32 spread) {
	period_mean = max(mean,0.0f);
	period_spread = spread;
	period_time = -1.0f;
	duration_time = -1.0f;
}

f32 SVParticles::getPeriodMean() const {
	return period_mean;
}

f32 SVParticles::getPeriodSpread() const {
	return period_spread;
}

void SVParticles::setDuration(f32 mean,f32 spread) {
	duration_mean = max(mean,0.0f);
	duration_spread = spread;
	period_time = -1.0f;
	duration_time = -1.0f;
}

f32 SVParticles::getDurationMean() const {
	return duration_mean;
}

f32 SVParticles::getDurationSpread() const {
	return duration_spread;
}

void SVParticles::setLife(f32 mean,f32 spread) {
	life_mean = max(mean,EPSILON);
	life_spread = spread;
	update_bounds();
}

f32 SVParticles::getLifeMean() const {
	return life_mean;
}

f32 SVParticles::getLifeSpread() const {
	return life_spread;
}

void SVParticles::setVelocity(f32 mean,f32 spread) {
	velocity_mean = mean;
	velocity_spread = spread;
	update_bounds();
}

f32 SVParticles::getVelocityMean() const {
	return velocity_mean;
}

f32 SVParticles::getVelocitySpread() const {
	return velocity_spread;
}

void SVParticles::setAngle(f32 mean,f32 spread) {
	angle_mean = mean;
	angle_spread = spread;
}

f32 SVParticles::getAngleMean() const {
	return angle_mean;
}

f32 SVParticles::getAngleSpread() const {
	return angle_spread;
}

void SVParticles::setRotation(f32 mean,f32 spread) {
	rotation_mean = mean;
	rotation_spread = spread;
}

f32 SVParticles::getRotationMean() const {
	return rotation_mean;
}

f32 SVParticles::getRotationSpread() const {
	return rotation_spread;
}

void SVParticles::setRadius(f32 mean,f32 spread) {
	radius_mean = max(mean,EPSILON);
	radius_spread = spread;
	update_bounds();
}

f32 SVParticles::getRadiusMean() const {
	return radius_mean;
}

f32 SVParticles::getRadiusSpread() const {
	return radius_spread;
}

void SVParticles::setFade(f32 _fade){
    fade = _fade;
}
f32 SVParticles::getFade(){
    return fade;
}

void SVParticles::setGrowth(f32 mean,f32 spread) {
	growth_mean = mean;
	growth_spread = spread;
	update_bounds();
}

f32 SVParticles::getGrowthMean() const {
	return growth_mean;
}

f32 SVParticles::getGrowthSpread() const {
	return growth_spread;
}

//*********************************** Forces *******************************************
void SVParticles::setGravity(const FVec3 &g) {
	gravity = g;
	update_bounds();
}

const FVec3 &SVParticles::getGravity() const {
	return gravity;
}

s32 SVParticles::addForce() {
    Force &f = forces.append();
    f.transform = FMat4_identity;
    f.attached = 1;
    f.radius = 1.0f;
    f.attenuation = 1.0f;
    f.attractor = 0.0f;
    f.rotator = 0.0f;
    return forces.size() - 1;
}

void SVParticles::removeForce(s32 num) {
    assert(num >= 0 && num < forces.size() && "SVParticles::removeForce(): bad force number");
    forces.remove(num);
}

s32 SVParticles::getNumForces() const {
	return forces.size();
}

void SVParticles::setForceAttached(s32 num,s32 attached) {
	assert(num >= 0 && num < forces.size() && "SVParticles::setForceAttached(): bad force number");
	forces[num].attached = attached;
}

s32 SVParticles::isForceAttached(s32 num) const {
	assert(num >= 0 && num < forces.size() && "SVParticles::isForceAttached(): bad force number");
	return forces[num].attached;
}

void SVParticles::setForceTransform(s32 num,const FMat4 &transform) {
	assert(num >= 0 && num < forces.size() && "SVParticles::setForceTransform(): bad force number");
	forces[num].transform = transform;
}

const FMat4 &SVParticles::getForceTransform(s32 num) const {
	assert(num >= 0 && num < forces.size() && "SVParticles::getForceTransform(): bad force number");
	return forces[num].transform;
}

void SVParticles::setForceRadius(s32 num,f32 radius) {
	assert(num >= 0 && num < forces.size() && "SVParticles::setForceRadius(): bad force number");
	forces[num].radius = max(radius,0.0f);
}

f32 SVParticles::getForceRadius(s32 num) const {
	assert(num >= 0 && num < forces.size() && "SVParticles::getForceRadius(): bad force number");
	return forces[num].radius;
}

void SVParticles::setForceAttenuation(s32 num,f32 attenuation) {
	assert(num >= 0 && num < forces.size() && "SVParticles::setForceAttenuation(): bad force number");
	forces[num].attenuation = max(attenuation,EPSILON);
}

f32 SVParticles::getForceAttenuation(s32 num) const {
	assert(num >= 0 && num < forces.size() && "SVParticles::getForceAttenuation(): bad force number");
	return forces[num].attenuation;
}

void SVParticles::setForceAttractor(s32 num,f32 attractor) {
	assert(num >= 0 && num < forces.size() && "SVParticles::setForceAttractor(): bad force number");
	forces[num].attractor = attractor;
}

f32 SVParticles::getForceAttractor(s32 num) const {
	assert(num >= 0 && num < forces.size() && "SVParticles::getForceAttractor(): bad force number");
	return forces[num].attractor;
}

void SVParticles::setForceRotator(s32 num,f32 rotator) {
	assert(num >= 0 && num < forces.size() && "SVParticles::setForceRotator(): bad force number");
	forces[num].rotator = rotator;
}

f32 SVParticles::getForceRotator(s32 num) const {
	assert(num >= 0 && num < forces.size() && "SVParticles::getForceRotator(): bad force number");
	return forces[num].rotator;
}

//*******************************  Noises ***********************************************
s32 SVParticles::addNoise() {
    Noise &n = noises.append();
    n.transform = FMat4_identity;
    n.itransform = FMat4_identity;
    n.offset = FVec3_zero;
    n.step = FVec3_one;
    n.attached = 1;
    n.force = 1.0f;
    n.scale = 0.5f;
    n.frequency = 4;
    n.size = 16;
    //n.image = NULL;
    return noises.size() - 1;
}

void SVParticles::removeNoise(s32 num) {
    assert(num >= 0 && num < noises.size() && "SVParticles::removeNoise(): bad noise number");
    noises.remove(num);
}

s32 SVParticles::getNumNoises() const {
	return noises.size();
}

void SVParticles::setNoiseAttached(s32 num,s32 attached) {
	assert(num >= 0 && num < noises.size() && "SVParticles::setNoiseAttached(): bad noise number");
	noises[num].attached = attached;
}

s32 SVParticles::isNoiseAttached(s32 num) const {
	assert(num >= 0 && num < noises.size() && "SVParticles::isNoiseAttached(): bad noise number");
	return noises[num].attached;
}

void SVParticles::setNoiseTransform(s32 num,const FMat4 &transform) {
	assert(num >= 0 && num < noises.size() && "SVParticles::setNoiseTransform(): bad noise number");
	noises[num].transform = transform;
}

const FMat4 &SVParticles::getNoiseTransform(s32 num) const {
	assert(num >= 0 && num < noises.size() && "SVParticles::getNoiseTransform(): bad noise number");
	return noises[num].transform;
}

void SVParticles::setNoiseOffset(s32 num,const FVec3 &offset) {
	assert(num >= 0 && num < noises.size() && "SVParticles::setNoiseOffset(): bad noise number");
	noises[num].offset = offset;
}

const FVec3 &SVParticles::getNoiseOffset(s32 num) const {
	assert(num >= 0 && num < noises.size() && "SVParticles::getNoiseOffset(): bad noise number");
	return noises[num].offset;
}

void SVParticles::setNoiseStep(s32 num,const FVec3 &step) {
	assert(num >= 0 && num < noises.size() && "SVParticles::setNoiseStep(): bad noise number");
	noises[num].step = step;
}

const FVec3 &SVParticles::getNoiseStep(s32 num) const {
	assert(num >= 0 && num < noises.size() && "SVParticles::getNoiseStep(): bad noise number");
	return noises[num].step;
}

void SVParticles::setNoiseForce(s32 num,f32 force) {
	assert(num >= 0 && num < noises.size() && "SVParticles::setNoiseForce(): bad noise number");
	noises[num].force = force;
}

f32 SVParticles::getNoiseForce(s32 num) const {
	assert(num >= 0 && num < noises.size() && "SVParticles::getNoiseForce(): bad noise number");
	return noises[num].force;
}

void SVParticles::setNoiseScale(s32 num,f32 scale) {
	assert(num >= 0 && num < noises.size() && "SVParticles::setNoiseScale(): bad noise number");
	noises[num].scale = saturate(scale);
//    delete noises[num].image;
//    noises[num].image = NULL;
}

f32 SVParticles::getNoiseScale(s32 num) const {
	assert(num >= 0 && num < noises.size() && "SVParticles::getNoiseScale(): bad noise number");
	return noises[num].scale;
}

//
void SVParticles::setNoiseFrequency(s32 num,s32 frequency) {
	assert(num >= 0 && num < noises.size() && "SVParticles::setNoiseFrequency(): bad noise number");
	noises[num].frequency = clamp(frequency,1,32);
//    delete noises[num].image;
//    noises[num].image = NULL;
}

s32 SVParticles::getNoiseFrequency(s32 num) const {
	assert(num >= 0 && num < noises.size() && "SVParticles::getNoiseFrequency(): bad noise number");
	return noises[num].frequency;
}

//
void SVParticles::setNoiseSize(s32 num,s32 size) {
	assert(num >= 0 && num < noises.size() && "SVParticles::setNoiseSize(): bad noise number");
	noises[num].size = clamp(size,1,512);
//    delete noises[num].image;
//    noises[num].image = NULL;
}

s32 SVParticles::getNoiseSize(s32 num) const {
	assert(num >= 0 && num < noises.size() && "SVParticles::getNoiseSize(): bad noise number");
	return noises[num].size;
}

///*
// */
//Image *SVParticles::getNoiseImage(s32 num) {
//    assert(num >= 0 && num < noises.size() && "SVParticles::getNoiseImage(): bad noise number");
//    if(noises[num].image == NULL) {
//        Noise &n = noises[num];
//        ::Noise x_noise(num);
//        ::Noise y_noise(num + 113);
//        ::Noise z_noise(num + 49344);
//        n.image = new Image();
//        n.image->create3D(n.size,n.size,n.size,Image::FORMAT_RGBA8);
//        f32 size = Math::itof(n.size);
//        for(s32 z = 0; z < n.size; z++) {
//            f32 sz = Math::itof(z) * n.scale;
//            for(s32 y = 0; y < n.size; y++) {
//                f32 sy = Math::itof(y) * n.scale;
//                for(s32 x = 0; x < n.size; x++) {
//                    f32 sx = Math::itof(x) * n.scale;
//                    s32 nx = clamp(Math::ftoi(x_noise.getTileableTurbulence3(sx,sy,sz,size,size,size,n.frequency) * 127.5f + 127.5f),0,255);
//                    s32 ny = clamp(Math::ftoi(y_noise.getTileableTurbulence3(sx,sy,sz,size,size,size,n.frequency) * 127.5f + 127.5f),0,255);
//                    s32 nz = clamp(Math::ftoi(z_noise.getTileableTurbulence3(sx,sy,sz,size,size,size,n.frequency) * 127.5f + 127.5f),0,255);
//                    n.image->set3D(x,y,z,nx,ny,nz,255);
//                }
//            }
//        }
//    }
//    return noises[num].image;
//}

//************************************ Deflectors ******************************************
s32 SVParticles::addDeflector() {
    Deflector &d = deflectors.append();
    d.transform = FMat4_identity;
    d.size = FVec3(1.0f,1.0f,0.0f);
    d.type = DEFLECTOR_REFLECTOR;
    d.attached = 1;
    d.restitution = 0.85f;
    d.roughness = 0.0f;
    return deflectors.size() - 1;
}

void SVParticles::removeDeflector(s32 num) {
    if(num >= 0 && num < deflectors.size()) {
        deflectors.removeForce(num);
    }
}

s32 SVParticles::getNumDeflectors() const {
	return deflectors.size();
}

void SVParticles::setDeflectorType(s32 num,s32 type) {
    if(num >= 0 && num < deflectors.size()) {
        if(type >= DEFLECTOR_REFLECTOR && type <= DEFLECTOR_CLIPPER) {
            deflectors[num].type = type;
        }
    }
}

s32 SVParticles::getDeflectorType(s32 num) const {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::getDeflectorType(): bad deflector number");
	return deflectors[num].type;
}

void SVParticles::setDeflectorAttached(s32 num,s32 attached) {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::setDeflectorAttached(): bad deflector number");
	deflectors[num].attached = attached;
}

s32 SVParticles::isDeflectorAttached(s32 num) const {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::isDeflectorAttached(): bad deflector number");
	return deflectors[num].attached;
}

void SVParticles::setDeflectorTransform(s32 num,const FMat4 &transform) {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::setDeflectorTransform(): bad deflector number");
	deflectors[num].transform = transform;
}

const FMat4 &SVParticles::getDeflectorTransform(s32 num) const {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::getDeflectorTransform(): bad deflector number");
	return deflectors[num].transform;
}

void SVParticles::setDeflectorSize(s32 num,const FVec3 &size) {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::setDeflectorSize(): bad deflector number");
	deflectors[num].size = max(size,FVec3_zero);
}

const FVec3 &SVParticles::getDeflectorSize(s32 num) const {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::getDeflectorSize(): bad deflector number");
	return deflectors[num].size;
}

void SVParticles::setDeflectorRestitution(s32 num,f32 restitution) {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::setDeflectorRestitution(): bad deflector number");
	deflectors[num].restitution = saturate(restitution);
}

f32 SVParticles::getDeflectorRestitution(s32 num) const {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::getDeflectorRestitution(): bad deflector number");
	return deflectors[num].restitution;
}

void SVParticles::setDeflectorRoughness(s32 num,f32 roughness) {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::setDeflectorRoughness(): bad deflector number");
	deflectors[num].roughness = saturate(roughness);
}

f32 SVParticles::getDeflectorRoughness(s32 num) const {
	assert(num >= 0 && num < deflectors.size() && "SVParticles::getDeflectorRoughness(): bad deflector number");
	return deflectors[num].roughness;
}

void SVParticles::_getRandomVextexColor(FVec3 &_color){
    if (m_vetexColor.size() > 0) {
        s32 t_r = random.getInt(0, m_vetexColor.size());
        _color.set(m_vetexColor[t_r]);
        
    }else{
        _color.set(1.0f, 1.0f, 1.0f);
    }
//    _color.set(random.getFloat(0.5, 0.8), random.getFloat(0.2, 0.3), random.getFloat(0.6, 0.9));
}

//序列化接口
void SVParticles::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                          RAPIDJSON_NAMESPACE::Value &_objValue) {

    RAPIDJSON_NAMESPACE::Value baseObj(RAPIDJSON_NAMESPACE::kObjectType);
    baseObj.AddMember("base_type", type, _allocator);
    baseObj.AddMember("base_warm", warming, _allocator);
    baseObj.AddMember("base_depthsort", depth_sort, _allocator);
    baseObj.AddMember("base_varx", variation_x, _allocator);
    baseObj.AddMember("base_vary", variation_y, _allocator);
    baseObj.AddMember("base_atlas", texture_atlas, _allocator);
    baseObj.AddMember("base_intersection", intersection, _allocator);
    baseObj.AddMember("base_collision", collision, _allocator);
    baseObj.AddMember("base_culling", culling, _allocator);
    baseObj.AddMember("base_spawn_rate", spawn_rate, _allocator);
    baseObj.AddMember("base_spawn_scale", spawn_scale, _allocator);
    baseObj.AddMember("base_spawn_threshold", spawn_threshold, _allocator);
    baseObj.AddMember("base_delay", delay_time, _allocator);
    baseObj.AddMember("base_period", period_time, _allocator);
    baseObj.AddMember("base_duration", duration_time, _allocator);
    baseObj.AddMember("base_fade", fade, _allocator);
    _objValue.AddMember("base", baseObj, _allocator);
    //
    RAPIDJSON_NAMESPACE::Value dynObj(RAPIDJSON_NAMESPACE::kObjectType);
    dynObj.AddMember("dyn_delay_mean", delay_mean, _allocator);
    dynObj.AddMember("dyn_delay_spread", delay_spread, _allocator);
    dynObj.AddMember("dyn_period_mean", period_mean, _allocator);
    dynObj.AddMember("dyn_period_spread", period_spread, _allocator);
    dynObj.AddMember("dyn_duration_mean", duration_mean, _allocator);
    dynObj.AddMember("dyn_duration_spread", duration_spread, _allocator);
    dynObj.AddMember("dyn_life_mean", life_mean, _allocator);
    dynObj.AddMember("dyn_life_spread", life_spread, _allocator);
    dynObj.AddMember("dyn_velocity_mean", velocity_mean, _allocator);
    dynObj.AddMember("dyn_velocity_spread", velocity_spread, _allocator);
    dynObj.AddMember("dyn_angle_mean", angle_mean, _allocator);
    dynObj.AddMember("dyn_angle_spread", angle_spread, _allocator);
    dynObj.AddMember("dyn_rotation_mean", rotation_mean, _allocator);
    dynObj.AddMember("dyn_rotation_spread", rotation_spread, _allocator);
    dynObj.AddMember("dyn_radius_mean", radius_mean, _allocator);
    dynObj.AddMember("dyn_radius_spread", radius_spread, _allocator);
    dynObj.AddMember("dyn_growth_mean", growth_mean, _allocator);
    dynObj.AddMember("dyn_growth_spread", growth_spread, _allocator);
    _objValue.AddMember("dyn", dynObj, _allocator);
    //
    RAPIDJSON_NAMESPACE::Value phyObj(RAPIDJSON_NAMESPACE::kObjectType);
    phyObj.AddMember("phy_grave_x", gravity.x, _allocator);
    phyObj.AddMember("phy_grave_y", gravity.y, _allocator);
    phyObj.AddMember("phy_grave_z", gravity.z, _allocator);
    phyObj.AddMember("phy_mass", world_mass, _allocator);
    phyObj.AddMember("phy_imass", world_imass, _allocator);
    phyObj.AddMember("phy_stretch", length_stretch, _allocator);
    phyObj.AddMember("phy_linear_damp", linear_damping, _allocator);
    phyObj.AddMember("phy_angular_damp", angular_damping, _allocator);
    phyObj.AddMember("phy_grow_damp", growth_damping, _allocator);
    phyObj.AddMember("phy_restitution", restitution, _allocator);
    phyObj.AddMember("phy_rouughness", roughness, _allocator);
    _objValue.AddMember("phy", phyObj, _allocator);
    //
    RAPIDJSON_NAMESPACE::Value emitObj(RAPIDJSON_NAMESPACE::kObjectType);
    emitObj.AddMember("emitter_type", emitter_type, _allocator);
    emitObj.AddMember("emitter_enabled", emitter_enabled, _allocator);
    emitObj.AddMember("emitter_based", emitter_based, _allocator);
    emitObj.AddMember("emitter_shift", emitter_shift, _allocator);
    emitObj.AddMember("emitter_continuous", emitter_continuous, _allocator);
    emitObj.AddMember("emitter_sequence", emitter_sequence, _allocator);
    emitObj.AddMember("emitter_limit", emitter_limit, _allocator);
    emitObj.AddMember("emitter_size_x", emitter_size.x, _allocator);
    emitObj.AddMember("emitter_size_y", emitter_size.y, _allocator);
    emitObj.AddMember("emitter_size_z", emitter_size.z, _allocator);
    emitObj.AddMember("emitter_direction_x", emitter_direction.x, _allocator);
    emitObj.AddMember("emitter_direction_y", emitter_direction.y, _allocator);
    emitObj.AddMember("emitter_direction_z", emitter_direction.z, _allocator);
    emitObj.AddMember("emitter_spread_x", emitter_spread.x, _allocator);
    emitObj.AddMember("emitter_spread_y", emitter_spread.y, _allocator);
    emitObj.AddMember("emitter_spread_z", emitter_spread.z, _allocator);
    emitObj.AddMember("emitter_velocity_x", emitter_velocity.x, _allocator);
    emitObj.AddMember("emitter_velocity_y", emitter_velocity.y, _allocator);
    emitObj.AddMember("emitter_velocity_z", emitter_velocity.z, _allocator);
    _objValue.AddMember("emit", emitObj, _allocator);
}

void SVParticles::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {
    if (item.HasMember("base") && item["base"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value baseobj = item["base"].GetObject();
        setType( baseobj["base_type"].GetInt() );
        warming = baseobj["base_warm"].GetInt();
        depth_sort = baseobj["base_depthsort"].GetInt();
        variation_x = baseobj["base_varx"].GetInt();
        variation_y = baseobj["base_vary"].GetInt();
        texture_atlas = baseobj["base_atlas"].GetInt();
        intersection = baseobj["base_intersection"].GetInt();
        collision = baseobj["base_collision"].GetInt();
        culling = baseobj["base_culling"].GetInt();
        spawn_rate = baseobj["base_spawn_rate"].GetFloat();
        spawn_scale = baseobj["base_spawn_scale"].GetFloat();
        spawn_threshold = baseobj["base_spawn_threshold"].GetFloat();
        delay_time = baseobj["base_delay"].GetFloat();
        period_time = baseobj["base_period"].GetFloat();
        duration_time = baseobj["base_duration"].GetFloat();
        fade = baseobj["base_fade"].GetFloat();
//        //
//        setWarming(warming);
//        setDepthSort(depth_sort);
//        setVariationX(variation_x);
//        setVariationY(variation_y);
//        setTextureAtlas(texture_atlas);
//        setIntersection(intersection);
//        setCollision(collision);
//        setCulling(culling);
//        setSpawnRate(spawn_rate);
//        setSpawnScale(spawn_scale);
//        setSpawnThreshold(spawn_threshold);
    }
    //
    if (item.HasMember("dyn") && item["dyn"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value dynObj = item["dyn"].GetObject();
        delay_mean = dynObj["dyn_delay_mean"].GetFloat();
        delay_spread = dynObj["dyn_delay_spread"].GetFloat();
        period_mean = dynObj["dyn_period_mean"].GetFloat();
        period_spread = dynObj["dyn_period_spread"].GetFloat();
        duration_mean = dynObj["dyn_duration_mean"].GetFloat();
        duration_spread = dynObj["dyn_duration_spread"].GetFloat();
        life_mean = dynObj["dyn_life_mean"].GetFloat();
        life_spread = dynObj["dyn_life_spread"].GetFloat();
        velocity_mean = dynObj["dyn_velocity_mean"].GetFloat();
        velocity_spread = dynObj["dyn_velocity_spread"].GetFloat();
        angle_mean = dynObj["dyn_angle_mean"].GetFloat();
        angle_spread = dynObj["dyn_angle_spread"].GetFloat();
        rotation_mean = dynObj["dyn_rotation_mean"].GetFloat();
        rotation_spread = dynObj["dyn_rotation_spread"].GetFloat();
        radius_mean = dynObj["dyn_radius_mean"].GetFloat();
        radius_spread = dynObj["dyn_radius_spread"].GetFloat();
        growth_mean = dynObj["dyn_growth_mean"].GetFloat();
        growth_spread = dynObj["dyn_growth_spread"].GetFloat();
    }
    //
    if (item.HasMember("phy") && item["phy"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value phyobj = item["phy"].GetObject();
        gravity.x = phyobj["phy_grave_x"].GetFloat();
        gravity.y = phyobj["phy_grave_y"].GetFloat();
        gravity.z = phyobj["phy_grave_z"].GetFloat();
        world_mass = phyobj["phy_mass"].GetFloat();
        world_imass = phyobj["phy_imass"].GetFloat();
        length_stretch = phyobj["phy_stretch"].GetFloat();
        linear_damping = phyobj["phy_linear_damp"].GetFloat();
        angular_damping = phyobj["phy_angular_damp"].GetFloat();
        growth_damping = phyobj["phy_grow_damp"].GetFloat();
        restitution = phyobj["phy_restitution"].GetFloat();
        roughness = phyobj["phy_rouughness"].GetFloat();
//        setWorldMass(world_mass);
//        setLengthStretch(length_stretch);
//        setLinearDamping(linear_damping);
//        setAngularDamping(angular_damping);
//        setGrowthDamping(growth_damping);
//        setRestitution(restitution);
//        setRoughness(roughness);
    }
    //
    if (item.HasMember("emit") && item["emit"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value emitobj = item["emit"].GetObject();
        emitter_type = emitobj["emitter_type"].GetInt();
        emitter_enabled = emitobj["emitter_enabled"].GetInt();
        emitter_based = emitobj["emitter_based"].GetInt();
        emitter_shift = emitobj["emitter_shift"].GetInt();
        emitter_continuous = emitobj["emitter_continuous"].GetInt();
        emitter_sequence = emitobj["emitter_sequence"].GetFloat();
        emitter_limit = emitobj["emitter_limit"].GetFloat();
        emitter_size.x = emitobj["emitter_size_x"].GetFloat();
        emitter_size.y = emitobj["emitter_size_y"].GetFloat();
        emitter_size.z = emitobj["emitter_size_x"].GetFloat();
        emitter_direction.x = emitobj["emitter_direction_x"].GetFloat();
        emitter_direction.y = emitobj["emitter_direction_y"].GetFloat();
        emitter_direction.z = emitobj["emitter_direction_z"].GetFloat();
        emitter_spread.x = emitobj["emitter_spread_x"].GetFloat();
        emitter_spread.y = emitobj["emitter_spread_y"].GetFloat();
        emitter_spread.z = emitobj["emitter_spread_z"].GetFloat();
        emitter_velocity.x = emitobj["emitter_velocity_x"].GetFloat();
        emitter_velocity.y = emitobj["emitter_velocity_y"].GetFloat();
        emitter_velocity.z = emitobj["emitter_velocity_z"].GetFloat();
//        //
//        setEmitterType(emitter_type);
//        setEmitterBased(emitter_based);
//        setEmitterShift(emitter_shift);
//        setEmitterContinuous(emitter_continuous);
//        setEmitterSequence(emitter_sequence);
//        setEmitterLimit(emitter_limit);
    }
}

///*
// */
//s32 SVParticles::saveState(const Stream &stream) const {
//    // random seed
//    stream.writeUInt(getSeed());
//    // frame number
//    stream.writeInt(frame);
//    // update
//    stream.writeFloat(spawn_count);
//    // emitter
//    stream.writeFloatArray(getEmitterTransform(),16);
//    stream.writeFloatArray(old_emitter_transform,16);
//    // forces
//    stream.writeFloatArray(getGravity(),3);
//    stream.writeInt(getNumForces());
//    for(s32 i = 0; i < getNumForces(); i++) {
//        stream.writeUChar(isForceAttached(i));
//        stream.writeFloatArray(getForceTransform(i),16);
//        stream.writeFloat(getForceRadius(i));
//        stream.writeFloat(getForceAttenuation(i));
//        stream.writeFloat(getForceAttractor(i));
//        stream.writeFloat(getForceRotator(i));
//    }
//    // noises
//    stream.writeInt(getNumNoises());
//    for(s32 i = 0; i < getNumNoises(); i++) {
//        stream.writeUChar(isNoiseAttached(i));
//        stream.writeFloatArray(getNoiseTransform(i),16);
//        stream.writeFloatArray(getNoiseOffset(i),3);
//        stream.writeFloatArray(getNoiseStep(i),3);
//        stream.writeFloat(getNoiseForce(i));
//        stream.writeFloat(getNoiseScale(i));
//        stream.writeInt(getNoiseFrequency(i));
//        stream.writeInt(getNoiseSize(i));
//    }
//    // deflectors
//    stream.writeInt(getNumDeflectors());
//    for(s32 i = 0; i < getNumDeflectors(); i++) {
//        stream.writeUChar(getDeflectorType(i));
//        stream.writeUChar(isDeflectorAttached(i));
//        stream.writeFloatArray(getDeflectorTransform(i),16);
//        stream.writeFloatArray(getDeflectorSize(i),3);
//        stream.writeFloat(getDeflectorRestitution(i));
//        stream.writeFloat(getDeflectorRoughness(i));
//    }
//    // particles
//    stream.writeInt(particles.size());
//    for(s32 i = 0; i < particles.size(); i++) {
//        const Particle &p = particles[i];
//        stream.writeFloatArray(p.position,3);
//        stream.writeFloatArray(p.parameters,3);
//        stream.writeFloatArray(p.velocity,3);
//        stream.writeFloat(p.angle);
//        stream.writeFloat(p.rotation);
//        stream.writeFloat(p.radius);
//        stream.writeFloat(p.growth);
//        stream.writeFloat(p.life);
//        stream.writeFloat(p.ilife);
//        stream.writeUChar(p.orientation);
//    }
//
//    // bounds
//    stream.writeFloatArray(bound_box.getMin(),3);
//    stream.writeFloatArray(bound_box.getMax(),3);
//    stream.writeFloatArray(bound_sphere.getCenter(),3);
//    stream.writeFloat(bound_sphere.getRadius());
//
//    return 1;
//}
//
//s32 SVParticles::restoreState(const Stream &stream) {
//
//    // random seed
//    setSeed(stream.readUInt());
//
//    // frame number
//    frame = stream.readInt();
//
//    // update
//    spawn_count = stream.readFloat();
//    // type
//    // emitter
//    stream.readFloatArray(emitter_transform,16);
//    stream.readFloatArray(old_emitter_transform,16);
//    // forces
//    forces.resize(stream.readInt());
//    for(s32 i = 0; i < getNumForces(); i++) {
//        setForceAttached(i,stream.readUChar());
//        stream.readFloatArray(forces[i].transform,16);
//        setForceRadius(i,stream.readFloat());
//        setForceAttenuation(i,stream.readFloat());
//        setForceAttractor(i,stream.readFloat());
//        setForceRotator(i,stream.readFloat());
//    }
//
//    // noises
//    noises.resize(stream.readInt());
//    for(s32 i = 0; i < getNumNoises(); i++) {
//        noises[i].image = NULL;
//        setNoiseAttached(i,stream.readUChar());
//        stream.readFloatArray(noises[i].transform,16);
//        stream.readFloatArray(noises[i].offset,3);
//        stream.readFloatArray(noises[i].step,3);
//        setNoiseForce(i,stream.readFloat());
//        setNoiseScale(i,stream.readFloat());
//        setNoiseFrequency(i,stream.readInt());
//        setNoiseSize(i,stream.readInt());
//    }
//
//    // deflectors
//    deflectors.resize(stream.readInt());
//    for(s32 i = 0; i < getNumDeflectors(); i++) {
//        setDeflectorType(i,stream.readUChar());
//        setDeflectorAttached(i,stream.readUChar());
//        stream.readFloatArray(deflectors[i].transform,16);
//        stream.readFloatArray(deflectors[i].size,3);
//        setDeflectorRestitution(i,stream.readFloat());
//        setDeflectorRoughness(i,stream.readFloat());
//    }
//
//    // particles
//    particles.resize(stream.readInt());
//    for(s32 i = 0; i < particles.size(); i++) {
//        Particle &p = particles[i];
//        stream.readFloatArray(p.position,3);
//        stream.readFloatArray(p.parameters,3);
//        stream.readFloatArray(p.velocity,3);
//        p.angle = stream.readFloat();
//        p.rotation = stream.readFloat();
//        p.radius = stream.readFloat();
//        p.growth = stream.readFloat();
//        p.life = stream.readFloat();
//        p.ilife = stream.readFloat();
//        p.orientation = stream.readUChar();
//    }
//
//    // bounds
//    FVec3 min,max;
//    stream.readFloatArray(min,3);
//    stream.readFloatArray(max,3);
//    bound_box.set(min,max);
//
//    FVec3 center;
//    stream.readFloatArray(center,3);
//    bound_sphere.set(center,stream.readFloat());
//
//    return 1;
//}
