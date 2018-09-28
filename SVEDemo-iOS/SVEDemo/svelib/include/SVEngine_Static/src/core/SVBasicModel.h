//
// SVBasicModel.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BASICMODEL_H
#define SV_BASICMODEL_H

#ifdef CONFIG_IS_LOAD_ASSIMP


#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "../base/SVObject.h"
#include "../base/SVColor.h"

class aiScene;

class aiMesh;

namespace sv {
    
    namespace util {
        
        class SVBasicModel : public SVGBase {
        public:
            SVBasicModel(SVInst *_app);
            
            virtual ~SVBasicModel();
            
            //暂时先放这里，可以单独抽出去加载
            bool LoadModel(cptr8 pPath , cptr8 Filename);
            
            const aiScene *getModelData();
            
            virtual void
            getMeshInfo(u32 iMeshIndex, void **pVertexData, u32 &iVertexNum, u32 &iVertexSize,
                        u32 &iMaterialIndex) {};
            
            virtual void
            getIndexInfo(u32 iMeshIndex, void **pIndexData, u32 &iIndexNum, u32 &iIndexSize) {};
            
            cptr8 getname();
            
            void setname(cptr8 _name);
            
            
        private:
            void clear();
            
            Assimp::Importer m_Importer;
            
        protected:
            const aiScene *m_pModelScene;
            
            virtual bool InitFromScene(const aiScene *pScene, cptr8 Filename);
            
            struct BasicMeshEntry {
                BasicMeshEntry() {
                    NumIndices = 0;
                    NumVertex = 0;
                    BaseVertex = 0;
                    BaseIndex = 0;
                    MaterialIndex = 0;
                    pNode = nullptr;
                    pMesh = nullptr;
                }
                
                u32 NumIndices;
                u32 NumVertex;
                u32 BaseVertex;
                u32 BaseIndex;
                u32 MaterialIndex;
                aiNode* pNode;
                aiMesh* pMesh;
            };
            
            SVArray<BasicMeshEntry> m_Entries;
            
            SVString m_strPath;
            
            SVString m_name;
        };
        
    }//!namespace util
    
}//!namespace sv



#endif

#endif //SV_BASICMODEL_H
