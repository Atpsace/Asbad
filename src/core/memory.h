#pragma once
#include "types.h"
#include "process.h"
#include <vector>

namespace Core {

class Memory {
public:
    static bool ReadMatrix(uintptr_t address, Matrix4x4& matrix);
    static bool ReadString(uintptr_t address, char* buffer, size_t maxLen);
    static bool ReadBones(uintptr_t boneArrayAddr, BoneJointData* bones, size_t count);
    
    template<typename T>
    static bool Read(uintptr_t address, T& out) {
        return ProcessManager::Read(address, out);
    }
    
    template<typename T>
    static bool Write(uintptr_t address, const T& value) {
        return ProcessManager::Write(address, value);
    }
};

} // namespace Core
