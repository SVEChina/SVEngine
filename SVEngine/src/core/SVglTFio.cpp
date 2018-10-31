//
// SVglTFio.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVglTFio.h"
#include "json.hpp"
#include "../base/SVMap.h"
#ifndef _WIN32
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
#endif

#ifndef __clang_analyzer__

//#define STB_IMAGE_IMPLEMENTATION
//#include "ext/stb_image.h"
//
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "ext/stb_image_write.h"
//
//#define STB_IMAGE_RESIZE_IMPLEMENTATION
//#include "ext/stb_image_resize.h"
//
//#define TINYEXR_IMPLEMENTATION
//#include "ext/tinyexr.h"

#endif

#ifndef _WIN32
#pragma GCC diagnostic pop
#endif
namespace sv{
    namespace util{
        // -----------------------------------------------------------------------------
        // IMPLEMENTATION OF PATH UTILITIES
        // -----------------------------------------------------------------------------
        cptr8 normalize_path(cptr8 _filename) {
            SVString filename = _filename;
            for (s32 i = 0; i<filename.size(); i++) {
                c8 c = filename[i];
                if (c == '\\') c = '/';
            }
            if (filename.size() > 1 && filename[0] == '/' && filename[1] == '/') {
                SV_LOG_ERROR("SVglTFio Error:absolute paths are not supported");
                return _filename;
            }
            if (filename.size() > 3 && filename[1] == ':' && filename[2] == '/' &&
                filename[3] == '/') {
                SV_LOG_ERROR("SVglTFio Error:absolute paths are not supported");
                return _filename;
            }
            auto pos = (size_t)0;
            while ((pos = filename.find("//")) != -1)
                filename = SVString::substr(filename.c_str(), 0, pos) + SVString::substr(filename.c_str(), pos + 1);
            return filename.c_str();
        }
        
        // Get directory name (not including '/').
        cptr8 get_dirname(cptr8 _filename) {
            SVString filename = normalize_path(_filename);
            auto pos      = filename.rfind('/');
            if (pos == -1) return "";
            return SVString::substr(filename.c_str(), 0, pos).c_str();
        }
        
        // Get extension (not including '.').
        cptr8 get_extension(cptr8 _filename) {
            SVString filename = normalize_path(_filename);
            auto pos      = filename.rfind('.');
            if (pos == -1) return "";
            return SVString::substr(filename.c_str(), pos + 1).c_str();
        }
        
        // Get filename without directory.
        cptr8 get_filename(cptr8 _filename) {
            SVString filename = normalize_path(_filename);
            auto pos      = filename.rfind('/');
            if (pos == -1) return "";
            return SVString::substr(filename.c_str(), pos + 1).c_str();
        }
        
        // Replace extension.
        cptr8 replace_extension(cptr8 _filename, cptr8 _ext) {
            SVString filename = normalize_path(_filename);
            SVString ext      = normalize_path(_ext);
            if (ext[0] == '.') ext = SVString::substr(ext.c_str(), 1);
            auto pos = filename.rfind('.');
            if (pos == -1) return filename.c_str();
            return SVString::substr(filename.c_str(), 0, pos) + "." + ext;
        }
        
        // Check if a file can be opened for reading.
        bool exists_file(cptr8 _filename) {
            auto f = fopen(_filename, "r");
            if (!f) return false;
            fclose(f);
            return true;
        }
        
        
        // -----------------------------------------------------------------------------
        // IMPLEMENTATION OF FILE READING
        // -----------------------------------------------------------------------------
        // log io error
        template <typename... Args>
        void log_io_error(cptr8 _fmt, const Args&... _args) {
            //            SV_LOG_ERROR(_fmt, _args...);
        }
        // File stream wrapper
        struct file_stream {
            SVString filename = "";
            SVString mode     = "";
            FILE*  fs       = nullptr;
            
            file_stream()                   = default;
            file_stream(const file_stream&) = delete;
            file_stream& operator=(const file_stream&) = delete;
            file_stream(file_stream&&)                 = default;
            file_stream& operator=(file_stream&&) = default;
            
            ~file_stream() {
                if (fs) {
                    fclose(fs);
                    fs = nullptr;
                }
            }
            
            operator bool() const { return fs; }
        };
        
        // Opens a file
        file_stream open(cptr8 _filename, cptr8 _mode) {
            auto fs = fopen(_filename, _mode);
            if (!fs) {
                log_io_error("SVglTFio Error:cannot open {}", _filename);
                return {};
            }
            return {_filename, _mode, fs};
        }
        
        // Close a file
        bool close(file_stream& fs) {
            if (!fs) {
                log_io_error("SVglTFio Error:cannot close {}", fs.filename.c_str());
                return false;
            }
            fclose(fs.fs);
            fs.fs = nullptr;
            return true;
        }
        
        // Gets the length of a file
        size_t get_length(file_stream& fs) {
            if (!fs) return 0;
            fseek(fs.fs, 0, SEEK_END);
            auto fsize = ftell(fs.fs);
            fseek(fs.fs, 0, SEEK_SET);
            return fsize;
        }
        
        // Print to file
        bool write_text(file_stream& _fs, cptr8 _str) {
            if (!_fs) return false;
            if (fprintf(_fs.fs, "%s", _str) < 0) {
                log_io_error("SVglTFio Error:cannot write to {}", _fs.filename.c_str());
                return false;
            }
            return true;
        }
        
        // Write to file
        template <typename T>
        bool write_value(file_stream& fs, const T& val) {
            if (!fs) return false;
            if (fwrite(&val, sizeof(T), 1, fs.fs) != 1) {
                log_io_error("SVglTFios Error:cannot write to {}", fs.filename.c_str());
                return false;
            }
            return true;
        }
        
        // Write to file
        template <typename T>
        bool write_values(file_stream& fs, const std::vector<T>& vals) {
            if (!fs) return false;
            if (fwrite(vals.data(), sizeof(T), vals.size(), fs.fs) != vals.size()) {
                log_io_error("SVglTFios Error:cannot write to {}", fs.filename.c_str());
                return false;
            }
            return true;
        }
        
        // Print shortcut
        template <typename... Args>
        bool print(file_stream& _fs, cptr8 _fmt, const Args&... _args) {
            if (!_fs) return false;
            return write_text(_fs, format(_fmt, _args...));
        }
        
        // Read binary data to fill the whole buffer
        bool read_line(file_stream& _fs, c8* _val) {
            if (!_fs) return false;
            // TODO: make lkne as large as possible
            _val = "";
            c8 buf[4096];
            if (!fgets(buf, 4096, _fs.fs)) return false;
            _val = buf;
            return true;
        }
        
        // Read binary data to fill the whole buffer
        template <typename T>
        bool read_value(file_stream& _fs, T& _val) {
            if (!_fs) return false;
            if (fread(&_val, sizeof(T), 1, _fs.fs) != 1) {
                log_io_error("cannot read from {}", _fs.filename);
                return false;
            }
            return true;
        }
        
        // Read binary data to fill the whole buffer
        template <typename T>
        bool read_values(file_stream& _fs, std::vector<T>& _vals) {
            if (!_fs) return false;
            if (fread(_vals.data(), sizeof(T), _vals.size(), _fs.fs) != _vals.size()) {
                log_io_error("cannot read from {}", _fs.filename);
                return false;
            }
            return true;
        }
        
        // Load a text file
        cptr8 load_text(cptr8 _filename) {
            // https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
            auto fs = open(_filename, "rb");
            if (!fs) return {};
            auto buf = std::vector<char>(get_length(fs));
            if (!read_values(fs, buf)) return {};
            SVString t_buf = "";
            for (s32 i=0; i<buf.size(); i++) {
                t_buf.append(buf[i]);
            }
            //这改的会不会有问题呢，注意下！！！！！！！！！！！！！！！！！！！！！！
            return t_buf.c_str();
        }
        
        // Save a text file
        bool save_text(cptr8 _filename, cptr8 _str) {
            auto fs = open(_filename, "wt");
            if (!fs) return false;
            if (!write_text(fs, _str)) return false;
            return true;
        }
        
        // Load a binary file
        std::vector<Byte> load_binary(cptr8 _filename) {
            // https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
            auto fs = open(_filename, "rb");
            if (!fs) return {};
            auto data = std::vector<Byte>(get_length(fs));
            if (!read_values(fs, data)) return {};
            return data;
        }
        
        // Save a binary file
        bool save_binary(cptr8 _filename, const std::vector<Byte>& _data) {
            auto fs = open(_filename, "wb");
            if (!fs) return false;
            if (!write_values(fs, _data)) return false;
            return true;
        }
        
        
        // -----------------------------------------------------------------------------
        // JSON UTILITIES
        // -----------------------------------------------------------------------------
        
        
        // Json alias
        using json = nlohmann::json;
        
        // Load a JSON object
        json load_json(cptr8 _filename) {
            SVString texture = load_text(_filename);
            if (texture.empty()) return {};
            //            try {
            //                return json::parse(texture.get(0), texture.get(texture.size() - 1));
            //            } catch (...) {
            //                log_io_error("could not parse json {}", _filename);
            //                return {};
            //            }
            return {};
        }
        
        // Save a JSON object
        bool save_json(cptr8 _filename, const json& _js) {
            auto str = "";
            //            try {
            //                str = _js.dump(4);
            //            } catch (...) {
            //                log_io_error("could not dump json {}", _filename);
            //                return false;
            //            }
            return save_text(_filename, str);
        }
        
        // -----------------------------------------------------------------------------
        // GENERIC IMAGE LOADING
        // -----------------------------------------------------------------------------
        
        
        // Load a scene
        svgltf_scene* load_scene(
                                 cptr8 _filename, bool _load_textures, bool _skip_missing) {
            cptr8 ext = get_extension(_filename);
            //        if (strcmp(ext, "json") == 0  || strcmp(ext, "JSON") == 0) {
            //            return load_json_scene(_filename, _load_textures, _skip_missing);
            //        } else
            if (strcmp(ext, "gltf") == 0 || strcmp(ext, "GLTF") == 0) {
                return load_gltf_scene(_filename, _load_textures, _skip_missing);
            }else {
                log_io_error("unsupported scene format {}", ext);
                return nullptr;
            }
        }
        
        // Save a scene
        bool save_scene(cptr8 _filename, const svgltf_scene* _scene,
                        bool _save_textures, bool _skip_missing) {
            cptr8 ext = get_extension(_filename);
            //        if (strcmp(ext, "json") == 0  || strcmp(ext, "JSON") == 0) {
            //            return save_json_scene(_filename, _scene, _save_textures, _skip_missing);
            //        } else
//            if (strcmp(ext, "gltf") == 0 || strcmp(ext, "GLTF") == 0) {
//                return save_gltf_scene(_filename, _scene, _save_textures, _skip_missing);
//            }else {
//                log_io_error("unsupported scene format {}", ext);
//                return false;
//            }
            return false;
        }
        
        bool load_scene_textures(svgltf_scene* _scene, cptr8 _dirname,
                                 bool _skip_missing, bool _assign_opacity) {
//            // load images
//            for (auto texture : _scene->textures) {
//                if (texture->filename == "" || !texture->hdr_image.pixels.empty() ||
//                    !texture->ldr_image.pixels.empty())
//                    continue;
//                auto filename = normalize_path(dirname + "/" + texture->filename);
//                if (is_hdr_filename(filename)) {
//                    texture->hdr_image = load_image4f_nolog(filename);
//                } else {
//                    texture->ldr_image = load_image4b_nolog(filename);
//                }
//                if (texture->hdr_image.pixels.empty() &&
//                    texture->ldr_image.pixels.empty()) {
//                    if (!skip_missing) return false;
//                }
//            }
//
//            // load volumes
//            for (auto texture : scene->voltextures) {
//                if (texture->filename == "" || !texture->volume_data.voxels.empty())
//                    continue;
//                auto filename = normalize_path(dirname + "/" + texture->filename);
//                texture->volume_data = load_volume1f_nolog(filename);
//                if (texture->volume_data.voxels.empty()) {
//                    if (!skip_missing) return false;
//                }
//            }
//
//            // assign opacity texture if needed
//            if (assign_opacity) {
//                auto has_opacity = unordered_map<yocto_texture*, bool>();
//                for (auto& texture : scene->textures) {
//                    has_opacity[texture] = false;
//                    for (auto& p : texture->hdr_image.pixels) {
//                        if (p.w < 0.999f) {
//                            has_opacity[texture] = true;
//                            break;
//                        }
//                    }
//                    for (auto& p : texture->ldr_image.pixels) {
//                        if (p.w < 255) {
//                            has_opacity[texture] = true;
//                            break;
//                        }
//                    }
//                }
//                for (auto& mat : scene->materials) {
//                    if (mat->diffuse_texture && !mat->opacity_texture &&
//                        has_opacity.at(mat->diffuse_texture))
//                        mat->opacity_texture = mat->diffuse_texture;
//                }
//            }
            
            // done
            return true;
        }
        
        // helper to save textures
        bool save_scene_textures(const svgltf_scene* _scene, cptr8 _dirname, bool _skip_missing) {
            /*
             // save images
             for (auto texture : scene->textures) {
             if (texture->hdr_image.pixels.empty() && texture->ldr_image.pixels.empty())
             continue;
             auto filename = normalize_path(dirname + "/" + texture->filename);
             if (is_hdr_filename(filename)) {
             if (!save_image4f_nolog(filename, texture->hdr_image)) {
             if (!skip_missing) return false;
             }
             } else {
             if (!save_image4b_nolog(filename, texture->ldr_image)) {
             if (!skip_missing) return false;
             }
             }
             }
             
             // save volumes
             for (auto texture : scene->voltextures) {
             if (texture->volume_data.voxels.empty()) continue;
             auto filename = normalize_path(dirname + "/" + texture->filename);
             if (!save_volume1f_nolog(filename, texture->volume_data)) {
             if (!skip_missing) return false;
             }
             }
             */
            // done
            return true;
        }
        
        
        // -----------------------------------------------------------------------------
        // IO UTILITIES
        // -----------------------------------------------------------------------------
        // Encode in base64
        string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
            static const string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";
            
            string        ret;
            int           i = 0;
            int           j = 0;
            unsigned char char_array_3[3];
            unsigned char char_array_4[4];
            
            while (in_len--) {
                char_array_3[i++] = *(bytes_to_encode++);
                if (i == 3) {
                    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
                    ((char_array_3[1] & 0xf0) >> 4);
                    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
                    ((char_array_3[2] & 0xc0) >> 6);
                    char_array_4[3] = char_array_3[2] & 0x3f;
                    
                    for (i = 0; (i < 4); i++) ret += base64_chars[char_array_4[i]];
                    i = 0;
                }
            }
            
            if (i) {
                for (j = i; j < 3; j++) char_array_3[j] = '\0';
                
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
                ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
                ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;
                
                for (j = 0; (j < i + 1); j++) ret += base64_chars[char_array_4[j]];
                
                while ((i++ < 3)) ret += '=';
            }
            
            return ret;
        }
        
        // Decode from base64
        cptr8 base64_decode(cptr8 _encoded_string) {
            SVString encoded_string = _encoded_string;
            static const SVString base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";
            
            auto is_base64 = [](unsigned char c) -> bool {
                return (isalnum(c) || (c == '+') || (c == '/'));
            };
            
            int           in_len = (int)encoded_string.size();
            int           i      = 0;
            int           j      = 0;
            int           in_    = 0;
            unsigned char char_array_4[4], char_array_3[3];
            SVString        ret;
            
            while (in_len-- && (encoded_string[in_] != '=') &&
                   is_base64(encoded_string[in_])) {
                char_array_4[i++] = encoded_string[in_];
                in_++;
                if (i == 4) {
                    for (i = 0; i < 4; i++)
                        char_array_4[i] = base64_chars.find(char_array_4[i]);
                    
                    char_array_3[0] = (char_array_4[0] << 2) +
                    ((char_array_4[1] & 0x30) >> 4);
                    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) +
                    ((char_array_4[2] & 0x3c) >> 2);
                    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
                    
                    for (i = 0; (i < 3); i++) ret += char_array_3[i];
                    i = 0;
                }
            }
            
            if (i) {
                for (j = i; j < 4; j++) char_array_4[j] = 0;
                
                for (j = 0; j < 4; j++)
                    char_array_4[j] = base64_chars.find(char_array_4[j]);
                
                char_array_3[0] = (char_array_4[0] << 2) +
                ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) +
                ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
                
                for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
            }
            
            return ret;
        }
        
    // -----------------------------------------------------------------------------
    // GLTF CONVESION
    // -----------------------------------------------------------------------------
    
    
    static bool startswith(cptr8 _str, cptr8 _substr) {
        if (strlen(_str) < strlen(_substr)) return false;
        for (auto i = 0; i < strlen(_substr); i++)
            if (_str[i] != _substr[i]) return false;
        return true;
    }
    
    // convert gltf to scene
    bool gltf_to_scene(svgltf_scene* _scene, const json& _gltf, cptr8 _dirname) {
        // convert textures
        if (_gltf.count("images")) {
            for (auto iid = 0; iid < _gltf.at("images").size(); iid++) {
                auto& gimg        = _gltf.at("images").at(iid);
                auto  texture     = new svgltf_texture();
//                texture->name     = gimg.value("name", ""s);
//                texture->filename = (startswith(gimg.value("uri", ""s), "data:")) ?
//                SVString("[glTF-inline].png") :
//                gimg.value("uri", ""s);
                _scene->textures.push_back(texture);
            }
        }
        
        // load buffers
        auto bmap = std::vector<std::vector<Byte>>();
        if (_gltf.count("buffers")) {
            bmap.resize(_gltf.at("buffers").size());
            for (auto bid = 0; bid < _gltf.at("buffers").size(); bid++) {
                auto& gbuf = _gltf.at("buffers").at(bid);
                auto& data = bmap.at(bid);
//                cptr8  uri  = gbuf.value("uri", ""s);
                SVString  uri  = "";
                if (uri == "") continue;
                if (startswith(uri, "data:")) {
                    // assume it is base64 and find ','
                    auto pos = uri.find(',');
                    if (pos == -1) {
                        return false;
                    }
                    // decode
                    SVString data_char = base64_decode(SVString::substr(uri.c_str(), pos + 1));
                    data = std::vector<u8>((u8*)data_char.c_str(),
                                                 (u8*)data_char.c_str() + data_char.size());
                } else {
                    auto filename = normalize_path((SVString(_dirname) + "/" + uri).c_str());
                    data          = load_binary(filename);
                    if (data.empty()) return false;
                }
                if (gbuf.value("byteLength", -1) != data.size()) {
                    return false;
                }
            }
        }
        
        // add a texture
        auto add_texture = [_scene, &_gltf](const json& ginfo, bool force_linear) {
            if (!_gltf.count("images") || !_gltf.count("textures"))
                return (svgltf_texture*)nullptr;
            if (ginfo.is_null() || ginfo.empty()) return (svgltf_texture*)nullptr;
            if (ginfo.value("index", -1) < 0) return (svgltf_texture*)nullptr;
            auto& gtxt = _gltf.at("textures").at(ginfo.value("index", -1));
            if (gtxt.empty() || gtxt.value("source", -1) < 0)
                return (svgltf_texture*)nullptr;
            auto texture = _scene->textures.at(gtxt.value("source", -1));
            if (!_gltf.count("samplers") || gtxt.value("sampler", -1) < 0)
                return texture;
            auto& gsmp = _gltf.at("samplers").at(gtxt.value("sampler", -1));
            texture->clamp_to_edge = gsmp.value("wrapS", ""s) == "ClampToEdge" ||
            gsmp.value("wrapT", ""s) == "ClampToEdge";
            texture->height_scale = gsmp.value("scale", 1.0f) *
            gsmp.value("strength", 1.0f);
//            texture->ldr_as_linear = force_linear ||
//            is_hdr_filename(texture->filename);
            return texture;
        };
        
        // convert materials
        if (_gltf.count("materials")) {
            for (auto mid = 0; mid < _gltf.at("materials").size(); mid++) {
                auto& gmat    = _gltf.at("materials").at(mid);
                auto  mat     = new svgltf_material();
//                mat->name     = gmat.value("name", "");
//                mat->emission = gmat.value("emissiveFactor", zero3f);
                if (gmat.count("emissiveTexture"))
                    mat->emission_texture = add_texture(
                                                        gmat.at("emissiveTexture"), false);
                if (gmat.count("extensions") &&
                    gmat.at("extensions").count("KHR_materials_pbrSpecularGlossiness")) {
                    mat->base_metallic = false;
                    mat->gltf_textures = true;
                    auto& gsg          = gmat.at("extensions")
                    .at("KHR_materials_pbrSpecularGlossiness");
//                    auto kb        = gsg.value("diffuseFactor", FVec4(1, 1, 1, 1));
//                    mat->diffuse   = {kb.x, kb.y, kb.z};
//                    mat->opacity   = kb.w;
//                    mat->specular  = gsg.value("specularFactor", vec3f{1, 1, 1});
                    mat->roughness = 1 - gsg.value("glossinessFactor", 1.0f);
                    if (gsg.count("diffuseTexture"))
                        mat->diffuse_texture = add_texture(
                                                           gsg.at("diffuseTexture"), false);
                    if (gsg.count("specularGlossinessTexture"))
                        mat->specular_texture = add_texture(
                                                            gsg.at("specularGlossinessTexture"), false);
                    mat->roughness_texture = mat->specular_texture;
                } else if (gmat.count("pbrMetallicRoughness")) {
                    mat->base_metallic = true;
                    mat->gltf_textures = true;
                    auto& gmr          = gmat.at("pbrMetallicRoughness");
//                    auto  kb      = gmr.value("baseColorFactor", vec4f{1, 1, 1, 1});
//                    mat->diffuse  = {kb.x, kb.y, kb.z};
//                    mat->opacity  = kb.w;
                    auto km       = gmr.value("metallicFactor", 1.0f);
                    mat->specular = {km, km, km};
                    mat->roughness = gmr.value("roughnessFactor", 1.0f);
                    if (gmr.count("baseColorTexture"))
                        mat->diffuse_texture = add_texture(
                                                           gmr.at("baseColorTexture"), false);
                    if (gmr.count("metallicRoughnessTexture"))
                        mat->specular_texture = add_texture(
                                                            gmr.at("metallicRoughnessTexture"), true);
                    mat->roughness_texture = mat->specular_texture;
                }
                if (gmat.count("occlusionTexture"))
                    mat->occlusion_texture = add_texture(
                                                         gmat.at("occlusionTexture"), true);
                if (gmat.count("normalTexture"))
                    mat->normal_texture = add_texture(gmat.at("normalTexture"), true);
                mat->double_sided = gmat.value("doubleSided", true);
                _scene->materials.push_back(mat);
            }
        }
        
        // get values from accessors
        auto accessor_values =
        [&_gltf, &bmap](const json& gacc,
                       bool normalize = false) -> vector<std::array<double, 4>> {
            auto gview  = _gltf.at("bufferViews").at(gacc.value("bufferView", -1));
            auto data   = bmap.at(gview.value("buffer", -1)).data();
            auto offset = gacc.value("byteOffset", 0) + gview.value("byteOffset", 0);
            auto stride      = gview.value("byteStride", 0);
            auto compTypeNum = gacc.value("componentType", 5123);
            auto count       = gacc.value("count", -1);
            auto type        = gacc.value("type", ""s);
            auto ncomp       = 0;
            if (type == "SCALAR") ncomp = 1;
            if (type == "VEC2") ncomp = 2;
            if (type == "VEC3") ncomp = 3;
            if (type == "VEC4") ncomp = 4;
            auto compSize = 1;
            if (compTypeNum == 5122 || compTypeNum == 5123) {
                compSize = 2;
            }
            if (compTypeNum == 5124 || compTypeNum == 5125 || compTypeNum == 5126) {
                compSize = 4;
            }
            if (!stride) stride = compSize * ncomp;
            auto vals = vector<std::array<double, 4>>(count, {{0.0, 0.0, 0.0, 1.0}});
            for (auto i = 0; i < count; i++) {
                auto d = data + offset + i * stride;
                for (auto c = 0; c < ncomp; c++) {
                    if (compTypeNum == 5120) {  // char
                        vals[i][c] = (double)(*(char*)d);
                        if (normalize) vals[i][c] /= SCHAR_MAX;
                    } else if (compTypeNum == 5121) {  // byte
                        vals[i][c] = (double)(*(Byte*)d);
                        if (normalize) vals[i][c] /= UCHAR_MAX;
                    } else if (compTypeNum == 5122) {  // short
                        vals[i][c] = (double)(*(short*)d);
                        if (normalize) vals[i][c] /= SHRT_MAX;
                    } else if (compTypeNum == 5123) {  // unsigned short
                        vals[i][c] = (double)(*(unsigned short*)d);
                        if (normalize) vals[i][c] /= USHRT_MAX;
                    } else if (compTypeNum == 5124) {  // int
                        vals[i][c] = (double)(*(int*)d);
                        if (normalize) vals[i][c] /= INT_MAX;
                    } else if (compTypeNum == 5125) {  // unsigned int
                        vals[i][c] = (double)(*(unsigned int*)d);
                        if (normalize) vals[i][c] /= UINT_MAX;
                    } else if (compTypeNum == 5126) {  // float
                        vals[i][c] = (*(float*)d);
                    }
                    d += compSize;
                }
            }
            return vals;
        };
        
        // convert meshes
        auto meshes = std::vector<std::vector<svgltf_shape*>>();
        if (_gltf.count("meshes")) {
            for (auto mid = 0; mid < _gltf.at("meshes").size(); mid++) {
                auto& gmesh = _gltf.at("meshes").at(mid);
                meshes.push_back({});
                auto sid = 0;
                for (auto& gprim : gmesh.value("primitives", json::array())) {
                    if (!gprim.count("attributes")) continue;
                    auto shape  = new svgltf_shape();
//                    shape->name = gmesh.value("name", ""s) +
//                    ((sid) ? std::to_string(sid) : string());
//                    sid++;
                    for (json::iterator gattr_it = gprim.at("attributes").begin();
                         gattr_it != gprim.at("attributes").end(); ++gattr_it) {
                        auto  semantic = gattr_it.key();
                        auto& gacc     = _gltf.at("accessors")
                        .at(gattr_it.value().get<int>());
                        auto vals = accessor_values(gacc);
                        if (semantic == "POSITION") {
                            shape->positions.reserve(vals.size());
                            for (auto i = 0; i < vals.size(); i++)
                                shape->positions.push_back({(float)vals[i][0],
                                    (float)vals[i][1], (float)vals[i][2]});
                        } else if (semantic == "NORMAL") {
                            shape->normals.reserve(vals.size());
                            for (auto i = 0; i < vals.size(); i++)
                                shape->normals.push_back({(float)vals[i][0],
                                    (float)vals[i][1], (float)vals[i][2]});
                        } else if (semantic == "TEXCOORD" ||
                                   semantic == "TEXCOORD_0") {
                            shape->texturecoords.reserve(vals.size());
                            for (auto i = 0; i < vals.size(); i++)
                                shape->texturecoords.push_back(
                                                               {(float)vals[i][0], (float)vals[i][1]});
                        } else if (semantic == "COLOR" || semantic == "COLOR_0") {
                            shape->colors.reserve(vals.size());
                            for (auto i = 0; i < vals.size(); i++)
                                shape->colors.push_back(
                                                        {(float)vals[i][0], (float)vals[i][1],
                                                            (float)vals[i][2], (float)vals[i][3]});
                        } else if (semantic == "TANGENT") {
                            shape->tangentspaces.reserve(vals.size());
                            for (auto i = 0; i < vals.size(); i++)
                                shape->tangentspaces.push_back(
                                                               {(float)vals[i][0], (float)vals[i][1],
                                                                   (float)vals[i][2], (float)vals[i][3]});
                            for (auto& t : shape->tangentspaces) t.w = -t.w;
                        } else if (semantic == "RADIUS") {
                            shape->radius.reserve(vals.size());
                            for (auto i = 0; i < vals.size(); i++)
                                shape->radius.push_back((float)vals[i][0]);
                        } else {
                            // ignore
                        }
                    }
                    // indices
                    auto mode = gprim.value("mode", 4);
                    if (!gprim.count("indices")) {
                        if (mode == 4) {
                            // triangles
                            shape->triangles.reserve(shape->positions.size() / 3);
                            for (auto i = 0; i < shape->positions.size() / 3; i++)
                                shape->triangles.push_back(
                                                           FVec3(i * 3 + 0, i * 3 + 1, i * 3 + 2));
                        } else if (mode == 6) {
                            // triangle fan
                            shape->triangles.reserve(shape->positions.size() - 2);
                            for (auto i = 2; i < shape->positions.size(); i++)
                                shape->triangles.push_back(FVec3(0, i - 1, i));
                        } else if (mode == 5) {
                            // triangle strip
                            shape->triangles.reserve(shape->positions.size() - 2);
                            for (auto i = 2; i < shape->positions.size(); i++)
                                shape->triangles.push_back(FVec3(i - 2, i - 1, i));
                        } else if (mode == 1) {
                            // lines
                            shape->lines.reserve(shape->positions.size() / 2);
                            for (auto i = 0; i < shape->positions.size() / 2; i++)
                                shape->lines.push_back(FVec2(i * 2 + 0, i * 2 + 1));
                        } else if (mode == 2) {
                            // line loop
                            shape->lines.reserve(shape->positions.size());
                            for (auto i = 1; i < shape->positions.size(); i++)
                                shape->lines.push_back(FVec2(i - 1, i));
                            shape->lines.back() = FVec2(
                                (s32)shape->positions.size() - 1, 0);
                        } else if (mode == 3) {
                            // line strip
                            shape->lines.reserve(shape->positions.size() - 1);
                            for (auto i = 1; i < shape->positions.size(); i++)
                                shape->lines.push_back(FVec2(i - 1, i));
                        } else if (mode == -1 || mode == 0) {
                            // points
                            SV_LOG_ERROR("SVglTFio Error:points not supported\n");
                        } else {
//                            log_error("unknown primitive type");
                        }
                    } else {
                        auto indices = accessor_values(
                                                       _gltf.at("accessors").at(gprim.value("indices", -1)),
                                                       false);
                        if (mode == 4) {
                            // triangles
                            shape->triangles.reserve(indices.size());
                            for (auto i = 0; i < indices.size() / 3; i++)
                                shape->triangles.push_back(
                                                           FVec3((s32)indices[i * 3 + 0][0],
                                                               (s32)indices[i * 3 + 1][0],
                                                               (s32)indices[i * 3 + 2][0]));
                        } else if (mode == 6) {
                            // triangle fan
                            shape->triangles.reserve(indices.size() - 2);
                            for (auto i = 2; i < indices.size(); i++)
                                shape->triangles.push_back(FVec3((s32)indices[0][0],
                                    (s32)indices[i - 1][0], (int)indices[i][0]));
                        } else if (mode == 5) {
                            // triangle strip
                            shape->triangles.reserve(indices.size() - 2);
                            for (auto i = 2; i < indices.size(); i++)
                                shape->triangles.push_back(FVec3((s32)indices[i - 2][0],
                                    (s32)indices[i - 1][0], (s32)indices[i][0]));
                        } else if (mode == 1) {
                            // lines
                            shape->lines.reserve(indices.size() / 2);
                            for (auto i = 0; i < indices.size() / 2; i++)
                                shape->lines.push_back(FVec2((s32)indices[i * 2 + 0][0],
                                    (s32)indices[i * 2 + 1][0]));
                        } else if (mode == 2) {
                            // line loop
                            shape->lines.reserve(indices.size());
                            for (auto i = 1; i < indices.size(); i++)
                                shape->lines.push_back(
                                                       FVec2((s32)indices[i - 1][0], (s32)indices[i][0]));
                            shape->lines.back() = FVec2(
                                (s32)indices[indices.size() - 1][0],
                                (s32)indices[0][0]);
                        } else if (mode == 3) {
                            // line strip
                            shape->lines.reserve(indices.size() - 1);
                            for (auto i = 1; i < indices.size(); i++)
                                shape->lines.push_back(
                                                       FVec2((s32)indices[i - 1][0], (s32)indices[i][0]));
                        } else if (mode == -1 || mode == 0) {
                            // points
                            SV_LOG_ERROR("SVglTFio Error:points not supported\n");
                        } else {
//                            log_error("unknown primitive type");
                        }
                    }
                    shape->material = (gprim.count("material")) ?
                    _scene->materials.at(
                                        gprim.value("material", -1)) :
                    nullptr;
                    meshes.back().push_back(shape);
                    _scene->shapes.push_back(shape);
                }
            }
        }
        
        // convert cameras
        if (_gltf.count("cameras")) {
            for (auto cid = 0; cid < _gltf.at("cameras").size(); cid++) {
                auto& gcam           = _gltf.at("cameras").at(cid);
                auto  camera         = new svgltf_camera();
//                camera->name         = gcam.value("name", ""s);
                camera->orthographic = gcam.value("type", ""s) == "orthographic";
                if (camera->orthographic) {
                    SV_LOG_ERROR("SVglTFio Error:orthographic not supported well\n");
                    auto ortho = gcam.value("orthographic", json::object());
//                    set_camera_fovy(camera, ortho.value("ymag", 0.0f),
//                                    ortho.value("xmag", 0.0f) / ortho.value("ymag", 0.0f));
//                    camera->focus_distance = maxf;
                    camera->lens_aperture  = 0;
                } else {
                    auto persp = gcam.value("perspective", json::object());
//                    set_camera_fovy(camera, persp.value("yfov", 1.0f),
//                                    persp.value("aspectRatio", 1.0f));
//                    camera->focus_distance = maxf;
                    camera->lens_aperture  = 0;
                }
                _scene->cameras.push_back(camera);
            }
        }
        
        // convert nodes
        if (_gltf.count("nodes")) {
            for (auto nid = 0; nid < _gltf.at("nodes").size(); nid++) {
                auto& gnde = _gltf.at("nodes").at(nid);
                auto  node = new svgltf_scene_node();
//                node->name = gnde.value("name", ""s);
                if (gnde.count("camera"))
                    node->camera = _scene->cameras[gnde.value("camera", 0)];
                FVec3 zero3 = FVec3_zero;
//                node->translation = gnde.value("translation", zero3);
//                node->rotation    = gnde.value("rotation", vec4f{0, 0, 0, 1});
//                node->scale       = gnde.value("scale", vec3f{1, 1, 1});
//                node->local = mat_to_frame(gnde.value("matrix", identity_mat4f));
//                scene->nodes.push_back(node);
            }
            
            // set up parent pointers
            for (auto nid = 0; nid < _gltf.at("nodes").size(); nid++) {
                auto& gnde = _gltf.at("nodes").at(nid);
                if (!gnde.count("children")) continue;
                auto node = _scene->nodes[nid];
                for (auto& cid : gnde.at("children"))
                    _scene->nodes[cid.get<int>()]->parent = node;
            }
            
            // set up instances
            for (auto nid = 0; nid < _gltf.at("nodes").size(); nid++) {
                auto& gnde = _gltf.at("nodes").at(nid);
                if (!gnde.count("mesh")) continue;
                auto  node = _scene->nodes[nid];
                auto& shps = meshes.at(gnde.value("mesh", 0));
                if (shps.empty()) continue;
                if (shps.size() == 1) {
                    node->instance        = new svgltf_instance();
                    node->instance->name  = node->name;
                    node->instance->shape = shps[0];
                    _scene->instances.push_back(node->instance);
                } else {
                    for (auto shape : shps) {
                        auto child             = new svgltf_scene_node();
                        child->name            = node->name + "_" + shape->name;
                        child->parent          = node;
                        child->instance        = new svgltf_instance();
                        child->instance->name  = child->name;
                        child->instance->shape = shape;
                        _scene->instances.push_back(child->instance);
                        _scene->nodes.push_back(child);
                    }
                }
            }
        }
        // convert animations
        if (_gltf.count("animations")) {
            for (auto& ganm : _gltf.at("animations")) {
                auto aid         = 0;
                SVMap<SVString, s32> sampler_map;
                for (auto& gchannel : ganm.at("channels")) {
                    auto path_ = gchannel.at("target").at("path").get<string>();
                    auto path  = -1;
                    if (path_ == "translation") path = 0;
                    if (path_ == "rotation") path = 1;
                    if (path_ == "scale") path = 2;
                    if (path_ == "weights") path = 3;
                    SVString key = (std::to_string(gchannel.at("sampler").get<s32>()) + "_" + std::to_string(path)).c_str();
                    if (sampler_map.find(key) == sampler_map.end()) {
                        auto& gsampler = ganm.at("samplers")
                        .at(gchannel.at("sampler").get<s32>());
                        auto animation  = new svgltf_animation();
//                        animation->name = (ganm.count("name") ?
//                                           ganm.value("name", ""s) :
//                                           "anim") +
//                        std::to_string(aid++);
//                        animation->animation_group = ganm.value("name", ""s);
                        auto input_view            = accessor_values(_gltf.at("accessors").at(gsampler.value("input", -1)));
                        animation->keyframes_times.resize(input_view.size());
                        for (auto i = 0; i < input_view.size(); i++)
                            animation->keyframes_times[i] = input_view[i][0];
                        auto type = gsampler.value("interpolation", "LINEAR");
                        if (type == "LINEAR")
                            animation->interpolation_type = svgltf_interpolation_type::linear;
                        if (type == "STEP")
                            animation->interpolation_type = svgltf_interpolation_type::step;
                        if (type == "CUBICSPLINE")
                            animation->interpolation_type = svgltf_interpolation_type::bezier;
                        auto output_view = accessor_values(
                                                           _gltf.at("accessors").at(gsampler.value("output", -1)));
                        switch (path) {
                            case 0: {  // translation
                                animation->translation_keyframes.reserve(
                                                                         output_view.size());
                                for (auto i = 0; i < output_view.size(); i++)
                                    animation->translation_keyframes.push_back(
                                                                               {(f32)output_view[i][0],
                                                                                   (f32)output_view[i][1],
                                                                                   (f32)output_view[i][2]});
                            } break;
                            case 1: {  // rotation
                                animation->rotation_keyframes.reserve(
                                                                      output_view.size());
                                for (auto i = 0; i < output_view.size(); i++)
                                    animation->rotation_keyframes.push_back(
                                                                            {(f32)output_view[i][0],
                                                                                (f32)output_view[i][1],
                                                                                (f32)output_view[i][2],
                                                                                (f32)output_view[i][3]});
                            } break;
                            case 2: {  // scale
                                animation->scale_keyframes.reserve(
                                                                   output_view.size());
                                for (auto i = 0; i < output_view.size(); i++)
                                    animation->scale_keyframes.push_back(
                                                                         {(f32)output_view[i][0],
                                                                             (f32)output_view[i][1],
                                                                             (f32)output_view[i][2]});
                            } break;
                            case 3: {  // weights
                                SV_LOG_ERROR("SVglTFio Error:weights not supported for now\n");
#if 0
                                // get a node that it refers to
                                auto ncomp = 0;
                                auto gnode = gltf->get(gchannel->target->node);
                                auto gmesh = gltf->get(gnode->mesh);
                                if (gmesh) {
                                    for (auto gshp : gmesh->primitives) {
                                        ncomp = max((s32)gshp->targets.size(), ncomp);
                                    }
                                }
                                if (ncomp) {
                                    auto values = vector<f32>();
                                    values.reserve(output_view.size());
                                    for (auto i = 0; i < output_view.size(); i++)
                                        values.push_back(output_view.get(i));
                                    animation->weights.resize(values.size() / ncomp);
                                    for (auto i = 0; i < animation->weights.size(); i++) {
                                        animation->weights[i].resize(ncomp);
                                        for (auto j = 0; j < ncomp; j++)
                                            animation->weights[i][j] = values[i * ncomp + j];
                                    }
                                }
#endif
                            } break;
                            default: { return false; }
                        }
                        key = (std::to_string(gchannel.at("sampler").get<s32>()) + "_" + std::to_string(path)).c_str();
                        sampler_map[key] = (s32)_scene->animations.size();
                        _scene->animations.push_back(animation);
                    }
                    key = (std::to_string(gchannel.at("sampler").get<s32>()) + "_" + std::to_string(path)).c_str();
                    _scene->animations[sampler_map[key]]
                    ->node_targets.push_back(_scene->nodes[(s32)gchannel.at("target").at("node").get<s32>()]);
                }
            }
        }
         
        
        return true;
    }
    
    // Load a scene
    svgltf_scene* load_gltf_scene(cptr8 _filename, bool _load_textures, bool _skip_missing) {
        // initialization
        svgltf_scene *scene = new svgltf_scene();
        
        // convert json
        auto js = load_json(_filename);
        if (js.empty()) return nullptr;
        try {
            if (!gltf_to_scene(scene, js, get_dirname(_filename)))
                return nullptr;
        } catch (...) {
            return nullptr;
        }
        
        // load textures
        auto dirname = get_dirname(_filename);
        if (_load_textures) {
            if (!load_scene_textures(scene, dirname, _skip_missing, false))
                return nullptr;
        }
        
        // fix scene
        scene->name = get_filename(_filename);
//        add_missing_cameras(scene);
//        add_missing_materials(scene);
//        add_missing_names(scene);
//        update_transforms(scene);
        
        // fix cameras
//        auto bbox = compute_scene_bounds(scene);
//        for (auto camera : scene->cameras) {
//            auto center = (bbox.min + bbox.max) / 2;
//            auto dist   = dot(-camera->frame.z, center - camera->frame.o);
//            if (dist > 0) camera->focus_distance = dist;
//        }
        
        // done
        return scene;
    }
}// namespace util
}// namespace sv

