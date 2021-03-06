LOCALIZED_TEXT(ENGLISH)
{
    /* USAGE_STRING */
    HL_NTEXT("Usage: HedgeArcPack input [output] [mode] [flags]\n\n"),

    /* HELP1_STRING */
    HL_NTEXT("Arguments surrounded by square brackets are optional. If they\n")
    HL_NTEXT("aren't specified, they will be auto-determined based on input.\n\n")
    HL_NTEXT("If the desired archive type wasn't specified with -T and cannot\n")
    HL_NTEXT("be automatically determined, the user will be prompted to enter one.\n\n")
    HL_NTEXT("Modes:\n\n")
    HL_NTEXT(" -?\t\tDisplays this help.\n\n")
    HL_NTEXT(" -E\t\tExtracts the archive specified by input to the directory\n")
    HL_NTEXT("\t\tspecified by output.\n\n")
    HL_NTEXT(" -P\t\tPacks the directory specified by input into an archive located\n")
    HL_NTEXT("\t\tat output as well as a series of \"split\" archives located next to\n")
    HL_NTEXT("\t\toutput if required (e.g. if output == \"out/w1a01_far.pac\", \"splits\"\n")
    HL_NTEXT("\t\tsuch as \"out/w1a01_far.pac.00\", \"out/w1a01_far.pac.01\", etc. may\n")
    HL_NTEXT("\t\tbe generated).\n\n")
    HL_NTEXT("Flags:\n\n")
    HL_NTEXT(" -T=type\tSpecifies what type of archive to pack/extract.\n")
    HL_NTEXT("\t\tValid options are:\n\n\n"),

    /* HELP2_STRING */
    HL_NTEXT(" -B\t\tPacks archives in big-endian if the given type supports it.\n")
    HL_NTEXT("\t\tIgnored when extracting.\n\n")
    HL_NTEXT(" -S=limit\tSpecifies a custom \"split limit\" (how big each split is\n")
    HL_NTEXT("\t\tallowed to be, in bytes) for packing. Ignored when extracting.\n")
    HL_NTEXT("\t\tSet this to 0 to disable the split limit.\n\n")
    HL_NTEXT(" -I\t\tAlso generates a .pfi for the given archive. Specified by default\n")
    HL_NTEXT("\t\twhen type == pfd. Ignored when extracting.\n"),

    /* PRESS_ENTER_STRING */
    HL_NTEXT("\nPress enter to continue..."),

    /* WARNING_STRING */
    HL_NTEXT("WARNING: %s\n"),

    /* ERROR_STRING */
    HL_NTEXT("ERROR: %s\n"),

    /* ERROR_TOO_MANY_MODES */
    HL_NTEXT("Only one mode may be specified at a time. Use -? for proper usage information."),

    /* ERROR_TOO_MANY_PATHS */
    HL_NTEXT("Too many paths were given. Use -? for proper usage information."),

    /* ERROR_INVALID_TYPE */
    HL_NTEXT("Invalid archive type."),

    /* ERROR_INVALID_INPUT */
    HL_NTEXT("The given input file or folder does not exist. Use -? for proper usage information."),

    /* ERROR_INVALID_SPLIT_LIMIT */
    HL_NTEXT("Invalid split limit."),

    /* TYPE1_STRING */
    HL_NTEXT("Archive type could not be auto-determined.\n")
    HL_NTEXT("Please enter one of the following options:\n\n"),

    /* TYPE2_STRING */
    HL_NTEXT("Archive type: "),

    /* FILE_TYPE_AR */
    HL_NTEXT("su/sg/gens/ar/pfd\t(Unleashed/Generations .ar/.pfd files)\n"),

    /* FILE_TYPE_PACxV2 */
    HL_NTEXT("lw/slw/pac2\t\t(Lost World .pac files)\n"),
    
    /* FILE_TYPE_PACxV3 */
    HL_NTEXT("wars/forces/pac3\t(Forces .pac files)\n"),

    /* FILE_TYPE_PACxV4 */
    HL_NTEXT("rings/pac4\t\t(Tokyo 2020/Sakura Wars .pac files)\n\n"),

    /* EXTRACTING_STRING */
    HL_NTEXT("Extracting..."),

    /* PACKING_STRING */
    HL_NTEXT("Packing..."),

    /* DONE1_STRING */
    HL_NTEXT("Done! Completed in "),

    /* DONE2_STRING */
    HL_NTEXT(" seconds.")
};
