#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

// Processor architecture detection.  For more info on what's defined, see:
//   http://msdn.microsoft.com/en-us/library/b0084kay.aspx
//   http://www.agner.org/optimize/calling_conventions.pdf
//   or with gcc, run: "echo | gcc -E -dM -"
#if defined(_M_X64) || defined(__x86_64__) || defined(__LP64__)
#define GN_ARCH_X86_FAMILY
#define GN_ARCH_X86_64
#define GN_ARCH_64_BITS
#define GN_ARCH_LITTLE_ENDIAN
#elif defined(_M_IX86) || defined(__i386__)
#define GN_ARCH_X86_FAMILY
#define GN_ARCH_X86
#define GN_ARCH_32_BITS
#define GN_ARCH_LITTLE_ENDIAN
#elif defined(__ARMEL__)
// TODO(ajm): We'd prefer to control platform defines here, but this is
// currently provided by the Android makefiles. Commented to avoid duplicate
// definition warnings.
//#define HJ_ARCH_ARM
// TODO(ajm): Chromium uses the following two defines. Should we switch?
//#define HJ_ARCH_ARM_FAMILY
//#define HJ_ARCH_ARMEL
#define GN_ARCH_32_BITS
#define GN_ARCH_LITTLE_ENDIAN
#elif defined(__MIPSEL__)
#define GN_ARCH_32_BITS
#define GN_ARCH_LITTLE_ENDIAN
#else
#error Please add support for your architecture in typedefs.h
#endif

#if !(defined(GN_ARCH_LITTLE_ENDIAN) ^ defined(GN_ARCH_BIG_ENDIAN))
#error Define either HJ_ARCH_LITTLE_ENDIAN or HJ_ARCH_BIG_ENDIAN
#endif

#if defined(__SSE2__) || defined(_MSC_VER)
#define GN_USE_SSE2
#endif

#if !defined(_MSC_VER)
#include <stdint.h>
#else
// Define C99 equivalent types, since MSVC doesn't provide stdint.h.
#ifndef int8_t
typedef signed char         int8_t;
#endif
#ifndef int16_t
typedef signed short        int16_t;
#endif
#ifndef int32_t
typedef signed int          int32_t;
#endif
#ifndef int64_t
typedef __int64             int64_t;
#endif
#ifndef uint8_t
typedef unsigned char       uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short      uint16_t;
#endif
#ifndef uint32_t
typedef unsigned int        uint32_t;
#endif
#ifndef uint64_t
typedef unsigned __int64    uint64_t;
#endif
#endif

// Borrowed from Chromium's base/compiler_specific.h.
// Annotate a virtual method indicating it must be overriding a virtual
// method in the parent class.
// Use like:
//   virtual void foo() OVERRIDE;
#if defined(_MSC_VER)
#define OVERRIDE override
#elif defined(__clang__)
// Clang defaults to C++03 and warns about using override. Squelch that.
// Intentionally no push/pop here so all users of OVERRIDE ignore the warning
// too. This is like passing -Wno-c++11-extensions, except that GCC won't die
// (because it won't see this pragma).
#pragma clang diagnostic ignored "-Wc++11-extensions"
#define OVERRIDE override
#else
#define OVERRIDE
#endif

// Annotate a function indicating the caller must examine the return value.
// Use like:
//   int foo() WARN_UNUSED_RESULT;
// TODO(ajm): Hack to avoid multiple definitions until the base/ of webrtc and
// libjingle are merged.
#if !defined(WARN_UNUSED_RESULT)
#if defined(__GNUC__)
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define WARN_UNUSED_RESULT
#endif
#endif  // WARN_UNUSED_RESULT

#endif  // _TYPEDEFS_H_
