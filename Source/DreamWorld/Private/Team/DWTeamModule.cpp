// Fill out your copyright notice in the Description page of Project Settings.


#include "Team/DWTeamModule.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "SpawnPool/SpawnPoolModuleBPLibrary.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Voxels/VoxelAssetBase.h"

// Sets default values
ADWTeamModule::ADWTeamModule()
{
	TeamMap = TMap<FName, FTeamData>();
}

#if WITH_EDITOR
void ADWTeamModule::OnGenerate_Implementation()
{
	Super::OnGenerate_Implementation();
}

void ADWTeamModule::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
#endif

void ADWTeamModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWTeamModule::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();
}

void ADWTeamModule::OnRefresh_Implementation(float DeltaSeconds)
{
	switch (UDWHelper::GetGameState(this)->GetCurrentState())
	{
		case EGameState::MainMenu:
		case EGameState::Loading:
		{
			GenerateTerrain();
			break;
		}
		case EGameState::Playing:
		{
			GenerateTerrain();
			break;
		}
		default: break;
	}
}

void ADWTeamModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWTeamModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWTeamModule::OnTermination_Implementation()
{
	Super::OnTermination_Implementation();
}

bool ADWTeamModule::IsExistTeam(const FName& InTeamID) const
{
	return TeamMap.Contains(InTeamID);
}

bool ADWTeamModule::CreateTeam(ADWCharacter* InCaptain, FName InTeamName /*= NAME_None*/, FString InTeamDetail /*= TEXT("")*/)
{
	if (InCaptain->GetTeamID().IsEmpty())
	{
		auto tmpData = FTeamData();
		tmpData.ID = *FString::Printf(TEXT("Team_%d"), TeamMap.Num());
		if (!IsExistTeam(tmpData.ID))
		{
			tmpData.Name = !InTeamName.IsNone() ? InTeamName : *FString::Printf(TEXT("%s �� Team"), *InCaptain->GetNameC());
			tmpData.Detail = !InTeamDetail.IsEmpty() ? InTeamDetail : tmpData.Name.ToString();
			tmpData.Captain = InCaptain;
			tmpData.AddMember(InCaptain);
			TeamMap.Add(tmpData.ID, tmpData);
			return true;
		}
	}
	return false;
}

bool ADWTeamModule::DissolveTeam(const FName& InTeamID, ADWCharacter* InCaptain)
{
	if (IsExistTeam(InTeamID) && TeamMap[InTeamID].IsCaptain(InCaptain))
	{
		TeamMap[InTeamID].DissolveTeam();
		TeamMap.Remove(InTeamID);
		return true;
	}
	return false;
}

FTeamData* ADWTeamModule::GetTeamData(const FName& InTeamID)
{
	if (TeamMap.Contains(InTeamID))
	{
		return &TeamMap[InTeamID];
	}
	return &FTeamData::Empty;
}
