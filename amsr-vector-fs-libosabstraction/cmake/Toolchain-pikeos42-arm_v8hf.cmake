#######################################################################################################################
#    Model Element   : Toolchain-pikeos42-arm_v8hf
#    Copyright       : Copyright (c) 2018, Vector Informatik GmbH.
#    File Name       : Toolchain-pikeos42-arm_v8hf.cmake
#    Details         : This file allows cross compiling for PikeOS 4.2 using CMake and the PikeOS CDK.
#                      It contains toolchain settings specific to PikeOS 4.2 for ARMv8.
#######################################################################################################################

# GENERAL
# =======

# PikeOS installation specific
set(PIKEOS_INSTALL_DIR "/opt/pikeos-4.2")
set(PIKEOS_TARGET_BIN_DIR "${PIKEOS_INSTALL_DIR}/cdk/arm/v8hf/bin")
set(PIKEOS_TARGET_BIN_DIR "${PIKEOS_INSTALL_DIR}/cdk/arm/v8hf/bin")
set(PIKEOS_POOL_DIR "${PIKEOS_INSTALL_DIR}/target/arm/v8hf")
set(CUSTOM_POOL_DIR "$ENV{HOME}/build/pikeos_pool")

# Set the name of the target system
set(CMAKE_SYSTEM_NAME PikeOS)
# Set the version of the target system
set(CMAKE_SYSTEM_VERSION 4.2)
# Set processor (or hardware) of the target system
set(CMAKE_SYSTEM_PROCESSOR arm_v8hf)

# Set C compiler
set(CMAKE_C_COMPILER ${PIKEOS_TARGET_BIN_DIR}/arm_v8hf-gcc)
# Set C++ compiler
set(CMAKE_CXX_COMPILER ${PIKEOS_TARGET_BIN_DIR}/arm_v8hf-g++)
## Set linker
#set(CMAKE_LINKER /opt/pikeos-4.2/cdk/arm/v8hf/bin/arm_v8hf-ld
#  CACHE STRING "Path to linker."
#  )

# Set linker calling command rule
# !!! Order of parameters is important here !!!
# Otherwise you will get plenty of unresolved STL symbols during linking!
set(CMAKE_CXX_LINK_EXECUTABLE
  "<CMAKE_LINKER> -o <TARGET> <OBJECTS> <LINK_LIBRARIES> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS>"
  CACHE STRING "Linker calling command rule for C executables." FORCE
)
set(CMAKE_C_LINK_EXECUTABLE
  "<CMAKE_LINKER> -o <TARGET> <OBJECTS> <LINK_LIBRARIES> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS>"
  CACHE STRING "Linker calling command rule for C executables." FORCE
)


# Set search directory
# If you don't want to use only libraries that come with the toolchain but also build and use additional libraries for
# your target platform, you should create an install directory for these packages, e.g. $HOME/eldk-ppc_74xx-inst/
set(CMAKE_FIND_ROOT_PATH ${PIKEOS_INSTALL_DIR}/cdk/arm/v8hf)
# Set that programs should only be searched in the directories given in CMAKE_FIND_ROOT_PATH
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
# Set that libraries should only be searched in the directories given in CMAKE_FIND_ROOT_PATH
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# Set that include files should only be searched in the directories given in CMAKE_FIND_ROOT_PATH
# RapidJSON is needed for
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)


# Compiler flags from PikeOS 4.2 Makefiles
# ========================================

# project.mk
# ----------
set(POSIX_CXX_STD "c++11")

message(STATUS "-------------------------------------------------------------")
message(STATUS "PikeOS Settings")
message(STATUS "PIKEOS_INSTALL_DIR: ${PIKEOS_INSTALL_DIR}")
message(STATUS "PIKEOS_TARGET_BIN_DIR: ${PIKEOS_TARGET_BIN_DIR}")
message(STATUS "PIKEOS_POOL_DIR: ${PIKEOS_POOL_DIR}")
message(STATUS "CUSTOM_POOL_DIR: ${CUSTOM_POOL_DIR}")
message(STATUS "POSIX_CXX_STD: ${POSIX_CXX_STD}")
option(POSIX_CXX   "Build POSIX with C++ runtime " ON)
message(STATUS "POSIX_CXX: ${POSIX_CXX}")
option(POSIX_SMP   "Build POSIX with SMP support " OFF)
message(STATUS "POSIX_SMP: ${POSIX_SMP}")
option(POSIX_DEBUG "Build POSIX application debugging " OFF)
message(STATUS "POSIX_DEBUG: ${POSIX_DEBUG}")
option(POSIX_TRACE "Build POSIX application tracing " OFF)
message(STATUS "POSIX_TRACE: ${POSIX_TRACE}")
option(POSIX_INSTR "Build POSIX instrumon support " OFF)
message(STATUS "POSIX_INSTR: ${POSIX_INSTR}")
option(POSIX_LWIP  "Build network support using LwIP " OFF)
message(STATUS "POSIX_LWIP: ${POSIX_LWIP}")
option(POSIX_LWIP_CUSTOM  "Build network support using custom LwIP " OFF)
message(STATUS "POSIX_LWIP_CUSTOM: ${POSIX_LWIP_CUSTOM}")
option(POSIX_NFSCL "Build POSIX with NFS client " OFF)
message(STATUS "POSIX_NFSCL: ${POSIX_NFSCL}")
option(POSIX_SCHED_HOOK "Build POSIX with scheduler hook support " OFF)
message(STATUS "POSIX_SCHED_HOOK: ${POSIX_SCHED_HOOK}")
option(POSIX_DDK   "Build POSIX with DDK " OFF)
message(STATUS "POSIX_DDK: ${POSIX_DDK}")
message(STATUS "-------------------------------------------------------------")


# makeinc/compiler.mk
# -------------------
# we always want debugging symbols, we strip the binary used on the target
add_compile_options(-g)
# we never want internal include
#add_compile_options(-nostdinc)
# we never want internal c++ include
add_compile_options(-nostdinc++)
# no libc support so builtin of gcc must not be used
add_compile_options(-fno-builtin)
# disable gcc strict aliasing
add_compile_options(-fno-strict-aliasing)
# Don't let compiler remove null pointer checking
add_compile_options(-fno-delete-null-pointer-checks)
# don't let gcc think we cannot wrap around
add_compile_options(-fno-strict-overflow)
# no global block for global variables
add_compile_options(-fno-common)
# add define to signal assembler compilation
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS} -D__ASSEMBLER__)

# makeinc/architecture.mk
# -----------------------
add_compile_options(-mfix-cortex-a53-835769)
add_compile_options(-mfix-cortex-a53-843419)
# Rationale: components such as libraries should ensure minimal interference to
# the programs linking with them.  Therefore they should prevent e.g., the
# generation of vector-related instructions, or the generation of non-ABI
# compilant instr.
#add_compile_options(-mcmodel=large)
#when fpu is not used for using only general registers
#add_compile_options(-mgeneral-regs-only)
#add_compile_options(-mgeneral-regs-only)

# makeinc/libgcc.mk
# -----------------
#ask compiler for libgcc directory
execute_process(COMMAND
  ${CMAKE_CXX_COMPILER}
  -print-libgcc-file-name
  OUTPUT_VARIABLE
  LIBGCC_DIR
)
get_filename_component(LIBGCC_DIR ${LIBGCC_DIR} DIRECTORY)
set(LIBGCC_DIR "${LIBGCC_DIR}/")

#push to linker rules and include for compiler
add_compile_options(-I${LIBGCC_DIR}/include)
add_compile_options(-I${LIBGCC_DIR}/include-fixed)
set(LINKER_LIBS "${LINKER_LIBS} -lgcc")
set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${LIBGCC_DIR}")

# posix/makeinc/personality.mk
# ----------------------------
add_compile_options(-I${CUSTOM_POOL_DIR}/posix/include)
add_compile_options(-I${PIKEOS_POOL_DIR}/posix/include)

# PikeOS Standard headers
add_compile_options(-I${CUSTOM_POOL_DIR}/include)
add_compile_options(-I${PIKEOS_POOL_DIR}/include)

# PikeOS DDK headers
add_compile_options(-I${CUSTOM_POOL_DIR}/driver/include)
add_compile_options(-I${PIKEOS_POOL_DIR}/driver/include)

# Personality identifier
add_definitions(-DPIKEOS_POSIX)

# Determine OS variant (debug, trace, Instrumon, and combinations thereof)
if(POSIX_INSTR)
  if(POSIX_DEBUG)
    set(POSIX_VARIANT instr-debug)
  else()
    set(POSIX_VARIANT instr)
  endif()
else()
  if(POSIX_DEBUG AND POSIX_TRACE)
	  set(POSIX_VARIANT debug-trace)
  elseif(POSIX_TRACE)
	  set(POSIX_VARIANT trace)
  elseif(POSIX_DEBUG)
    set(POSIX_VARIANT debug)
  else()
    unset(POSIX_VARIANT)
  endif()
endif()


# Determine library location
if(POSIX_DEBUG)
   add_definitions(-DPOSIX_DEBUG)
  set(POSIX_LIB_DIR lib/debug)
else()
  set(POSIX_LIB_DIR lib)
endif()


# Determine OS location
if(POSIX_SMP)
  set(POSIX_OS_LIB_DIR os/smp/${POSIX_VARIANT})
else()
  set(POSIX_OS_LIB_DIR os/up/${POSIX_VARIANT})
endif()

# POSIX library directory (debug/normal)
set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${CUSTOM_POOL_DIR}/posix/${POSIX_LIB_DIR}")
set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${PIKEOS_POOL_DIR}/posix/${POSIX_LIB_DIR}")

# POSIX library directory (debug-independent)
if(NOT ${POSIX_LIB_DIR} STREQUAL "lib")
  set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${CUSTOM_POOL_DIR}/posix/lib")
  set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${PIKEOS_POOL_DIR}/posix/lib")
endif()

# POSIX OS directory
set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${CUSTOM_POOL_DIR}/posix/${POSIX_OS_LIB_DIR}")
set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${PIKEOS_POOL_DIR}/posix/${POSIX_OS_LIB_DIR}")

# PikOS Standard library directory
set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${CUSTOM_POOL_DIR}/lib")
set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${PIKEOS_POOL_DIR}/lib")

# Put crt0 in final binary
set(LINKER_OBJ_FILES "${LINKER_OBJ_FILES} ${PIKEOS_POOL_DIR}/posix/${POSIX_OS_LIB_DIR}/crt0.o")

# Dummy console driver
if(${POSIX_TTY_NONE})
  set(LINKER_OBJ_FILES "${LINKER_OBJ_FILES} ${PIKEOS_POOL_DIR}/posix/${POSIX_OS_LIB_DIR}/tty_none.o")
endif()

# Math library
set(LINKER_LIBS "${LINKER_LIBS} -lm")

# C library
set(LINKER_LIBS "${LINKER_LIBS} -lc")

# PSE51 library
set(LINKER_LIBS "${LINKER_LIBS} -lpse51")


# GDB stub with signal support
if(${POSIX_DEBUG})
  set(LINKER_LIBS "${LINKER_LIBS} -lgdbstubsig")
endif()

# SPIDER trace support
if(${POSIX_TRACE})
  set(LINKER_LIBS "${LINKER_LIBS} -lspider")
endif()

# SSW library
set(LINKER_LIBS "${LINKER_LIBS} -lvm")

# Stand library
set(LINKER_LIBS "${LINKER_LIBS} -lstand")

# PikeOS library
set(LINKER_LIBS "${LINKER_LIBS} -lp4")

# Use standard linker script
set(LINKER_SCRIPT "${PIKEOS_POOL_DIR}/ldscript/app.ld")

# No special flavor, use standard obj and depend
set(ODIR obj)
set(DEPDIR depend)


# posix/makeinc/option/cxx.mk
# ---------------------------
if(${POSIX_CXX})
  # Add personality option identifier
  add_definitions(-DPOSIX_CXX)
  # C++ language standard version
  add_compile_options(-std=${POSIX_CXX_STD})
  # Add C++ headers
  add_compile_options(-I${CUSTOM_POOL_DIR}/posix/include/${POSIX_CXX_STD})
  add_compile_options(-I${PIKEOS_POOL_DIR}/posix/include/${POSIX_CXX_STD})
  # Add backward compat header
  add_compile_options(-I${CUSTOM_POOL_DIR}/posix/include/${POSIX_CXX_STD}/backward)
  add_compile_options(-I${PIKEOS_POOL_DIR}/posix/include/${POSIX_CXX_STD}/backward)
  # Add C++ init
  set(LINKER_OBJ_FILES "${LINKER_OBJ_FILES} ${PIKEOS_POOL_DIR}/posix/${POSIX_LIB_DIR}/cxx_init-${POSIX_CXX_STD}.o")
  # Add C++ library
  set(LINKER_LIBS "${LINKER_LIBS} -lstdcxx-${POSIX_CXX_STD}")
endif()


# posix/makeinc/option/ddk.mk
# ---------------------------
if(${POSIX_DDK})
  add_compile_options(-I${CUSTOM_POOL_DIR}/driver/include)
  add_compile_options(-I${PIKEOS_POOL_DIR}/driver/include)
endif()


# posix/makeinc/option/instrumon-api.mk
# ---------------------------
if(${POSIX_INSTR})
  # Add personality option identifier
  add_definitions(-DPOSIX_LIB_INSTRUMON)
  # Add Instrumon API headers
  add_compile_options(-I${PIKEOS_POOL_DIR}/posix/include/instrumon-api)
  # Add Instrumon API library path
  set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${PIKEOS_POOL_DIR}/posix/lib")
  # Add Instrumon API library
  set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -linstrumon_api_pse5x")
endif()

# posix/makeinc/option/lwip.mk
# ---------------------------
if(${POSIX_LWIP} OR ${POSIX_LWIP_CUSTOM})
  # Add personality option identifier
  add_definitions(-DPOSIX_LWIP)
  if(${POSIX_LWIP_CUSTOM})
    # Add lwIP standard configuration header
    add_compile_options(-I${CUSTOM_POOL_DIR}/posix/lwip/include)
    # Add lwIP library path
    set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${CUSTOM_POOL_DIR}/posix/lwip/lib")
    # Add lwIP headers
    add_compile_options(-I${PIKEOS_POOL_DIR}/posix/lwip/include)
  else(${POSIX_LWIP_CUSTOM})
    # Add lwIP standard configuration header
    add_compile_options(-I${PIKEOS_POOL_DIR}/posix/lwip/include/opts)
    # Add lwIP library path
    set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${PIKEOS_POOL_DIR}/posix/lwip/lib")
    # Add lwIP headers
    add_compile_options(-I${PIKEOS_POOL_DIR}/posix/lwip/include)
  endif(${POSIX_LWIP_CUSTOM})
  # Add lwIP library
  set(LINKER_LIBS "${LINKER_LIBS} -llwip")
  # Add SBUF library
  set(LINKER_LIBS "${LINKER_LIBS} -lsbuf")
endif()

# posix/makeinc/option/nfscl.mk
# ---------------------------
if(${POSIX_NFSCL})
  # Add personality option identifier
  add_definitions(-DPOSIX_NFSCL)
  # Add NFS client headers
  add_compile_options(-I${PIKEOS_POOL_DIR}/posix/nfscl/include)

  # RPC layer for selected network
  if(${POSIX_LWIP})
    set(POSIX_NFSCL_RPC -lrpc_lwip)
  endif()
  if(${POSIX_ANISFP})
    set(POSIX_NFSCL_RPC -lrpc_anisfp)
  endif()
  # Library suffix
  if(${POSIX_NFSCL_DIAG})
    set(POSIX_NFSCL_LIBSUFF -diag)
  else()
    unset(POSIX_NFSCL_LIBSUFF)
  endif()

  # Add NFS client library path
  set(LINKER_LIB_DIRS "${LINKER_LIB_DIRS} -L${PIKEOS_POOL_DIR}/posix/nfscl/lib")

  # Add NFS client library
  set(LINKER_LIBS "${LINKER_LIBS} -lnfscl${POSIX_NFSCL_LIBSUFF}")

  # Add RPC layer
  set(LINKER_LIBS "${LINKER_LIBS} -lnfsrpc${POSIX_NFSCL_LIBSUFF}")

  # Add RPC transport layer
  set(LINKER_LIBS "${LINKER_LIBS} ${POSIX_NFSCL_RPC}${POSIX_NFSCL_LIBSUFF}")
endif()


# posix/makeinc/option/sched-hook.mk
# ---------------------------
if(${POSIX_SCHED_HOOK})
  # Add personality option identifier
  add_definitions(-DPOSIX_SCHED_HOOK)

  # Add scheduler with hook function support
  set(LINKER_OBJ_FILES "${LINKER_OBJ_FILES} ${PIKEOS_POOL_DIR}/posix/${POSIX_OS_LIB_DIR}/h_scheduler.o")
endif()

# posix/makeinc/option/smp.mk
# ---------------------------
if(${POSIX_SMP})
  # Sanity check
  if(${POSIX_INSTR})
    message(FATAL_ERROR  "Instrumon is unsupported in SMP configurations")
  endif()

  # Add personality option identifier
  add_definitions(-DPOSIX_SMP)
  # Option identifier for compatibility with older versions
  add_definitions(-DSMP)
endif()


# Build linker flags. Force update on each configuration run.
set(CMAKE_EXE_LINKER_FLAGS
  "${LINKER_LIB_DIRS} ${LINKER_OBJ_FILES} -T${LINKER_SCRIPT} --start-group ${LINKER_LIBS} --end-group"
  CACHE STRING "Linker flags for linking executables." FORCE
)


