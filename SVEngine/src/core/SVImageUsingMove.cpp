//
// SVImageUsingMove.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVImageUsingMove.h"
#include "../base/SVLock.h"

SVImageUsingMove::SVImageUsingMove(){
    m_lock=MakeSharedPtr<SVLock>();
}

SVImageUsingMove::~SVImageUsingMove(){
}

void  SVImageUsingMove::clearContrl(){
    m_lock->lock();
    m_controlArray.clear();
    m_targetArray.clear();
    m_lock->unlock();
}

void SVImageUsingMove::setControl(FVec2 _v){
    m_lock->lock();
    m_controlArray.append(_v);
    m_lock->unlock();
}

void SVImageUsingMove::setTargetControl(FVec2 _v){
    m_lock->lock();
    m_targetArray.append(_v);
    m_lock->unlock();
}

FVec2 SVImageUsingMove::MLS(const FVec2& t){
      m_lock->lock();
    if(m_controlArray.empty()){
         m_lock->unlock();
         return t;
    }//原图像的控制顶点p，与输入点t为同一副图像坐标系下
    
    FVec2 t_fv;
    m_weightArray.clear();
    f64 A[2][2],B[2][2],M[2][2];
    //初始化
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            A[i][j]=0;
            B[i][j]=0;
            M[i][j]=0;
        }
    }

    //
    //SVArray<FVec2>::Iterator iter= m_controlArray.begin();
    //计算各个控制顶点的权重，也就是计算点t到各个顶点的距离1/sqr(d)
    SVArray<FVec2>::Iterator  iter_control=m_controlArray.begin();
    f64 tw=0;
    while(iter_control!=m_controlArray.end()){
        f64 temp;
        if((s32)iter_control->x!=(s32)t.x|| (s32)iter_control->y!=(s32)t.y) {
            temp=1.0/((iter_control->x-t.x)*(iter_control->x-t.x)+(iter_control->y-t.y)*(iter_control->y-t.y));
        } else {
            temp=1.0; //如果t为控制顶点，那么需要把该控制顶点的权重设置为无穷大
        }
        tw+=temp;//总权重
        m_weightArray.append(temp);
        iter_control++;
    }
    if(tw<0.00005){
        m_lock->unlock();
        return t;
    }
    
    SVArray<f64>::Iterator iter_w=m_weightArray.begin();
    SVArray<FVec2>::Iterator iter_target=m_targetArray.begin();
    //target为目标图像的控制点的位置，我们的目标是找到t在q中的对应位置
   iter_control=m_controlArray.begin();
    FVec2 pc,qc;
    f64 px=0,py=0,qx=0,qy=0;
    while(iter_w!=m_weightArray.end()){
        px+=(*iter_w)*(iter_control->x);//所有控制顶点p的加权位置
        py+=(*iter_w)*(iter_control->y);
        qx+=(*iter_w)*(iter_target->x);//所有控制顶点q的加权位置
        qy+=(*iter_w)*(iter_target->y);
        iter_control++;
        iter_w++;
        iter_target++;
    }
    pc.x=px/tw;
    pc.y=py/tw;
    qc.x=qx/tw;
    qc.y=qy/tw;
    iter_control=m_controlArray.begin();
    iter_w=m_weightArray.begin();
    iter_target=m_targetArray.begin();
    //
    while(iter_control!=m_controlArray.end()){
        f64 P[2]={iter_control->x-pc.x,iter_control->y-pc.y};
        f64 PT[2][1];
        PT[0][0]=iter_control->x-pc.x;
        PT[1][0]=iter_control->y-pc.y;
        f64 Q[2]={iter_target->x-qc.x,iter_target->y-qc.y};
        f64 T[2][2];

        T[0][0]=PT[0][0]*P[0];
        T[0][1]=PT[0][0]*P[1];
        T[1][0]=PT[1][0]*P[0];
        T[1][1]=PT[1][0]*P[1];

        for(s32 i=0;i<2;i++) {
            for(s32 j=0;j<2;j++){
                A[i][j]+=(*iter_w)*T[i][j];
            }
        }

        T[0][0]=PT[0][0]*Q[0];
        T[0][1]=PT[0][0]*Q[1];
        T[1][0]=PT[1][0]*Q[0];
        T[1][1]=PT[1][0]*Q[1];

        for(s32 i=0;i<2;i++) {
            for(s32 j=0;j<2;j++){
                B[i][j]+=(*iter_w)*T[i][j];
            }
        }

        iter_control++;
        iter_w++;
        iter_target++;
    }

    f64 det=A[0][0]*A[1][1]-A[0][1]*A[1][0];
    if(det<0.0000001){
        t_fv.x=t.x+qc.x-pc.x;
        t_fv.y=t.y+qc.y-pc.y;
        m_lock->unlock();
        return t_fv;
    }

    A[0][0] = A[1][1]/det;
    A[0][1] = -A[0][1]/det;
    A[1][0] = -A[1][0]/det;
    A[1][1] = A[0][0]/det;

    M[0][0] = A[0][0]*B[0][0]+A[0][1]*B[1][0];
    M[0][1] = A[0][0]*B[0][1]+A[0][1]*B[1][1];
    M[1][0] = A[1][0]*B[0][0]+A[1][1]*B[1][0];
    M[1][1] = A[1][0]*B[0][1]+A[1][1]*B[1][1];
    //
    f64 V[2] = {t.x-pc.x,t.y-pc.y};
    f64 R[2][1];
    //
    R[0][0] = V[0]*M[0][0]+V[1]*M[1][0];//lv（x）总计算公式
    R[1][0] = V[0]*M[0][1]+V[1]*M[1][1];
    t_fv.x  = (s32)(R[0][0]+qc.x);
    t_fv.y  = (s32)(R[1][0]+qc.y);
    m_lock->unlock();
    return t_fv;
}
