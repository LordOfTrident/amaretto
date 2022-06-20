#ifndef PLATFORM_HH__HEADER_GUARD__
#define PLATFORM_HH__HEADER_GUARD__

// Windows
#if defined(WIN32) or defined(_WIN32) or defined(__WIN32__) or defined(__NT__)
#	define AMARETTO_PLATFORM_WINDOWS

// Apple
#elif defined(__APPLE__)
#	define AMARETTO_PLATFORM_APPLE

// Linux
#elif defined(__linux__) or defined(__gnu_linux__) or defined(linux)
#	define AMARETTO_PLATFORM_LINUX

// Unix
#elif defined(__unix__) or defined(unix)
#	define AMARETTO_PLATFORM_UNIX

// Unknown
#else
#	define AMARETTO_PLATFORM_UNKNOWN
#endif // else

#ifdef AMARETTO_PLATFORM_UNKNOWN
#	error Unknown platform
#elif defined(AMARETTO_PLATFORM_APPLE)
#	pragma message "Apple platform detected, attempting to compile as linux"

#	define AMARETTO_PLATFORM_LINUX
#elif defined(AMARETTO_PLATFORM_UNIX)
#	pragma message "Unix platform detected, attempting to compile as linux"

#	define AMARETTO_PLATFORM_LINUX
#endif // PLATFORM_UNIX

#endif // not PLATFORM_HH__HEADER_GUARD__
