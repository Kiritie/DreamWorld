#include "Dialogue.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"
#include "Engine/GameInstance.h"
#include "DialoguePluginPrivatePCH.h"



UDialogue::UDialogue(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UDialogue::AssignPersistentOuter(UGameInstance * inGameInstance)
{
	PersistentGameInstance = inGameInstance;
}

void UDialogue::CleanOuter()
{
	PersistentGameInstance = nullptr;
}

UWorld* UDialogue::GetWorld() const
{	
	if (PersistentGameInstance)
	{
		return PersistentGameInstance->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

FDialogueNode UDialogue::GetNodeById(int32 id, int32 & index)
{
	index = -1;

	int i = 0;
	for (FDialogueNode FoundNode : Data)
	{
		if (FoundNode.id == id)
		{
			index = i;
			return FoundNode;
		}
		i++;
	}

	FDialogueNode Empty;
	return Empty;
}

FDialogueNode UDialogue::GetNodeById(int32 id)
{
	int32 index;
	return GetNodeById(id, index);
}

FDialogueNode UDialogue::GetFirstNode()
{
	FDialogueNode StartNode = GetNodeById(0);

	if (StartNode.Links.Num() > 0)
	{
		return GetNodeById(StartNode.Links[0]);
	}

	FDialogueNode Empty;
	return Empty;
}

TArray<FDialogueNode> UDialogue::GetNextNodes(FDialogueNode Node)
{
	TArray<FDialogueNode> Output;

	for (int32 foundindex : Node.Links)
	{
		Output.Add(GetNodeById(foundindex));
	}

	return Output;
}

void UDialogue::CallFunctionByName(UObject* Object, FString FunctionName)
{
	FOutputDeviceNull ar;
	Object->CallFunctionByNameWithArguments(*FunctionName, ar, NULL, true);
}

UDialogueConditions::UDialogueConditions() 
{

}

UWorld* UDialogueConditions::GetWorld() const
{
	UDialogue * outerDialogue = GetTypedOuter<UDialogue>();

	if (outerDialogue)
	{
		return outerDialogue->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

UDialogueEvents::UDialogueEvents()
{

}

void UDialogueEvents::RecieveEventTriggered_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
}

bool UDialogueConditions::IsConditionMet_Implementation(APlayerController* ConsideringPlayer, AActor* NPCActor)
{
	return true;
}

UWorld* UDialogueEvents::GetWorld() const
{
	UDialogue * outerDialogue = GetTypedOuter<UDialogue>();

	if (outerDialogue)
	{
		return outerDialogue->GetWorld();
	}
	else
	{
		return nullptr;
	}
}
