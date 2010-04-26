
//#define LINUX
//#define MACOSX
#define WINDOWS


#if defined(MSDOS) || defined(OS2) || defined(WINDOWS) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif
