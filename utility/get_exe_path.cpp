#include <stdexcept>
#include <string>
#ifdef _WIN32
#include <Windows.h>
static const auto bsize = MAX_PATH;
static const auto delim = '\\';
#else
#include <unistd.h>
#include <string.h>
static const auto bsize = 1024;
static const auto delim = '/';
#endif

namespace utility
{
    std::string get_exe_path()
    {
        char buf[bsize];
#ifdef _WIN32
        auto r = ::GetModuleFileNameA(0, buf, bsize);
#else
        auto r = readlink("/proc/self/exe", buf, bsize);
        if (r == -1) {
            throw std::runtime_error("get_exe_path failed");
        }
        if (r >= bsize) {
            --r;
        }
        buf[r] = '\0';
#endif
        char* p = strrchr(buf, delim);
        if (p)
        {
            p[1] = '\0';
        }
        return std::string(buf);
    }
}

