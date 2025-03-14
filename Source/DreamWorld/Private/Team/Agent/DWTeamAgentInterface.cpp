#include "Team/Agent/DWTeamAgentInterface.h"

#include "Team/DWTeamModule.h"

bool IDWTeamAgentInterface::HasTeam() const
{
	return UDWTeamModule::Get().GetTeamData(GetTeamID()).IsValid();
}

bool IDWTeamAgentInterface::IsTeamMate(IDWTeamAgentInterface* InTargetCharacter) const
{
	return InTargetCharacter != this && !GetTeamID().IsNone() && InTargetCharacter->GetTeamID() == GetTeamID();
}

bool IDWTeamAgentInterface::CreateTeam(const FName& InTeamName /*= MANE_None*/, FString InTeamDetail /*= TEXT("")*/)
{
	return UDWTeamModule::Get().CreateTeam(this, InTeamName, InTeamDetail);
}

bool IDWTeamAgentInterface::DissolveTeam()
{
	return UDWTeamModule::Get().DissolveTeam(GetTeamID(), this);
}

bool IDWTeamAgentInterface::JoinTeam(const FName& InTeamID)
{
	if(UDWTeamModule::Get().IsExistTeam(InTeamID))
	{
		UDWTeamModule::Get().GetTeamData(InTeamID).AddMember(this);
		return true;
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
		UDWTeamModule::Get().GetTeamData(GetTeamID()).RemoveMember(this);
		return true;
	}
	return false;
}

bool IDWTeamAgentInterface::AddTeamMate(IDWTeamAgentInterface* InTargetCharacter)
{
	if (HasTeam() && UDWTeamModule::Get().GetTeamData(GetTeamID()).IsCaptain(this))
	{
		UDWTeamModule::Get().GetTeamData(GetTeamID()).AddMember(InTargetCharacter);
		return true;
	}
	return false;
}

bool IDWTeamAgentInterface::RemoveTeamMate(IDWTeamAgentInterface* InTargetCharacter)
{
	if (HasTeam() && UDWTeamModule::Get().GetTeamData(GetTeamID()).IsCaptain(this))
	{
		UDWTeamModule::Get().GetTeamData(GetTeamID()).RemoveMember(InTargetCharacter);
		return true;
	}
	return false;
}

TArray<IDWTeamAgentInterface*> IDWTeamAgentInterface::GetTeamMates()
{
	return UDWTeamModule::Get().GetTeamData(GetTeamID()).GetMembers(this);
}
