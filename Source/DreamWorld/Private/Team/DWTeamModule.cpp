// Fill out your copyright notice in the Description page of Project Settings.


#include "Team/DWTeamModule.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "ReferencePool/ReferencePoolModuleBPLibrary.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Datas/VoxelData.h"
		
IMPLEMENTATION_MODULE(ADWTeamModule)

// Sets default values
ADWTeamModule::ADWTeamModule()
{
	ModuleName = FName("TeamModule");

	TeamMap = TMap<FName, FDWTeamData>();
}

ADWTeamModule::~ADWTeamModule()
{
	TERMINATION_MODULE(ADWTeamModule)
}

#if WITH_EDITOR
void ADWTeamModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWTeamModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWTeamModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWTeamModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
}

void ADWTeamModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWTeamModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWTeamModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWTeamModule::OnTermination_Implementation(EPhase InPhase)
{
	Super::OnTermination_Implementation(InPhase);
}

bool ADWTeamModule::IsExistTeam(const FName& InTeamID) const
{
	return TeamMap.Contains(InTeamID);
}

bool ADWTeamModule::CreateTeam(ADWCharacter* InCaptain, FName InTeamName /*= NAME_None*/, FString InTeamDetail /*= TEXT("")*/)
{
	if (InCaptain->GetTeamID().IsNone())
	{
		auto tmpData = FDWTeamData();
		tmpData.ID = *FString::Printf(TEXT("Team_%d"), TeamMap.Num());
		if (!IsExistTeam(tmpData.ID))
		{
			tmpData.Name = !InTeamName.IsNone() ? InTeamName : *FString::Printf(TEXT("%s Team"), *InCaptain->GetNameC().ToString());
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

FDWTeamData* ADWTeamModule::GetTeamData(const FName& InTeamID)
{
	if (TeamMap.Contains(InTeamID))
	{
		return &TeamMap[InTeamID];
	}
	return &FDWTeamData::Empty;
}
