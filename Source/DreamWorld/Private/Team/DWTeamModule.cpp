// Fill out your copyright notice in the Description page of Project Settings.


#include "Team/DWTeamModule.h"

#include "Team/Agent/DWTeamAgentInterface.h"

IMPLEMENTATION_MODULE(ADWTeamModule)

// Sets default values
ADWTeamModule::ADWTeamModule()
{
	ModuleName = FName("TeamModule");

	TeamDatas = TMap<FName, FDWTeamSaveData>();
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

void ADWTeamModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	const auto& SaveData = InSaveData->CastRef<FDWTeamModuleSaveData>();

	if(PHASEC(InPhase, EPhase::Final))
	{
		TeamDatas = SaveData.TeamDatas;
	}
}

FSaveData* ADWTeamModule::ToData(bool bRefresh)
{
	static FDWTeamModuleSaveData* SaveData;
	SaveData = new FDWTeamModuleSaveData();

	SaveData->TeamDatas = TeamDatas;

	return SaveData;
}

void ADWTeamModule::UnloadData(EPhase InPhase)
{
	if(PHASEC(InPhase, EPhase::Primary))
	{
		TeamDatas.Empty();
	}
}

bool ADWTeamModule::IsExistTeam(const FName& InTeamID) const
{
	return TeamDatas.Contains(InTeamID);
}

bool ADWTeamModule::CreateTeam(IDWTeamAgentInterface* InCaptain, FName InTeamName /*= NAME_None*/, FString InTeamDetail /*= TEXT("")*/)
{
	if (InCaptain->GetTeamID().IsNone())
	{
		auto tmpData = FDWTeamSaveData();
		tmpData.ID = *FString::Printf(TEXT("Team_%d"), TeamDatas.Num());
		if (!IsExistTeam(tmpData.ID))
		{
			tmpData.Name = !InTeamName.IsNone() ? InTeamName : *FString::Printf(TEXT("%s Team"), *InCaptain->GetNameT().ToString());
			tmpData.Detail = !InTeamDetail.IsEmpty() ? InTeamDetail : tmpData.Name.ToString();
			tmpData.Captain = InCaptain->GetActorIDT();
			tmpData.AddMember(InCaptain);
			TeamDatas.Add(tmpData.ID, tmpData);
			return true;
		}
	}
	return false;
}

bool ADWTeamModule::DissolveTeam(const FName& InTeamID, IDWTeamAgentInterface* InCaptain)
{
	if (IsExistTeam(InTeamID) && TeamDatas[InTeamID].IsCaptain(InCaptain))
	{
		TeamDatas[InTeamID].DissolveTeam();
		TeamDatas.Remove(InTeamID);
		return true;
	}
	return false;
}

FDWTeamSaveData& ADWTeamModule::GetTeamData(const FName& InTeamID)
{
	if (TeamDatas.Contains(InTeamID))
	{
		return TeamDatas[InTeamID];
	}
	return FDWTeamSaveData::Empty;
}
