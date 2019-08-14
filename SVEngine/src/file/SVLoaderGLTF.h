//
// SVLoaderGLTF.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_LOADER_GLTF_H
#define SV_LOADER_GLTF_H

#include "SVFileLoader.h"
#include "../base/SVPreDeclare.h"
#include "../rendercore/SVRenderDeclare.h"
#include "../node/SVNodeDeclare.h"
#include "../base/SVMap.h"
#include "../base/SVArray.h"
#include "../base/svstr.h"
#include "../base/SVMat4.h"
#include "../base/SVDataSwap.h"

namespace sv {
    
#define SVGLTF_MODE_POINTS (0)
#define SVGLTF_MODE_LINE (1)
#define SVGLTF_MODE_LINE_LOOP (2)
#define SVGLTF_MODE_TRIANGLES (4)
#define SVGLTF_MODE_TRIANGLE_STRIP (5)
#define SVGLTF_MODE_TRIANGLE_FAN (6)
    
#define SVGLTF_COMPONENT_TYPE_BYTE (5120)
#define SVGLTF_COMPONENT_TYPE_UNSIGNED_BYTE (5121)
#define SVGLTF_COMPONENT_TYPE_SHORT (5122)
#define SVGLTF_COMPONENT_TYPE_UNSIGNED_SHORT (5123)
#define SVGLTF_COMPONENT_TYPE_INT (5124)
#define SVGLTF_COMPONENT_TYPE_UNSIGNED_INT (5125)
#define SVGLTF_COMPONENT_TYPE_FLOAT (5126)
#define SVGLTF_COMPONENT_TYPE_DOUBLE (5130)
    
#define SVGLTF_TEXTURE_FILTER_NEAREST (9728)
#define SVGLTF_TEXTURE_FILTER_LINEAR (9729)
#define SVGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST (9984)
#define SVGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST (9985)
#define SVGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR (9986)
#define SVGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR (9987)
    
#define SVGLTF_TEXTURE_WRAP_REPEAT (10497)
#define SVGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE (33071)
#define SVGLTF_TEXTURE_WRAP_MIRRORED_REPEAT (33648)
    
#define SVGLTF_TARGET_ARRAY_BUFFER (34962)
#define SVGLTF_TARGET_ELEMENT_ARRAY_BUFFER (34963)
    
#define SVGLTF_TYPE_VEC2 (2)
#define SVGLTF_TYPE_VEC3 (3)
#define SVGLTF_TYPE_VEC4 (4)
#define SVGLTF_TYPE_MAT2 (32 + 2)
#define SVGLTF_TYPE_MAT3 (32 + 3)
#define SVGLTF_TYPE_MAT4 (32 + 4)
#define SVGLTF_TYPE_SCALAR (64 + 1)
#define SVGLTF_TYPE_VECTOR (64 + 4)
#define SVGLTF_TYPE_MATRIX (64 + 16)
    
    typedef enum {
        NULL_TYPE = 0,
        NUMBER_TYPE = 1,
        INT_TYPE = 2,
        BOOL_TYPE = 3,
        STRING_TYPE = 4,
        ARRAY_TYPE = 5,
        BINARY_TYPE = 6,
        OBJECT_TYPE = 7
    } Type;
    
    enum GLTFInterpolationMode{
        GLTFInterpolationModeNone = -1,
        GLTFInterpolationModeStep,
        GLTFInterpolationModeLinear,
        GLTFInterpolationModeCubic,
    };
    
    static sv_inline GLTFInterpolationMode _getInterpolationMode(SVString _interpolation){
        if (_interpolation == "LINEAR") {
            return GLTFInterpolationModeLinear;
        }else if(_interpolation == "STEP"){
            return GLTFInterpolationModeStep;
        }else if(_interpolation == "CUBICSPLINE"){
            return GLTFInterpolationModeCubic;
        }
        return GLTFInterpolationModeNone;
    }
    
    static sv_inline s32 _getCmpSize(u32 _componentType) {
        s32 t_cmpSize = 0;
        if (_componentType == SVGLTF_COMPONENT_TYPE_BYTE) {
            t_cmpSize = 1;
        } else if (_componentType == SVGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
            t_cmpSize = 1;
        } else if (_componentType == SVGLTF_COMPONENT_TYPE_SHORT) {
            t_cmpSize = 2;
        } else if (_componentType == SVGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            t_cmpSize = 2;
        } else if (_componentType == SVGLTF_COMPONENT_TYPE_INT) {
            t_cmpSize = 4;
        } else if (_componentType == SVGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
            t_cmpSize = 4;
        } else if (_componentType == SVGLTF_COMPONENT_TYPE_FLOAT) {
            t_cmpSize = 4;
        } else if (_componentType == SVGLTF_COMPONENT_TYPE_DOUBLE) {
            t_cmpSize = 8;
        }
        return t_cmpSize;
    }
    
    static sv_inline s32 _getCmpNum(u32 _ty) {
        //
        s32 t_cmpCount = 0;
        if (_ty == SVGLTF_TYPE_SCALAR) {
            t_cmpCount = 1;
        } else if (_ty == SVGLTF_TYPE_VEC2) {
            t_cmpCount = 2;
        } else if (_ty == SVGLTF_TYPE_VEC3) {
            t_cmpCount = 3;
        } else if (_ty == SVGLTF_TYPE_VEC4) {
            t_cmpCount = 4;
        } else if (_ty == SVGLTF_TYPE_MAT2) {
            t_cmpCount = 4;
        } else if (_ty == SVGLTF_TYPE_MAT3) {
            t_cmpCount = 9;
        } else if (_ty == SVGLTF_TYPE_MAT4) {
            t_cmpCount = 16;
        }
        return t_cmpCount;
    }

    /// Agregate object for representing a color
    using ColorValue = std::array<double, 4>;
    
    struct Parameter {
        bool bool_value = false;
        bool has_number_value = false;
        SVString string_value;
        SVArray<f64> number_array;
        SVMap<SVString, f64> json_double_value;
        f64 number_value = 0.0;
        //
        s32 TextureIndex() const {
            SVMap<SVString, f64>::Iterator it = json_double_value.find("index");
            if(it!=json_double_value.end()){
                return s32(it->data);
            }
            return -1;
        }
        
        f32 ParamValue(cptr8 _key) {
            SVMap<SVString, f64>::Iterator it = json_double_value.find(_key);
            if(it!=json_double_value.end()){
                return f32(it->data);
            }
            return 0.0f;
        }
        // metallicFactor or roughnessFactor
        f64 Factor() const { return number_value; }
        
        bool operator==(const Parameter &) const;
    };
    
    typedef SVMap<SVString, Parameter> ParameterMap;
    
    struct AnimationChannel {
        s32 sampler;              // required
        s32 target_node;          // required (index of the node to target)
        SVString target_path;     // required in ["translation", "rotation", "scale", "weights"]
        AnimationChannel()
        : sampler(-1)
        , target_node(-1) {
        }
        bool operator==(const AnimationChannel &) const;
    };
    
    struct AnimationSampler {
        s32 input;                 // required
        s32 output;                // required
        SVString interpolation;    // in ["LINEAR", "STEP", "CATMULLROMSPLINE", "CUBICSPLINE"], default "LINEAR"
        AnimationSampler()
        : input(-1)
        , output(-1)
        , interpolation("LINEAR") {
        }
        bool operator==(const AnimationSampler &) const;
    };
    
    struct Animation {
        SVString name;
        SVArray<AnimationChannel> channels;
        SVArray<AnimationSampler> samplers;
        bool operator==(const Animation &) const;
    };
    
    struct Skin {
        s32 inverseBindMatrices;    // required here but not in the spec
        s32 skeleton;               // The index of the node used as a skeleton root
        SVString name;              //骨架名称
        SVArray<s32> joints;        // Indices of skeleton nodes
        Skin()
        :inverseBindMatrices(-1)
        ,skeleton(-1){
        }
        bool operator==(const Skin &) const;
    };
    
    struct Sampler {
        SVString name;
        s32 minFilter;  // ["NEAREST", "LINEAR", "NEAREST_MIPMAP_LINEAR", "LINEAR_MIPMAP_NEAREST", "NEAREST_MIPMAP_LINEAR", "LINEAR_MIPMAP_LINEAR"]
        s32 magFilter;  // ["NEAREST", "LINEAR"]
        s32 wrapS;      // ["CLAMP_TO_EDGE", "MIRRORED_REPEAT", "REPEAT"], default // "REPEAT"
        s32 wrapT;      // ["CLAMP_TO_EDGE", "MIRRORED_REPEAT", "REPEAT"], default // "REPEAT"
        s32 wrapR;      // TinyGLTF extension
        
        Sampler()
        : wrapS(SVGLTF_TEXTURE_WRAP_REPEAT)
        , wrapT(SVGLTF_TEXTURE_WRAP_REPEAT) {
        }
        bool operator==(const Sampler &) const;
    };
    
    struct Image {
        SVString name;
        SVString mimeType;     // (required if no uri) ["image/jpeg", "image/png", "image/bmp", "image/gif"]
        SVString uri;          // (required if no mimeType)
        s32 width;
        s32 height;
        s32 component;
        s32 bufferView;        // (required if no uri)
        SVTexturePtr texture;

        bool as_is;
        
        Image()
        : as_is(false)
        , bufferView(-1)
        , width(-1)
        , height(-1)
        , component(-1){
        }
        bool operator==(const Image &) const;
    };
    
    struct Texture {
        SVString name;
        s32 sampler;
        s32 source;
        Texture()
        : sampler(-1)
        , source(-1) {
        }
        bool operator==(const Texture &) const;
    };
    
    // Each extension should be stored in a ParameterMap.
    // members not in the values could be included in the ParameterMap
    // to keep a single material model
    struct Material {
        SVString name;
        ParameterMap values;            // PBR metal/roughness workflow
        ParameterMap additionalValues;  // normal/occlusion/emissive values
        bool operator==(const Material &) const;
    };
    
    struct BufferView {
        SVString name;
        s32 target;         // ["ARRAY_BUFFER", "ELEMENT_ARRAY_BUFFER"]
        s32 buffer;         // Required
        s64 byteOffset;  // minimum 0, default 0
        s64 byteLength;  // required, minimum 1
        s64 byteStride;  // minimum 4, maximum 252 (multiple of 4), default 0 = understood to be tightly packed
        
        BufferView()
        : byteOffset(0)
        , byteStride(0) {
        }
        bool operator==(const BufferView &) const;
    };
    
    struct Accessor {
        SVString name;
        bool normalized;    // optinal.
        s32 bufferView;     // optional in spec but required here since sparse accessor are not supported
        s64 byteOffset;     // 偏移
        s32 componentType;  // (required) One of SVGLTF_COMPONENT_TYPE_***
        s32 type;           // (required) One of SVGLTF_TYPE_***   ..
        s64 count;          // required
        SVArray<f64> minValues;  // optional
        SVArray<f64> maxValues;  // optional
        Accessor()
        :bufferView(-1){
        }
        bool operator==(const Accessor &) const;
    };
    //
    struct PerspectiveCamera {
        f64 aspectRatio;  // min > 0
        f64 yfov;         // required. min > 0
        f64 zfar;         // min > 0
        f64 znear;        // required. min > 0
        PerspectiveCamera()
        : aspectRatio(0.0)
        , yfov(0.0)
        , zfar(0.0)  // 0 = use infinite projecton matrix
        , znear(0.0) {
        }
        bool operator==(const PerspectiveCamera &) const;
    };
    //
    struct OrthographicCamera {
        f64 xmag;   // required. must not be zero.
        f64 ymag;   // required. must not be zero.
        f64 zfar;   // required. `zfar` must be greater than `znear`.
        f64 znear;  // required
        
        OrthographicCamera()
        : xmag(0.0)
        , ymag(0.0)
        , zfar(0.0)
        , znear(0.0) {
        }
        bool operator==(const OrthographicCamera &) const;
        
    };
    //
    struct Camera {
        SVString type;  // required. "perspective" or "orthographic"
        SVString name;
        PerspectiveCamera perspective;
        OrthographicCamera orthographic;
        Camera() {
        }
        bool operator==(const Camera &) const;
    };
    //
    struct Primitive {
        SVMap<SVString, s32> attributes;
        s32 material;
        s32 indices;   // The index of the accessor that contains the indices.
        s32 mode;
        SVArray<SVMap<SVString, s32>> targets;  // array of morph targets,
        // where each target is a dict with attribues in ["POSITION, "NORMAL",
        // "TANGENT"] pointing
        // to their corresponding accessors
        Primitive()
        : material(-1)
        , indices(-1){
        }
        bool operator==(const Primitive &) const;
    };
    //
    struct Mesh {
        SVString name;
        SVArray<Primitive> primitives;
        SVArray<f64> weights;  //weights to be applied to the Morph Targets
        SVArray<SVMap<SVString, s32> > targets;
        bool operator==(const Mesh &) const;
    };
    //主体
    class Node {
    public:
        Node()
        : camera(-1)
        , skin(-1)
        , mesh(-1) {
        }
        Node(const Node &rhs) {
            camera = rhs.camera;
            name = rhs.name;
            skin = rhs.skin;
            mesh = rhs.mesh;
            children = rhs.children;
            rotation = rhs.rotation;
            scale = rhs.scale;
            translation = rhs.translation;
            matrix = rhs.matrix;
            weights = rhs.weights;
        }
        ~Node() {
        }
        bool operator==(const Node &) const;
        
        SVString name;
        s32 camera;  // the index of the camera referenced by this node
        s32 skin;
        s32 mesh;
        SVArray<s32> children;
        SVArray<f64> rotation;     // length must be 0 or 4
        SVArray<f64> scale;        // length must be 0 or 3
        SVArray<f64> translation;  // length must be 0 or 3
        SVArray<f64> matrix;       // length must be 0 or 16
        SVArray<f64> weights;  // The weights of the instantiated Morph Target
    };
    
    struct Buffer {
        SVString name;
        SVString uri;  // considered as required here but not in the spec (need to clarify)
        SVDataSwapPtr data = MakeSharedPtr<SVDataSwap>();
        bool operator==(const Buffer &) const;
    };
    
    struct Asset {
        SVString version;  // required
        SVString generator;
        SVString minVersion;
        SVString copyright;
        bool operator==(const Asset &) const;
    };
    
    struct Scene {
        SVString name;
        SVArray<s32> nodes;
        bool operator==(const Scene &) const;
    };
    
    struct Light {
        SVString name;
        SVString type;
        SVArray<f64> color;
        bool operator==(const Light &) const;
    };
    
    //
    class GLTFModel {
    public:
        GLTFModel() {
        }
        
        ~GLTFModel() {
            accessors.destroy();
            animations.destroy();
            buffers.destroy();
            bufferViews.destroy();
            materials.destroy();
            meshes.destroy();
            nodes.destroy();
            textures.destroy();
            images.destroy();
            skins.destroy();
            samplers.destroy();
            cameras.destroy();
            scenes.destroy();
            lights.destroy();
            extensionsUsed.destroy();
            extensionsRequired.destroy();
            defaultScene = -1;
        }
        bool operator==(const GLTFModel &) const;
        //
        SVArray<Accessor> accessors;
        SVArray<Animation> animations;
        SVArray<Buffer> buffers;
        SVArray<BufferView> bufferViews;
        SVArray<Material> materials;
        SVArray<Mesh> meshes;
        SVArray<Node> nodes;
        SVArray<Texture> textures;
        SVArray<Image> images;
        SVArray<Skin> skins;
        SVArray<Sampler> samplers;
        SVArray<Camera> cameras;
        SVArray<Scene> scenes;
        SVArray<Light> lights;
        SVArray<SVString> extensionsUsed;
        SVArray<SVString> extensionsRequired;
        Asset asset;
        s32 defaultScene;
        s32 m_defaultSceneIndex;
    };
    
    //
    class SVLoaderGLTF : public SVGBase{
    public:
        SVLoaderGLTF(SVInst *_app);
        
        ~SVLoaderGLTF();
        
        bool loadFromFile(cptr8 _filename);
        
        void building();
        
        SVNodePtr getNode(cptr8 _nodename);
        //
    private:
        typedef SVArray<SVNodePtr> NODEARRAY;
        NODEARRAY m_nodeArray;
        
    protected:
        GLTFModel m_gltf;
        
        cptr8 _base64_encode(u8 const *s, u32 len);
        
        cptr8 _base64_decode(SVString const &s);
        
        //解析各种数据
        bool _parseAsset(Asset *_asset , RAPIDJSON_NAMESPACE::Value &_item);
        
        bool _parseBuffer(Buffer *_buffer, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _basedir);
        
        bool _isDataURI(cptr8 _inData);
        
        bool _decodeDataURI(SVString &_outData, SVString &_mime_type,cptr8 _inData, s64 _reqBytes, bool _checkSize);
        
        bool _loadExternalFile(SVDataSwapPtr _dataOut, cptr8 _filename, s64 _reqBytes);
        
        bool _parseBufferView(BufferView *_bufferView, RAPIDJSON_NAMESPACE::Value &_item);
        
        bool _parseAccessor(Accessor *_accessor, RAPIDJSON_NAMESPACE::Value &_item);
        
        bool _parseMesh(Mesh *_mesh, RAPIDJSON_NAMESPACE::Value &_item);
        
        bool _parsePrimitive(Primitive *_primitive, RAPIDJSON_NAMESPACE::Value &_item);
        
        bool _parseNode(Node *_node, RAPIDJSON_NAMESPACE::Value &_item);
        
        bool _parseMaterial(Material *_material, RAPIDJSON_NAMESPACE::Value &_item);
        
        bool _parseParameterProperty(Parameter *_param, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _prop);
        
        bool _parseImage(Image *_image, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _basedir);
        
        bool _parseTexture(Texture *_texture, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _basedir);
        
        bool _parseAnimation(Animation *_animation, RAPIDJSON_NAMESPACE::Value &_item);
        
        bool _parseAnimationChannel(AnimationChannel *_channel, RAPIDJSON_NAMESPACE::Value &_item);
        
        bool _parseSkin(Skin *_skin, RAPIDJSON_NAMESPACE::Value &_item);
        
        bool _parseSampler(Sampler *_sampler, RAPIDJSON_NAMESPACE::Value &_item);
        
        void _loadSkinsData();
        
        void _loadAnimationData();
        
    private:
        //构建骨骼动画节点
        SVNodePtr _buildSkinNode(Node* _node);
        //构建3d节点
        SVNodePtr _build3DNode(Node* _node);
        //构建相机节点
        SVNodePtr _buildCameraNode(Node* _node);
        //构建模型
        SVModelPtr _buildModel(s32 _index);
        //构建骨架
        SVSkeletonPtr _buildSkin(s32 _index);
        //构建动画
        SVAnimateSkinPtr _buildAnimate(s32 _index);
        //构建轨道数据
        void _buildChnData(SVAnimateSkinPtr _ani,s32 _index);
        //构建骨头
        bool _buildBone(SVBonePtr _parent,Skin* _skinData,s32 _index,SVSkeletonPtr _ske);
        //构建材质
        SVMtlCorePtr _buildMtl(Primitive* _prim,s32 _vtf);
        //构建mesh
        SVMeshPtr _buildMeshPri(Primitive* _prim);
        //抓模型数据
        void _fetchDataFromAcc(SVDataSwapPtr _data,Accessor *_accessor);
        //抓动画数据
        void _fetchDataFromAcc(SVSkinAniDataPtr _data,Accessor *_accessor);
        //抓骨架数据
        void _fetchDataFromAcc(SVSkeletonPtr _ske,Skin* _skindata,Accessor *_accessor);
        
    };
    

}//!namespace sv




#endif //SV_LOADER_GLTF_H
