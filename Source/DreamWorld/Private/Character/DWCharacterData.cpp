#include "Character/DWCharacterData.h"

UDWCharacterData::UDWCharacterData()
{
	MaxLevel = 50;

	Nature = EDWCharacterNature::None;
	AttackDistance = 100.f;
	InteractDistance = 500.f;
	FollowDistance = 300.f;
	PatrolDistance = 1000.f;
	PatrolDuration = 10.f;
	FallingAttackAbility = FDWCharacterAttackAbilityData();
	FallDamageClass = nullptr;

	DefaultBehaviorTree = nullptr;
	ExcessiveBehaviorTree = nullptr;
}
