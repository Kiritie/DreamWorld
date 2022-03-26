#include "Character/DWCharacterAsset.h"

#include "Ability/AbilityModuleBPLibrary.h"

UDWCharacterAsset::UDWCharacterAsset()
{
	Nature = ECharacterNature::AIHostile;
	AttackDistance = 100.f;
	InteractDistance = 500.f;
	FollowDistance = 500.f;
	PatrolDistance = 1000.f;
	PatrolDuration = 10.f;
	AttackAbilityTable = nullptr;
	SkillAbilityTable = nullptr;
	ActionAbilityTable = nullptr;
	BehaviorTreeAsset = nullptr;
	FallingAttackAbility = FDWCharacterAttackAbilityData();
	InventoryData = FInventorySaveData();
}
