#pragma once
#include <Windows.h>
#include"Mem.h"
#include<vector>
class Player
{
public:
	Player(Mem* m)
	{
		mem = m;
	}
	typedef struct
	{
		unsigned long long ClientPlayer, ClientVehicleEntity;
		char* PlayerName, * VehicleID,* VehicleName;
		bool isInVehicle;
	}PlayerInfo;
	
	std::vector<PlayerInfo*> GetPlayerInVehicle();

private:
	const int OFFSET_PlayerName = 0x18;
	const int OFFSET_ClientVehicleEntity = 0x1D38;
	const int OFFSET_VehicleEntityData = 0x30;
	const int OFFSET_VeniceVehicleCustomizationAsset = 0x120;
	const int OFFSET_VehicleID = 0x10;
	Mem* mem;
	unsigned long long EncryptedPlayerMgr_GetPlayer(unsigned long long ptr, int id);
	PlayerInfo* GetPlayerById(int id);
	unsigned long long GetClientPlayerById(int id);
};

