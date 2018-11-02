//
// SVglTFio.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SVglTFio_h
#define SVglTFio_h
#include "../base/SVObject.h"
#include "SVglTF.h"
using namespace std;
namespace sv{
    namespace util{
        // -----------------------------------------------------------------------------
        // PATH UTILITIES
        // -----------------------------------------------------------------------------
        // Normalize path delimiters.
        cptr8 normalize_path(cptr8 _filename);
        // Get directory name (not including '/').
        cptr8 get_dirname(cptr8 _filename);
        // Get extension (not including '.').
        cptr8 get_extension(cptr8 _filename);
        // Get filename without directory.
        cptr8 get_filename(cptr8 _filename);
        // Replace extension.
        cptr8 replace_extension(cptr8 _filename, cptr8 _ext);
        // Check if a file can be opened for reading.
        bool exists_file(cptr8 _filename);
        
        
        // -----------------------------------------------------------------------------
        // TRIVIAL COMMAND LINE PARSING
        // -----------------------------------------------------------------------------
        // Command line parser data. All data should be considered private.
        struct cmdline_parser {
            std::vector<SVString> args  = {}    ;  // command line arguments
            SVString         usage_cmd = "";  // program name
            SVString         usage_hlp = "";  // program help
            SVString         usage_opt = "";  // options help
            SVString         usage_arg = "";  // arguments help
            SVString         error     = "";  // current parse error
        };
        
        // Initialize a command line parser.
        cmdline_parser make_cmdline_parser(
                                           s32 argc, c8** argv, cptr8 usage, cptr8 cmd = "");
        // check if any error occurred and exit appropriately
        void check_cmdline(cmdline_parser& parser);
        
        // Parse an int, float, string, vecXX and bool option or positional argument.
        // Options's names starts with "--" or "-", otherwise they are arguments.
        // vecXX options use space-separated values but all in one argument
        // (use " or ' from the common line). Booleans are flags.
        template <typename T>
        inline T      parse_arg(cmdline_parser& parser, cptr8 name, T def,
                                cptr8 usage, bool req = false);
        inline c8* parse_arg(cmdline_parser& parser, cptr8 name,
                                cptr8 def, cptr8 usage, bool req = false);
        // Parse all arguments left on the command line.
        template <typename T>
        inline std::vector<T> parse_args(cmdline_parser& parser, cptr8 name,
                                    const std::vector<T>& def, cptr8 usage, bool req = false);
        // Parse a labeled enum, with enum values that are successive integers.
        template <typename T>
        inline T parse_arge(cmdline_parser& parser, cptr8 name, T def,
                            cptr8 usage, const std::vector<SVString>& labels, bool req = false);

        
        
        // -----------------------------------------------------------------------------
        // FILE IO
        // -----------------------------------------------------------------------------
        // Load/save a text file
        cptr8 load_text(cptr8 filename);
        bool   save_text(cptr8 filename, cptr8 str);
        
        // Load/save a binary file
        std::vector<Byte> load_binary(cptr8 filename);
        bool         save_binary(cptr8 filename, const std::vector<Byte>& data);
        
        
//        // -----------------------------------------------------------------------------
//        // SCENE IO FUNCTIONS
//        // -----------------------------------------------------------------------------
//        // Load/save a scene in the supported formats.
        svgltf_scene* load_scene(cptr8 _filename, bool _load_textures = true,
                                bool _skip_missing = true);
        bool         save_scene(cptr8 _filename, const svgltf_scene* _scene,
                                bool _save_textures = true, bool _skip_missing = true);
//
//        // Load/save a scene in the builtin JSON format.
//        svgltf_scene* load_json_scene(cptr8 _filename, bool _load_textures = true,
//                                     bool _skip_missing = true);
//        bool         save_json_scene(cptr8 _filename, const svgltf_scene* _scene,
//                                     bool _save_textures = true, bool _skip_missing = true);
//
//        // Load/save a scene from/to glTF.
        svgltf_scene* load_gltf_scene(cptr8 _filename, bool _load_textures = true,
                                     bool _skip_missing = true);
        bool         save_gltf_scene(cptr8 _filename, const svgltf_scene* _scene,
                                     bool _save_textures = true, bool _skip_missing = true);

    }// namespace util
}// namespace sv
#endif /* SVglTFio_h */
