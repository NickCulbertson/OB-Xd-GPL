/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   appicon_png;
    const int            appicon_pngSize = 130776;

    extern const char*   main_png;
    const int            main_pngSize = 21159;

    extern const char*   main2x_png;
    const int            main2x_pngSize = 47319;

    extern const char*   main4x_png;
    const int            main4x_pngSize = 93446;

    extern const char*   menu_png;
    const int            menu_pngSize = 12061;

    extern const char*   menu2x_png;
    const int            menu2x_pngSize = 12061;

    extern const char*   menu4x_png;
    const int            menu4x_pngSize = 12061;

    extern const char*   presetnavigation_svg;
    const int            presetnavigation_svgSize = 2078;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 8;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
