#pragma once

#include "DWTeamAgentInterface.generated.h"

UINTERFACE()
class DREAMWORLD_API UDWTeamAgentInterface : public UInterface
{
	GENERATED_BODY()
};

class DREAMWORLD_API IDWTeamAgentInterface
{
	GENERATED_BODY()

public:
	virtual bool HasTeam() const;

	virtual bool IsTeamMate(IDWTeamAgentInterface* InTargetCharacter) const;

	virtual bool CreateTeam(const FName& InTeamName = NAME_None, FString InTeamDetail = TEXT(""));
		
	virtual bool DissolveTeam();

	virtual bool JoinTeam(const FName& InTeamID);
		
	virtual bool JoinTeam(IDWTeamAgentInterface* InTargetCharacter);

	virtual bool LeaveTeam();
		
	virtual bool AddTeamMate(IDWTeamAgentInterface* InTargetCharacter);
	
	virtual bool RemoveTeamMate(IDWTeamAgentInterface* InTargetCharacter);

	virtual TArray<IDWTeamAgentInterface*> GetTeamMates();

public:
	virtual FGuid GetActorIDT() const = 0;
	
	virtual FName GetNameT() const = 0;

	virtual FName GetTeamID() const = 0;

	virtual void SetTeamID(FName InTeamID) = 0;
};