#include "memory.h"

namespace Core {

bool Memory::ReadMatrix(uintptr_t address, Matrix4x4& matrix) {
    return ProcessManager::ReadRaw(address, &matrix, sizeof(Matrix4x4));
}

bool Memory::ReadString(uintptr_t address, char* buffer, size_t maxLen) {
    if (!buffer || maxLen == 0) return false;
    return ProcessManager::ReadRaw(address, buffer, maxLen);
}

bool Memory::ReadBones(uintptr_t boneArrayAddr, BoneJointData* bones, size_t count) {
    if (!bones || count == 0) return false;
    return ProcessManager::ReadRaw(boneArrayAddr, bones, sizeof(BoneJointData) * count);
}

} // namespace Core
