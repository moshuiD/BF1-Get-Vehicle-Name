#pragma once
#include <Windows.h>
#include "Mem.h"
#define ValidPointer( pointer ) ( pointer != NULL && (DWORD_PTR)pointer >= 0x10000 && (DWORD_PTR)pointer < 0x00007FFFFFFEFFFF /*&& some other checks*/ )
class Mem
{
public:
	Mem(HANDLE h)
	{
		pHandle = h;
	}
	template<typename T>
	T ReadMem(uintptr_t addr)
	{
		T buffer;
		SIZE_T buff{};
		ReadProcessMemory(Mem::pHandle, (void*)addr, &buffer, sizeof(T), &buff);
		return buffer;
	}

	char* ReadMemStr(uintptr_t addr, int length)
	{
		char* buffer = new char[length];
		SIZE_T buff{};
		ReadProcessMemory(Mem::pHandle, (void*)addr, buffer, length, &buff);
		return buffer;
	}

	template<typename T>
	bool WriteMem(uintptr_t addr, T buff)
	{
		SIZE_T temp;
		return WriteProcessMemory(Mem::pHandle, addr, &buff, sizeof(T), &temp);
	}


private:
	HANDLE pHandle;

};

