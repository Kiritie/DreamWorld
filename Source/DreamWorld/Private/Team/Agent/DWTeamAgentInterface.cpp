#include "Team/Agent/DWTeamAgentInterface.h"

#include "Team/DWTeamModule.h"

bool IDWTeamAgentInterface::HasTeam() const
{
	return ADWTeamModule::Get()->GetTeamData(GetTeamID()).IsValid();
}

bool IDWTeamAgentInterface::IsTeamMate(IDWTeamAgentInterface* InTargetCharacter) const
{
	return InTargetCharacter->GetTeamID() == GetTeamID();
}

bool IDWTeamAgentInterface::CreateTeam(const FName& InTeamName /*= MANE_None*/, FString InTeamDetail /*= TEXT("")*/)
{
	if(ADWTeamModule* TeamModule = ADWTeamModule::Get())
	{
		return TeamModule->CreateTeam(this, InTeamName, InTeamDetail);
	}
	return false;
}

bool IDWTeamAgentInterface::DissolveTeam()
{
	if(ADWTeamModule* TeamModule = ADWTeamModule::Get())
	{
		return TeamModule->DissolveTeam(GetTeamID(), this);
	}
	return false;
}

bool IDWTeamAgentInterface::JoinTeam(const FName& InTeamID)
{
	if(ADWTeamModule* TeamModule = ADWTeamModule::Get())
	{
		if(TeamModule->IsExistTeam(InTeamID))
		{
			TeamModule->GetTeamData(InTeamID).AddMember(this);
			return true;
		}
	}
	return false;
}

bool IDWTeamAgentInterface::JoinTeam(IDWTeamAgentInterface* InTargetCharacter)
{
	return JoinTeam(InTargetCharacter->GetTeamID());
}

bool IDWTeamAgentInterface::LeaveTeam()
{
	if (HasTeam())
	{
		ADWTeamModule::Get()->GetTeamData(GetTeamID()).RemoveMember(this);
		return true;
	}
	return false;
}

bool IDWTeamAgentInterface::AddTeamMate(IDWTeamAgentInterface* InTargetCharacter)
{
	if (HasTeam() && ADWTeamModule::Get()->GetTeamData(GetTeamID()).IsCaptain(this))
	{
		ADWTeamModule::Get()->GetTeamData(GetTeamID()).AddMember(InTargetCharacter);
		return true;
	}
	return false;
}

bool IDWTeamAgentInterface::RemoveTeamMate(IDWTeamAgentInterface* InTargetCharacter)
{
	if (HasTeam() && ADWTeamModule::Get()->GetTeamData(GetTeamID()).IsCaptain(this))
	{
		ADWTeamModule::Get()->GetTeamData(GetTeamID()).RemoveMember(InTargetCharacter);
		return true;
	}
	return false;
}

TArray<IDWTeamAgentInterface*> IDWTeamAgentInterface::GetTeamMates()
{
	return ADWTeamModule::Get()->GetTeamData(GetTeamID()).GetMembers(this);
}
