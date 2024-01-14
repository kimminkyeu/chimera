//
// Created by Minkyeu Kim on 1/14/24.
//

#ifndef CHIMERAENGINECORE_PLATFORMDETECTION_H
#define CHIMERAENGINECORE_PLATFORMDETECTION_H

/*
 * Platform detection using predefined macros
 * ------------------------------------------
 * CM_PLATFORM_WINDOWS
 * CM_PLATFORM_MACOS
 * CM_PLATFORM_IOS
 * CM_PLATFORM_ANDROID
 * CM_PLATFORM_LINUX
 *
 * Platform detection using predefined macros
 * ------------------------------------------
 * CM_COMPILER_MSVC           Visual Studio
 * CM_COMPILER_GCC            gcc / g++
 * CM_COMPILER_CLANG          clang
 * CM_COMPILER_MINGW          MinGW 32, MinGW-w64 32bit, MinGW-w64 64bit  */

#ifdef _WIN32 /* Windows x64/x86 */
# ifdef _WIN64
#   define CM_PLATFORM_WINDOWS /* Windows x64  */
# else
#   error "x86 Builds are not supported!" /* Windows x86 */
# endif

#elif defined(__APPLE__) || defined(__MACH__)
# include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
# if TARGET_IPHONE_SIMULATOR == 1
#   error "IOS simulator is not supported!"
# elif TARGET_OS_IPHONE == 1
#   define CM_PLATFORM_IOS
#   error "IOS is not supported!"
# elif TARGET_OS_MAC == 1
#   define CM_PLATFORM_MACOS
# else
#   error "Unknown Apple platform!"
# endif

/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
# define CM_PLATFORM_ANDROID
# error "Android is not supported!"
#elif defined(__linux__)
# define CM_PLATFORM_LINUX
# error "Linux is not supported!"
#else
# error "Unknown platform!" /* Unknown compiler/platform */
#endif

#if defined(__clang__)
# define CM_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
# define CM_COMPILER_GCC
#elif defined(_MSC_VER)
# define CM_COMPILER_MSVC
#elif defined(__MINGW32__) || defined(__MINGW64__)
# define CM_COMPILER_MINGW
#endif

#endif // CHIMERAENGINECORE_PLATFORMDETECTION_H