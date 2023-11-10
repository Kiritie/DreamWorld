// Fill out your copyright notice in the Description page of Project Settings.


#include "Team/DWTeamModuleTypes.h"

#include "Scene/SceneModuleStatics.h"
#include "Team/Agent/DWTeamAgentInterface.h"

FDWTeamSaveData FDWTeamSaveData::Empty = FDWTeamSaveData();

void FDWTeamSaveData::AddMember(IDWTeamAgentInterface* InMember)
{
	if (!Members.Contains(InMember->GetActorIDT()))
	{
		InMember->SetTeamID(ID);
		Members.Add(InMember->GetActorIDT());
	}
}

void FDWTeamSaveData::RemoveMember(IDWTeamAgentInterface* InMember)
{
	if (Members.Contains(InMember->GetActorIDT()))
	{
		if(!IsCaptain(InMember))
		{
			InMember->SetTeamID(TEXT(""));
			Members.Remove(InMember->GetActorIDT());
		}
		else
		{
			DissolveTeam();
		}
	}
}

void FDWTeamSaveData::DissolveTeam()
{
	for (int i = 0; i < Members.Num(); i++)
	{
		USceneModuleStatics::GetSceneActor<IDWTeamAgentInterface>(Members[i])->SetTeamID(TEXT(""));
	}
	Members.Empty();
}

TArray<IDWTeamAgentInterface*> FDWTeamSaveData::GetMembers(IDWTeamAgentInterface* InMember)
{
	auto tmpArr = TArray<IDWTeamAgentInterface*>();
	for (int i = 0; i < Members.Num(); i++)
	{
		if (Members[i] != InMember->GetActorIDT())
		{
			tmpArr.Add(USceneModuleStatics::GetSceneActor<IDWTeamAgentInterface>(Members[i]));
		}
	}
	return tmpArr;
}

bool FDWTeamSaveData::IsCaptain(IDWTeamAgentInterface* InMember) const
{
	return !Captain.IsValid() || Captain == InMember->GetActorIDT();
}
