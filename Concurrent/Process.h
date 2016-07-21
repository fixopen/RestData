//
// Created by up duan on 24/7/15.
//

#ifndef RESTDATA_PROCESS_H
#define RESTDATA_PROCESS_H

#include <string>
#include <memory>

typedef void* PROCESS_INFORMATION;

class Process {
public:
    static std::unique_ptr<Process> Create(std::wstring const& command);
    Process(std::wstring const& command);
    void Start();
    void Stop();
private:
    std::wstring const& executableFile;
    PROCESS_INFORMATION ProcessInfo;
};


#endif //RESTDATA_PROCESS_H
