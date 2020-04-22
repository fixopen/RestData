#ifndef __UTIL_LOG_H__
#define __UTIL_LOG_H__

#pragma once

#include <string> //for std::wstring, std::string

namespace Util {
    void SetWriteLog(bool flag);
    bool WriteLog(std::wstring const& filePath, std::string const& mainContent, std::string const& type);
}

#endif //__UTIL_LOG_H__
