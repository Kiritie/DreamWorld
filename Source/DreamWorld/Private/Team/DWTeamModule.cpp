// Fill out your copyright notice in the Description page of Project Settings.


#include "Team/DWTeamModule.h"

#include "Team/Agent/DWTeamAgentInterface.h"

IMPLEMENTATION_MODULE(UDWTeamModule)

// Sets default values
UDWTeamModule::UDWTeamModule()
{
	ModuleName = FName("TeamModule");
	ModuleDisplayName = FText::FromString(TEXT("Team Module"));

	TeamDatas = TMap<FName, FDWTeamSaveData>();
}

UDWTeamModule::~UDWTeamModule()
{
	TERMINATION_MODULE(UDWTeamModule)
}

#if WITH_EDITOR
void UDWTeamModule::OnGenerate()
{
	Super::OnGenerate();
}

void UDWTeamModule::OnDestroy()
{
	Super::OnDestroy();

	TERMINATION_MODULE(UDWTeamModule)
}
#endif

void UDWTeamModule::OnInitialize()
{
	Super::OnInitialize();
}

void UDWTeamModule::OnPreparatory(EPhase InPhase)
{
	Super::OnPreparatory(InPhase);
}

void UDWTeamModule::OnRefresh(float DeltaSeconds, bool bInEditor)
{
	Super::OnRefresh(DeltaSeconds, bInEditor);
}

void UDWTeamModule::OnPause()
{
	Super::OnPause();
}

void UDWTeamModule::OnUnPause()
{
	Super::OnUnPause();
}

void UDWTeamModule::OnTermination(EPhase InPhase)
{
	Super::OnTermination(InPhase);
}

void UDWTeamModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	const auto& SaveData = InSaveData->CastRef<FDWTeamModuleSaveData>();

	if(PHASEC(InPhase, EPhase::Primary))
	{
		TeamDatas = SaveData.TeamDatas;
	}
}

FSaveData* UDWTeamModule::ToData()
{
	static FDWTeamModuleSaveData* SaveData;
	SaveData = new FDWTeamModuleSaveData();

	SaveData->TeamDatas = TeamDatas;

	return SaveData;
}

void UDWTeamModule::UnloadData(EPhase InPhase)
{
	if(PHASEC(InPhase, EPhase::Primary))
	{
		TeamDatas.Empty();
	}
}

bool UDWTeamModule::IsExistTeam(const FName& InTeamID) const
{
	return TeamDatas.Contains(InTeamID);
}

bool UDWTeamModule::CreateTeam(IDWTeamAgentInterface* InCaptain, FName InTeamName /*= NAME_None*/, FString InTeamDetail /*= TEXT("")*/)
{
	if (InCaptain->GetTeamID().IsNone())
	{
		auto Data = FDWTeamSaveData();
		Data.ID = *FString::Printf(TEXT("Team_%d"), TeamDatas.Num());
		if (!IsExistTeam(Data.ID))
		{
			Data.Name = !InTeamName.IsNone() ? InTeamName : *FString::Printf(TEXT("%s's team"), *InCaptain->GetNameT().ToString());
			Data.Detail = !InTeamDetail.IsEmpty() ? InTeamDetail : Data.Name.ToString();
			Data.Captain = InCaptain->GetActorIDT();
			Data.AddMember(InCaptain);
			TeamDatas.Add(Data.ID, Data);
			return true;
		}
	}
	return false;
}

bool UDWTeamModule::DissolveTeam(const FName& InTeamID, IDWTeamAgentInterface* InCaptain)
{
	if (IsExistTeam(InTeamID) && TeamDatas[InTeamID].IsCaptain(InCaptain))
	{
		TeamDatas[InTeamID].DissolveTeam();
		TeamDatas.Remove(InTeamID);
		return true;
	}
	return false;
}

FDWTeamSaveData& UDWTeamModule::GetTeamData(const FName& InTeamID)
{
	if (TeamDatas.Contains(InTeamID))
	{
		return TeamDatas[InTeamID];
	}
	return FDWTeamSaveData::Empty;
}
