#include "strings.h"
#include <HedgeLib/Archives/HHArchive.h>
#include <HedgeLib/Archives/LWArchive.h>
#include <HedgeLib/Archives/Archive.h>
#include <HedgeLib/IO/Path.h>
#include <string>
#include <cstring>
#include <chrono>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

enum HAP_MODES
{
    HAP_MODE_UNKNOWN = 0,
    HAP_MODE_EXTRACT,
    HAP_MODE_PACK
};

void PrintArchiveTypes(const char* prefix = " ")
{
    /*ncout << prefix << "heroes/shadow\t\t" << GetText(FILE_TYPE_HEROES) << std::endl;
    ncout << prefix << "sb/storybook\t\t" << GetText(FILE_TYPE_STORYBOOK) << std::endl;*/
    ncout << prefix << "unleashed/gens/ar/pfd\t" << GetText(FILE_TYPE_HEDGEHOG) << std::endl;
    ncout << prefix << "lw/lost world/pacv2\t" << GetText(FILE_TYPE_PACV2) << std::endl;
    //ncout << prefix << "forces/pacv3\t\t" << GetText(FILE_TYPE_PACV3) << std::endl;
}

void PrintHelp()
{
    // Usage
    ncout << "HedgeArcPack v" << GetText(VERSION_STRING) << std::endl;
    ncout << GetText(USAGE_STRING) << std::endl;
    ncout << std::endl;

    ncout << GetText(HELP1_STRING) << std::endl;
    PrintArchiveTypes("\t\t");
    ncout << std::endl;

    ncout << GetText(HELP2_STRING) << std::endl;

    // TODO: Let user specify padding amount (maybe -A?)
}

int Error(STRING_ID id)
{
    ncout << GetText(ERROR_STRING) << GetText(id) << std::endl;
    return EXIT_FAILURE;
}

hl_ArchiveType GetArchiveType(const hl_NativeStr type)
{
    //// Heroes/Shadow the Hedgehog .one files
    //if (hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("heroes")) ||
    //    hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("shadow")))
    //{
    //    return HL_ARC_TYPE_HEROES;
    //}

    //// Secret Rings/Black Knight .one files
    //if (hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("sb")) ||
    //    hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("storybook")))
    //{
    //    return HL_ARC_TYPE_STORYBOOK;
    //}

    // Unleashed/Generations .ar/.pfd files
    if (hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("unleashed")) ||
        hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("gens")) ||
        hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("ar")) ||
        hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("pfd")))
    {
        return HL_ARC_TYPE_HEDGEHOG;
    }

    // Lost World .pac files
    if (hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("lw")) ||
        hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("lost world")) ||
        hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("pacv2")))
    {
        return HL_ARC_TYPE_PACX_V2;
    }

    //// Forces .pac files
    //if (hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("forces")) ||
    //    hl_StringsEqualInvASCII(type, HL_NATIVE_TEXT("pacv3")))
    //{
    //    return HL_ARC_TYPE_PACX_V3;
    //}

    return HL_ARC_TYPE_UNKNOWN;
}

const hl_NativeStr GetArchiveExt(hl_ArchiveType type)
{
    switch (type)
    {
    /*case HL_ARC_TYPE_HEROES:
    case HL_ARC_TYPE_STORYBOOK:
        return hl_ONEExtensionNative;*/

    case HL_ARC_TYPE_HEDGEHOG:
        return hl_ARExtensionNative;

    case HL_ARC_TYPE_PACX_V2:
    case HL_ARC_TYPE_PACX_V3:
        return hl_PACxExtensionNative;
    }

    return nullptr;
}

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    // We have to manually tell the console on Windows to output UTF-16 or
    // it screws up because it's an absolute piece of unbelievable garbage.
#ifdef _WIN32
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1)
    {
        // Something is seriously wrong if this fails lol
        return EXIT_FAILURE;
    }
#endif

    // Show help
    if (argc < 2 || (argc == 2 &&
        (hl_StringsEqualNative(argv[1], HL_NATIVE_TEXT("-?")) ||
        hl_StringsEqualNative(argv[1], HL_NATIVE_TEXT("/?")))))
    {
        PrintHelp();
        return EXIT_SUCCESS;
    }

    // Parse arguments
    HL_RESULT result;
    const hl_NativeStr input;
    hl_NativeStr output;
    HAP_MODES mode;
    hl_ArchiveType type = HL_ARC_TYPE_UNKNOWN;
    bool isSplit, be = false, customSplitLimit = false, freeOutput = false;
    unsigned long splitLimit = 0;

    if (argc == 2)
    {
        // Check if argument is a flag
        if (argv[1][0] == HL_NATIVE_TEXT('-')) return Error(ERROR_NO_INPUT);

        // Auto-determine output and mode based on input
        input = argv[1];
        if (hl_PathIsDirectory(input))
        {
            mode = HAP_MODE_PACK;
            output = nullptr;
        }
        else
        {
            mode = HAP_MODE_EXTRACT;
            result = hl_PathRemoveExts(input, &output); // TODO: Check result
            freeOutput = true;
        }
    }
    else
    {
        // Parse arguments to get input, output, and mode
        input = nullptr;
        output = nullptr;
        mode = HAP_MODE_UNKNOWN;

        for (int i = 1; i < argc; ++i)
        {
            // Check if argument is a flag
            if (argv[i][0] == HL_NATIVE_TEXT('-'))
            {
                // Parse flag
                switch (HL_TOLOWERASCII(argv[i][1]))
                {
                // Extract mode
                case 'e':
                    if (mode) return Error(ERROR_TOO_MANY_MODES);
                    mode = HAP_MODE_EXTRACT;
                    continue;

                // Pack mode
                case 'p':
                    if (mode) return Error(ERROR_TOO_MANY_MODES);
                    mode = HAP_MODE_PACK;
                    continue;

                // Type flag
                case 't':
                    if (argv[i][2] != '=') return Error(ERROR_INVALID_TYPE);

                    type = GetArchiveType(argv[i] + 3);
                    if (type == HL_ARC_TYPE_UNKNOWN)
                    {
                        return Error(ERROR_INVALID_TYPE);
                    }
                    continue;

                // Big Endian flag
                case 'b':
                    be = true;
                    continue;

                // Split limit
                case 's':
                    if (argv[i][2] != '=') return Error(ERROR_INVALID_SPLIT_LIMIT);

                    splitLimit =
#ifdef _WIN32
                        std::wcstoul(
#else
                        std::strtoul(
#endif
                        argv[i] + 3, nullptr, 0);

                    customSplitLimit = true;
                    continue;

                // Invalid flag
                default:
                    return Error(ERROR_INVALID_FLAGS);
                }
            }

            // Set input and output
            if (!input) input = argv[i];
            else if (!output) output = argv[i];

            // Arguments are invalid
            else return Error(ERROR_TOO_MANY_ARGUMENTS);
        }
        
        // Quit if input was not specified
        if (!input) return Error(ERROR_NO_INPUT);

        // Auto-determine mode
        if (!mode)
        {
            mode = (hl_PathIsDirectory(input)) ?
                HAP_MODE_PACK : HAP_MODE_EXTRACT;
        }
    }

    // Auto-determine type
    if (type == HL_ARC_TYPE_UNKNOWN)
    {
        if (mode == HAP_MODE_EXTRACT)
        {
            isSplit = hl_GetArchiveType(input, &type);
        }
        else
        {
            // TODO: Determine archive type from various information, such
            // as presence of an existing archive with the same name.
        }
    }

    // Prompt user if type could not be auto-determined
    if (type == HL_ARC_TYPE_UNKNOWN)
    {
        // Prompt user for type
        ncout << GetText(TYPE1_STRING) << std::endl;
        
        PrintArchiveTypes();
        ncout << std::endl;
        ncout << GetText(TYPE2_STRING);

        // Get user input
        nstring arcType;
        ncin >> arcType;

        type = GetArchiveType(arcType.c_str());

        if (type == HL_ARC_TYPE_UNKNOWN)
        {
            return Error(ERROR_INVALID_TYPE);
        }
    }

    // Auto-determine outputs
    if (!output)
    {
        if (mode == HAP_MODE_PACK)
        {
            const hl_NativeStr ext = GetArchiveExt(type); // TODO: If PFD flag is set, use PFD extension instead
            result = hl_StringJoinNative(input, ext, &output); // TODO: Check result
        }
        else
        {
            result = hl_PathRemoveExts(input, &output); // TODO: Check result
        }

        freeOutput = true;
    }

    std::chrono::high_resolution_clock::time_point begin =
        std::chrono::high_resolution_clock::now();

    // Extract archive
    if (mode == HAP_MODE_EXTRACT)
    {
        ncout << GetText(EXTRACTING_STRING) << std::endl;
        result = hl_ExtractArchivesOfType(input, output, type);
    }

    // Pack archive from directory
    else
    {
        ncout << GetText(PACKING_STRING) << std::endl;

        // Get files in input directory
        size_t fileCount;
        char** files;

#ifdef _WIN32
        char* inputDir;
        // TODO: Check result
        result = hl_StringConvertUTF16ToUTF8(
            reinterpret_cast<const uint16_t*>(input),
            &inputDir);
#else
        const char* inputDir = input;
#endif

        // TODO: Check result
        result = hl_PathGetFilesInDirectory(
            inputDir, false, &fileCount, &files);

#ifdef _WIN32
        std::free(inputDir);
#endif

        hl_ArchiveFileEntry* entries = hl_CreateArchiveFileEntries(
            const_cast<const char**>(files), fileCount);

        // Pack archive
        if (type == HL_ARC_TYPE_HEDGEHOG)
        {
            // TODO: Re-add HHArchive support
            /*result = hl_CreateHHArchive(entries, fileCount, outputDir, outputName,
                static_cast<uint32_t>((customSplitLimit) ?
                    splitLimit : HL_PACX_DEFAULT_SPLIT_LIMIT), 0x10,
                HL_HHARCHIVE_TYPE_UNCOMPRESSED);*/
            return Error(ERROR_INVALID_TYPE);
        }
        else if (type == HL_ARC_TYPE_PACX_V2)
        {
            result = hl_CreateLWArchivesNative(entries, fileCount,
                output, static_cast<uint32_t>((customSplitLimit) ?
                splitLimit : HL_PACX_DEFAULT_SPLIT_LIMIT), be);
        }
        else
        {
            return Error(ERROR_INVALID_TYPE);
        }

        // TODO: Support other types

        // Free data
        std::free(entries);
        std::free(files);
    }

    // Free output string if necessary
    if (freeOutput) std::free(output);

    // Print elapsed time if succeeded
    if (HL_OK(result))
    {
        std::chrono::high_resolution_clock::time_point end =
            std::chrono::high_resolution_clock::now();

        auto runtime = std::chrono::duration_cast<
            std::chrono::milliseconds>(end - begin).count();

        ncout << GetText(DONE1_STRING) << (runtime / 1000.0f) <<
            GetText(DONE2_STRING) << std::endl;
    }

    // Otherwise, print error
    else
    {
        // TODO: Friendly error
        // TODO: Call Error function instead of this
        ncout << GetText(ERROR_STRING) << result << std::endl;
    }

    return EXIT_SUCCESS;
}
