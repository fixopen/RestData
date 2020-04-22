#include <sys/time.h>
#include <stdio.h> //for FILE, fopen, fclose, fwrite

#include "Log.h"
#include "../text/StringOp.h" //for Text::StringOp

namespace Util {
    bool isWriteLog = true;

    void SetWriteLog(bool flag) {
        isWriteLog = flag;
    }

    bool WriteLog(std::wstring const& filePath, std::string const& mainContent, std::string const& type) {
        auto result = false;

        if (isWriteLog) {
            auto logFile = fopen(Text::StringOp::FromUnicode(filePath).c_str(), "a");

            if (logFile) {
                time_t timeValue = time(nullptr);
                char time[32] = {0};
                tm* timeStructure = gmtime(&timeValue);
                strftime(time, 32, "%04d-%02d-%02d %02d:%02d:%02d", timeStructure);
                //sprintf(time, "%04d-%02d-%02d %02d:%02d:%02d", currentTime.wYear, currentTime.wMonth, currentTime.wDay, currentTime.wHour, currentTime.wMinute, currentTime.wSecond);
                std::string content = "\r\n" + std::string(time) + " " + type + ": " + mainContent + "\r\n";
                fwrite(content.c_str(), 1ul, content.length(), logFile);
                fclose(logFile);
                result = true;
            }
        }

        return result;
    }
}
