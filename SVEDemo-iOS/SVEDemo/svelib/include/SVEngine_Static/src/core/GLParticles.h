#ifndef __GL_PARTICLES_H__
#define __GL_PARTICLES_H__

//#include "GLExt.h"
#include "SVParticles.h"

namespace sv {
    
    namespace util{
        
        class GLParticles : public SVParticles {
        public:
            GLParticles();
            
            virtual ~GLParticles();
            
            // render destroy
            virtual void destroy();
            
            // render particles
            virtual int render(const FMat4 &modelview,const FVec3 &camera);
            
            // video memory usage
            virtual size_t getMemoryUsage() const;
            
        private:
            
            static int counter;
            static int num_vertex;
            static int vertex_offset;
            static RenderVertex *vertex;
            static GLuint vertex_vao_id;
            static GLuint vertex_vbo_id;
            static int num_indices;
            static GLuint indices_vbo_id;
        };
        
    }//!namespace util
    
}//!namespace sv



#endif /* __GL_PARTICLES_H__ */
