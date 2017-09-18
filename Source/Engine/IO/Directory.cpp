
#include <giga-engine.h>

#ifdef WIN32
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
    #define GetCurrentDir getcwd
#endif

Directory::Directory() {
    
}

std::string Directory::GetCurrentDirectory() {
    std::string str;
    char cCurrentPath[FILENAME_MAX];
    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to get current working directory."));
        return(str);
    }
    
    str = cCurrentPath;
    return(str);
}
