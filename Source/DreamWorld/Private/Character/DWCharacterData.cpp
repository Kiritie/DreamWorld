#include "Character/DWCharacterData.h"

#include "Ability/AbilityModuleBPLibrary.h"

UDWCharacterData::UDWCharacterData()
{
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
	InventoryData = FInventorySaveData();

	// tags
	DeadTag = FGameplayTag::RequestGameplayTag("State.Vitality.Dead");
	DyingTag = FGameplayTag::RequestGameplayTag("State.Vitality.Dying");
	ActiveTag = FGameplayTag::RequestGameplayTag("State.Character.Active");
	FallingTag = FGameplayTag::RequestGameplayTag("State.Character.Falling");
	WalkingTag = FGameplayTag::RequestGameplayTag("State.Character.Walking");
	JumpingTag = FGameplayTag::RequestGameplayTag("State.Character.Jumping");
	DodgingTag = FGameplayTag::RequestGameplayTag("State.Character.Dodging");
	SprintingTag = FGameplayTag::RequestGameplayTag("State.Character.Sprinting");
	CrouchingTag = FGameplayTag::RequestGameplayTag("State.Character.Crouching");
	SwimmingTag = FGameplayTag::RequestGameplayTag("State.Character.Swimming");
	FloatingTag = FGameplayTag::RequestGameplayTag("State.Character.Floating");
	ClimbingTag = FGameplayTag::RequestGameplayTag("State.Character.Climbing");
	RidingTag = FGameplayTag::RequestGameplayTag("State.Character.Riding");
	FlyingTag = FGameplayTag::RequestGameplayTag("State.Character.Flying");
	AttackingTag = FGameplayTag::RequestGameplayTag("State.Character.Attacking");
	NormalAttackingTag = FGameplayTag::RequestGameplayTag("State.Character.NormalAttacking");
	FallingAttackingTag = FGameplayTag::RequestGameplayTag("State.Character.FallingAttacking");
	SkillAttackingTag = FGameplayTag::RequestGameplayTag("State.Character.SkillAttacking");
	DefendingTag = FGameplayTag::RequestGameplayTag("State.Character.Defending");
	InterruptingTag = FGameplayTag::RequestGameplayTag("State.Character.Interrupting");
	ExhaustedTag = FGameplayTag::RequestGameplayTag("State.Character.Exhausted");
	FreeToAnimTag = FGameplayTag::RequestGameplayTag("State.Character.FreeToAnim");
	LockRotationTag = FGameplayTag::RequestGameplayTag("State.Character.LockRotation");
	BreakAllInputTag = FGameplayTag::RequestGameplayTag("State.Character.BreakAllInput");
}
