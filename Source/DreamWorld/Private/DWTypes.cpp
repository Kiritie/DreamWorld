#include "DWTypes.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Main/MainModule.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Ability/Character/CharacterAbilityBase.h"
#include "Vitality/DWVitalityData.h"
#include "Character/DWCharacterData.h"

FDWTeamData FDWTeamData::Empty = FDWTeamData();

void FDWVitalityBasicSaveData::Initialize()
{
}

void FDWVitalitySaveData::Initialize()
{
	Super::Initialize();

	InventoryData = GetVitalityData().InventoryData;
}

UDWVitalityData& FDWVitalitySaveData::GetVitalityData() const
{
	return UAssetModuleBPLibrary::LoadPrimaryAssetRef<UDWVitalityData>(ID);
}

void FDWCharacterSaveData::Initialize()
{
	Super::Initialize();

	InventoryData = GetCharacterData().InventoryData;
}

UDWCharacterData& FDWCharacterSaveData::GetCharacterData() const
{
	return UAssetModuleBPLibrary::LoadPrimaryAssetRef<UDWCharacterData>(ID);
}

FDWArchiveBasicSaveData FDWPlayerSaveData::GetArchiveData() const
{
	if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>())
	{
		if(GeneralSaveGame->SaveData.ArchiveBasicDatas.IsValidIndex(ArchiveID))
		{
			return GeneralSaveGame->SaveData.ArchiveBasicDatas[ArchiveID];
		}
	}
	return FDWArchiveBasicSaveData();
}

void FDWVoxelWorldSaveData::Initialize()
{
	Super::Initialize();
}

FDWArchiveBasicSaveData FDWVoxelWorldSaveData::GetArchiveData() const
{
	if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>())
	{
		if(GeneralSaveGame->SaveData.ArchiveBasicDatas.IsValidIndex(ArchiveID))
		{
			return GeneralSaveGame->SaveData.ArchiveBasicDatas[ArchiveID];
		}
	}
	return FDWArchiveBasicSaveData();
}

void FDWTeamData::AddMember(ADWCharacter* InMember)
{
	if (!Members.Contains(InMember))
	{
		InMember->SetTeamID(ID);
		Members.Add(InMember);
	}
}

void FDWTeamData::RemoveMember(ADWCharacter* InMember)
{
	if (Members.Contains(InMember))
	{
		if(!IsCaptain(InMember))
		{
			InMember->SetTeamID(TEXT(""));
			Members.Remove(InMember);
		}
		else
		{
			DissolveTeam();
		}
	}
}

void FDWTeamData::DissolveTeam()
{
	for (int i = 0; i < Members.Num(); i++)
	{
		if(Members[i] && Members[i]->IsValidLowLevel())
		{
			Members[i]->SetTeamID(TEXT(""));
		}
	}
	Members.Empty();
}

TArray<ADWCharacter*> FDWTeamData::GetMembers(ADWCharacter* InMember)
{
	auto tmpArr = TArray<ADWCharacter*>();
	for (int i = 0; i < Members.Num(); i++)
	{
		if (Members[i] != InMember)
			tmpArr.Add(Members[i]);
	}
	return tmpArr;
}
