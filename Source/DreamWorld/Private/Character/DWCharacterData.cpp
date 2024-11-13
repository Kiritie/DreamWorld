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
}
