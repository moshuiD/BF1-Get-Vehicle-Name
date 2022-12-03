#include "Player.h"
#define Long unsigned long long
#define OFFSET_OBFUSCATIONMGR 0x14351D058
#define OFFSET_CLIENTGAMECONTEXT 0x1437F7758

std::vector<Player::PlayerInfo*> Player::GetPlayerInVehicle()
{
	std::vector<PlayerInfo*>retValue;
	for (int i = 0; i < 70; i++)
	{
		PlayerInfo* p = GetPlayerById(i);
		if (!ValidPointer(p))
			continue;
		if (p->isInVehicle)
		{
			retValue.push_back(p);
		}
		else
		{
			delete[] p->PlayerName;

			delete p;
		}
	}
	return retValue;
}


/// <summary>
/// 获得全部玩家
/// </summary>
/// <param name="id"></param>
/// <returns>记得Free返回值</returns>
Player::PlayerInfo* Player::GetPlayerById(int id)
{

	PlayerInfo* info = new PlayerInfo;
	info->ClientPlayer = GetClientPlayerById(id);
	if (!ValidPointer(info->ClientPlayer))
	{
		delete info;
		return nullptr;
	}
	info->PlayerName = mem->ReadMemStr(mem->ReadMem<Long>(info->ClientPlayer + OFFSET_PlayerName), 40);

	Long tempVehicleEntity = mem->ReadMem<Long>(info->ClientPlayer + OFFSET_ClientVehicleEntity);
	if (ValidPointer(tempVehicleEntity))
	{
		info->ClientVehicleEntity = tempVehicleEntity;
		info->isInVehicle = true;

		Long tempVehicleEntityData = mem->ReadMem<Long>(info->ClientVehicleEntity + OFFSET_VehicleEntityData);
		if (ValidPointer(tempVehicleEntityData))
		{
			Long tempVeniceVehicleCustomizationAsset = mem->ReadMem<Long>(tempVehicleEntityData + OFFSET_VeniceVehicleCustomizationAsset);
			if (ValidPointer(tempVeniceVehicleCustomizationAsset))
			{
				info->VehicleID = mem->ReadMemStr(mem->ReadMem<Long>(tempVeniceVehicleCustomizationAsset + OFFSET_VehicleID), 100);
				if (ValidPointer(info->VehicleID))
				{
					return info;
				}
			}

		}
	}

	info->isInVehicle = false;

	return info;
}


Long Player::GetClientPlayerById(int id)
{
	Long pClientGameContext = mem->ReadMem<Long>(OFFSET_CLIENTGAMECONTEXT);
	if (!ValidPointer(pClientGameContext))
	{
		return 0;
	}

	Long pPlayerManager = mem->ReadMem<Long>(pClientGameContext + 0x68);
	if (!ValidPointer(pPlayerManager))
	{
		return 0;
	}

	Long pObfuscationMgr = mem->ReadMem<Long>(OFFSET_OBFUSCATIONMGR);
	if (!ValidPointer(pObfuscationMgr))
	{
		return 0;
	}

	Long PlayerListXorValue = mem->ReadMem<Long>(pPlayerManager + 0xF8);
	Long PlayerListKey = PlayerListXorValue ^ mem->ReadMem<Long>(pObfuscationMgr + 0x70);

	Long mpBucketArray = mem->ReadMem<Long>(pObfuscationMgr + 0x10);


	int mnBucketCount = mem->ReadMem<int>(pObfuscationMgr + 0x18);
	if (mnBucketCount == 0)
	{
		return 0;
	}

	int startCount = (int)PlayerListKey % mnBucketCount;

	Long mpBucketArray_startCount = mem->ReadMem<Long>(mpBucketArray + (Long)(startCount * 8));
	Long node_first = mem->ReadMem<Long>(mpBucketArray_startCount);
	Long node_second = mem->ReadMem<Long>(mpBucketArray_startCount + 0x8);
	Long node_mpNext = mem->ReadMem<Long>(mpBucketArray_startCount + 0x10);

	while (PlayerListKey != node_first)
	{
		mpBucketArray_startCount = node_mpNext;

		node_first = mem->ReadMem<Long>(mpBucketArray_startCount);
		node_second = mem->ReadMem<Long>(mpBucketArray_startCount + 0x8);
		node_mpNext = mem->ReadMem<Long>(mpBucketArray_startCount + 0x10);
	}

	Long EncryptedPlayerMgr = node_second;
	return EncryptedPlayerMgr_GetPlayer(EncryptedPlayerMgr, id);

}
Long Player::EncryptedPlayerMgr_GetPlayer(Long ptr, int id)
{
	Long XorValue1 = mem->ReadMem<Long>(ptr + 0x20) ^ mem->ReadMem<Long>(ptr + 0x8);
	Long XorValue2 = XorValue1 ^ mem->ReadMem<Long>(ptr + 0x10);
	if (!ValidPointer(XorValue2))
	{
		return 0;
	}

	return XorValue1 ^ mem->ReadMem<Long>(XorValue2 + 0x8 * id);
}