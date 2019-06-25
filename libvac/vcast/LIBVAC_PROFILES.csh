rm commands.tmp
echo 'options C_ALT_COMPILE_CMD gcc -c -g' >> commands.tmp
echo 'options C_ALT_EDG_FLAGS -w --gcc --gnu_version 50400' >> commands.tmp
echo 'options C_ALT_PREPROCESS_CMD gcc -E -C' >> commands.tmp
echo 'options C_COMPILER_CFG_SOURCE PY_CONFIGURATOR' >> commands.tmp
echo 'options C_COMPILER_FAMILY_NAME GNU_Native' >> commands.tmp
echo 'options C_COMPILER_HIERARCHY_STRING GNU Native_5.4_C++11' >> commands.tmp
echo 'options C_COMPILER_PY_ARGS --lang c++ --version 5.4 --standard c++11' >> commands.tmp
echo 'options C_COMPILER_TAG GNU_CPP11_54' >> commands.tmp
echo 'options C_COMPILER_VERSION_CMD g++ --version' >> commands.tmp
echo 'options C_COMPILE_CMD g++ -c -g -std=c++11' >> commands.tmp
echo 'options C_DEBUG_CMD gdb' >> commands.tmp
echo 'options C_DEFINE_LIST VCAST_NO_PTR_TO_UNCON_ARRAY_SUPPORT' >> commands.tmp
echo 'options C_EDG_FLAGS -w --g++ --gnu_version 50400 --type_info_in_std --type_traits_helpers --c++11' >> commands.tmp
echo 'options C_LINKER_VERSION_CMD ld --version' >> commands.tmp
echo 'options C_LINK_CMD g++ -g' >> commands.tmp
echo 'options C_PREPROCESS_CMD g++ -E -C -std=c++11' >> commands.tmp
echo 'options SOURCE_EXTENSION .cpp' >> commands.tmp
echo 'options VCAST_ASSEMBLY_FILE_EXTENSIONS asm s' >> commands.tmp
echo 'options VCAST_COLLAPSE_STD_HEADERS COLLAPSE_SYSTEM_HEADERS' >> commands.tmp
echo 'options VCAST_COMMAND_LINE_DEBUGGER TRUE' >> commands.tmp
echo 'options VCAST_COMPILER_SUPPORTS_CPP_CASTS TRUE' >> commands.tmp
echo 'options VCAST_DISABLE_STD_WSTRING_DETECTION TRUE' >> commands.tmp
echo 'options VCAST_DISPLAY_UNINST_EXPR FALSE' >> commands.tmp
echo 'options VCAST_ENABLE_FUNCTION_CALL_COVERAGE FALSE' >> commands.tmp
echo 'options VCAST_ENVIRONMENT_FILES ' >> commands.tmp
echo 'options VCAST_HAS_LONGLONG TRUE' >> commands.tmp
echo 'options VCAST_PREPROCESS_PREINCLUDE $(VECTORCAST_DIR)/DATA/gnu_native/intrinsics.h' >> commands.tmp
echo 'options VCAST_REPOSITORY ' >> commands.tmp
echo 'clear_default_source_dirs ' >> commands.tmp
echo 'options TESTABLE_SOURCE_DIR ../lib/' >> commands.tmp
echo 'options TESTABLE_SOURCE_DIR ../lib/include/vac/io/' >> commands.tmp
echo 'options TESTABLE_SOURCE_DIR ../lib/src/vac/io/' >> commands.tmp
echo 'environment build LIBVAC_PROFILES.env' >> commands.tmp
echo '-eLIBVAC_PROFILES tools script run LIBVAC_PROFILES.tst' >> commands.tmp
echo '-eLIBVAC_PROFILES execute batch' >> commands.tmp
echo '-eLIBVAC_PROFILES reports custom management LIBVAC_PROFILES_management_report.html' >> commands.tmp
$VECTORCAST_DIR/clicast  -lCPLUSPLUS tools execute commands.tmp false
