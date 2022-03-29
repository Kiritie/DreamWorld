#include "DWTypes.h"

#include "Asset/AssetModuleBPLibrary.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Main/MainModule.h"
#include "SaveGame/DWGeneralSaveGame.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Ability/Character/CharacterAbilityBase.h"

FTeamData FTeamData::Empty = FTeamData();

UVitalityAssetBase* FVitalitySaveData::GetVitalityData() const
{
	return UAssetModuleBPLibrary::LoadPrimaryAsset<UVitalityAssetBase>(ID);
}

UCharacterAssetBase* FCharacterSaveData::GetCharacterData() const
{
	return UAssetModuleBPLibrary::LoadPrimaryAsset<UCharacterAssetBase>(ID);
}

FArchiveBasicSaveData FPlayerSaveData::GetArchiveData() const
{
	if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>())
	{
		if(GeneralSaveGame->SaveData.ArchiveBasicDatas.IsValidIndex(ArchiveID))
		{
			return GeneralSaveGame->SaveData.ArchiveBasicDatas[ArchiveID];
		}
	}
	return FArchiveBasicSaveData();
}

FArchiveBasicSaveData FDWWorldSaveData::GetArchiveData() const
{
	if(UDWGeneralSaveGame* GeneralSaveGame = USaveGameModuleBPLibrary::GetSaveGame<UDWGeneralSaveGame>())
	{
		if(GeneralSaveGame->SaveData.ArchiveBasicDatas.IsValidIndex(ArchiveID))
		{
			return GeneralSaveGame->SaveData.ArchiveBasicDatas[ArchiveID];
		}
	}
	return FArchiveBasicSaveData();
}

void FTeamData::AddMember(ADWCharacter* InMember)
{
	if (!Members.Contains(InMember))
	{
		InMember->SetTeamID(ID.ToString());
		Members.Add(InMember);
	}
}

void FTeamData::RemoveMember(ADWCharacter* InMember)
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

void FTeamData::DissolveTeam()
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

TArray<ADWCharacter*> FTeamData::GetMembers(ADWCharacter* InMember)
{
	auto tmpArr = TArray<ADWCharacter*>();
	for (int i = 0; i < Members.Num(); i++)
	{
		if (Members[i] != InMember)
			tmpArr.Add(Members[i]);
	}
	return tmpArr;
}
