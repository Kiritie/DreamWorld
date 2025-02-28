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
	for (int32 i = 0; i < Members.Num(); i++)
	{
		if(IDWTeamAgentInterface* TeamAgent = USceneModuleStatics::GetSceneActor<IDWTeamAgentInterface>(Members[i].ToString(), false))
		{
			TeamAgent->SetTeamID(TEXT(""));
		}
	}
	Members.Empty();
}

TArray<IDWTeamAgentInterface*> FDWTeamSaveData::GetMembers(IDWTeamAgentInterface* InMember)
{
	TArray<IDWTeamAgentInterface*> ReturnValues;
	for (int32 i = 0; i < Members.Num(); i++)
	{
		if (Members[i] != InMember->GetActorIDT())
		{
			if(IDWTeamAgentInterface* TeamAgent = USceneModuleStatics::GetSceneActor<IDWTeamAgentInterface>(Members[i].ToString(), false))
			{
				ReturnValues.Add(TeamAgent);
			}
		}
	}
	return ReturnValues;
}

bool FDWTeamSaveData::IsCaptain(IDWTeamAgentInterface* InMember) const
{
	return !Captain.IsValid() || Captain == InMember->GetActorIDT();
}
