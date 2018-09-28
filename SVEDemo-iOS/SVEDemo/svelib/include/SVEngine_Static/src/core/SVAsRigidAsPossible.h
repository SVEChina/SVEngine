//
//  SVAsRigidAsPossible.hpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/7/30.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_ASRIGIDASPOSSIBLE_H
#define SV_ASRIGIDASPOSSIBLE_H
#include "../base/SVObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../base/SVGMatrix.h"

struct LUData {
    GMatrix<double> mLU;
    SVArray<int> vPivots;
};

namespace sv {
    
    namespace util{

        class SVAsRigidAsPossible :public SVRefObject{
        public:
            
            struct Vertex {
                vector2df vPosition;
            };
            
            struct Constraint {
                unsigned int nVertex;
                vector2df vConstrainedPos;
                
                Constraint() { nVertex = 0; vConstrainedPos = vector2df(0,0); }
                Constraint( unsigned int nVert, const vector2df & vPos ) { nVertex = nVert; vConstrainedPos = vPos; }
                
                bool operator<(const Constraint & c2) const
                { return nVertex < c2.nVertex; }
            };
            
            struct Triangle {
                unsigned int nVerts[3];
                
                // definition of each vertex in triangle-local coordinate system
                vector2df vTriCoords[3];
                
                // un-scaled triangle
                vector2df vScaled[3];
                
                // pre-computed matrices for triangle scaling step
                GMatrixd mF, mC;
            };
            
            map<s32, Constraint> m_vConstraints;
            bool m_bSetupValid;
            void initializeFromMesh(V2_T0* _vtdata,int _size,short *_index,int _indexSize );
            void precomputeOrientationMatrix();
            void precomputeScalingMatrices( unsigned int nTriangle );
            void precomputeFittingMatrices();
            void applyFittingStep();
            void updateScaledTriangle( unsigned int nTriangle );
            void validateDeformedMesh( bool bRigid );
            void validateSetup();
            
            LUData m_mLUDecompX, m_mLUDecompY;
            
            GMatrixd m_mHXPrime, m_mHYPrime;
            GMatrixd m_mDX, m_mDY;
            array<Vertex> m_vInitialVerts;
            array<Vertex> m_vDeformedVerts;
            array<Triangle> m_vTriangles;
            array<u32> m_vVertexMap;
            GMatrixd m_mFirstMatrix;
            
        };
        
    }//!namespace util

}//!namespace sv


#endif /* SV_ASRIGIDASPOSSIBLE_H */
