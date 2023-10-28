#include "Character/DWCharacterData.h"

UDWCharacterData::UDWCharacterData()
{
	MaxLevel = 50;

	Nature = EDWCharacterNature::AIHostile;
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
}
