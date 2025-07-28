#include "Character/DWCharacterData.h"

UDWCharacterData::UDWCharacterData()
{
	MaxLevel = 100;

	Nature = EDWCharacterNature::None;

	ActiveDistance = 7;
	AttackDistance = 100.f;
	InteractDistance = 500.f;
	FollowDistance = 300.f;
	PatrolDistance = 1000.f;
	PatrolDuration = 10.f;

	DefaultBehaviorTree = nullptr;
	ExcessiveBehaviorTree = nullptr;
}

void UDWCharacterData::OnReset_Implementation()
{
	Super::OnReset_Implementation();
}

UDialogue* UDWCharacterData::GetRandomDialogue(FRandomStream InRandomStream) const
{
	UDialogue* _Dialogue = nullptr;
	if(Dialogues.Num() > 0)
	{
		_Dialogue = Dialogues[InRandomStream.RandRange(0, Dialogues.Num() - 1)];
	}
	return _Dialogue;
}

TArray<FDWCharacterSkinSaveData> UDWCharacterData::GetRandomSkinDatas(FRandomStream InRandomStream) const
{
	TArray<FDWCharacterSkinSaveData> _SkinDatas;
	for (auto& Iter : SkinDatas)
	{
		FDWCharacterSkinSaveData _SkinData;
		_SkinData.MeshDatas = Iter.MeshDatas;
		if (Iter.Materials.Num() > 0)
		{
			_SkinData.Material = Iter.Materials[InRandomStream.RandRange(0, Iter.Materials.Num() - 1)];
		}
		_SkinDatas.Add(_SkinData);
	}
	return _SkinDatas;
}
