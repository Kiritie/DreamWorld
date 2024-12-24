#include "Character/DWCharacterData.h"

UDWCharacterData::UDWCharacterData()
{
	MaxLevel = 100;

	Nature = EDWCharacterNature::None;
	AttackDistance = 100.f;
	InteractDistance = 500.f;
	FollowDistance = 300.f;
	PatrolDistance = 1000.f;
	PatrolDuration = 10.f;

	DefaultBehaviorTree = nullptr;
	ExcessiveBehaviorTree = nullptr;
	
	LocalDialogueIndex = -1;
}

void UDWCharacterData::OnReset_Implementation()
{
	Super::OnReset_Implementation();

	LocalDialogueIndex = -1;
}

UDialogue* UDWCharacterData::GetRandomDialogue(FRandomStream RandomStream)
{
	if(Dialogues.Num() > 0)
	{
		if(++LocalDialogueIndex >= Dialogues.Num())
		{
			LocalDialogueIndex = 0;
		}
		return Dialogues[LocalDialogueIndex];
		// return Dialogues[RandomStream.RandRange(0, Dialogues.Num())];
	}
	return nullptr;
}
