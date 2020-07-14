#pragma once

template<typename func_type>
__forceinline static func_type call_vfunction(void* ppClass, int index) {
    int* pVTable = *(int**)ppClass;
    int dwAddress = pVTable[index];
    return (func_type)(dwAddress);
}

template<unsigned int IIdx, typename TRet, typename ... TArgs>
static auto call_vfunc(void* thisptr, TArgs ... argList) -> TRet {
    using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
    return (*(Fn**)(thisptr))[IIdx](thisptr, argList...);
}

template<typename T>
__forceinline static T vfunc(void* base, int index) {
	DWORD* vTabella = *(DWORD**)base;
	return (T)vTabella[index];
}

template<typename T, typename ...Args>
constexpr auto call_virtual_method(void* classBase, int index, Args... args) noexcept {
    return ((*(T(__thiscall***)(void*, Args...))(classBase))[index])(classBase, args...);
}
