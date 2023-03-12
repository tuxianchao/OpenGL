#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <string>
#include <cstdlib>
#include "root_directory.h"

class FileSystem
{

public:
    static std::string getResPath(const std::string& path)
    {
        return root_resource_dir + path;
    }
};

#endif
