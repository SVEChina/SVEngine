//
// SVglTFio.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SVglTFio_h
#define SVglTFio_h
#include "../base/SVGBase.h"
#include "SVglTF.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;
namespace sv{
    namespace util{
        class SVglTFio : public SVGBase{
        public:
            SVglTFio(SVInst *_app);
            
            ~SVglTFio();
            
            svgltf_scene* load_scene(cptr8 _filename, bool _load_textures = true,
                                     bool _skip_missing = true);
        protected:
            svgltf_scene* _load_gltf_scene(cptr8 _filename, bool _load_textures = true,
                                           bool _skip_missing = true);
            
            json _load_json(cptr8 _filename);
            
            bool _gltf_to_scene(svgltf_scene* _scene, const json& _gltf, cptr8 _dirname);
            
            std::vector<Byte> _load_binary(cptr8 filename);
            
            cptr8 _load_text(cptr8 filename);
            
            // Normalize path delimiters.
            cptr8 _normalize_path(cptr8 _filename);
            // Get directory name (not including '/').
            cptr8 _get_dirname(cptr8 _filename);
            // Get extension (not including '.').
            cptr8 _get_extension(cptr8 _filename);
            // Get filename without directory.
            cptr8 _get_filename(cptr8 _filename);
            // Replace extension.
            cptr8 _replace_extension(cptr8 _filename, cptr8 _ext);
            // Check if a file can be opened for reading.
            bool _exists_file(cptr8 _filename);
        };
    }// namespace util
}// namespace sv
#endif /* SVglTFio_h */
