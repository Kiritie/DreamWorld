#include "Common/DWCommonTypes.h"

#include "Ability/Effects/EffectBase.h"
#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Character/DWCharacter.h"
#include "Ability/Vitality/AbilityVitalityInterface.h"
#include "Character/States/DWCharacterState_Fall.h"
#include "FSM/Components/FSMComponent.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"

void UDWDamageHandle::HandleDamage(AActor* SourceActor, AActor* TargetActor, float DamageValue, EDamageType DamageType, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags)
{
	ADWCharacter* SourceCharacter = Cast<ADWCharacter>(SourceActor);

	ADWCharacter* TargetCharacter = Cast<ADWCharacter>(TargetActor);

	IAbilityVitalityInterface* TargetVitality = Cast<IAbilityVitalityInterface>(TargetActor);
	
	float SourceAttackForce = 0.f;
	float SourceAttackCritRate = 0.f;
	float SourceDefendRate = 0.f;
	float SourceDefendScope = 0.f;
	float SourcePhysicsDefRate = 0.f;
	float SourceMagicDefRate = 0.f;

	float LocalDamageDone = 0.f;
	float DefendRateDone = 0.f;
	bool bAttackCrited = false;
		
	if (SourceCharacter)
	{
		SourceAttackForce = SourceCharacter->GetAttackForce();
		SourceAttackCritRate = SourceCharacter->GetAttackCritRate();
	}
	if (TargetCharacter)
	{
		SourceDefendRate = TargetCharacter->GetDefendRate();
		SourceDefendScope = TargetCharacter->GetDefendScope();
		SourcePhysicsDefRate = TargetCharacter->GetPhysicsDefRate();
		SourceMagicDefRate = TargetCharacter->GetMagicDefRate();

		FVector DamageDirection = SourceActor->GetActorLocation() - TargetActor->GetActorLocation();
		DamageDirection.Normalize();
		if (FVector::DotProduct(DamageDirection, TargetActor->GetActorForwardVector())/* / 90.f*/ > (1.f - SourceDefendScope))
		{
			DefendRateDone = SourceDefendRate * (TargetCharacter->IsDefending() ? 1.f : 0.f);
			if(DefendRateDone > 0.f && !TargetCharacter->DoAction(GameplayTags::Ability_Character_Action_DefendBlock))
			{
				DefendRateDone = 0.f;
				TargetCharacter->UnDefend();
			}
		}
	}

	switch(DamageType)
	{
		case EDamageType::Physics:
		{
			bAttackCrited = FMath::FRand() <= SourceAttackCritRate;
			LocalDamageDone = SourceAttackForce * DamageValue * (1.f - SourcePhysicsDefRate) * (bAttackCrited ? 2.f : 1.f) * (1.f - DefendRateDone);
			if (SourceCharacter)
			{
				UEffectBase* Effect = UObjectPoolModuleStatics::SpawnObject<UEffectBase>();

				FGameplayModifierInfo ModifierInfo;
				ModifierInfo.Attribute = GET_GAMEPLAYATTRIBUTE_PROPERTY(UVitalityAttributeSetBase, Recovery);
				ModifierInfo.ModifierOp = EGameplayModOp::Override;
				ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(LocalDamageDone * SourceCharacter->GetAttackStealRate());

				Effect->Modifiers.Add(ModifierInfo);
				
				FGameplayEffectContextHandle EffectContext = SourceCharacter->GetAbilitySystemComponent()->MakeEffectContext();
				EffectContext.AddSourceObject(SourceCharacter);
				SourceCharacter->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(Effect, 0, EffectContext);

				UObjectPoolModuleStatics::DespawnObject(Effect);
			}
			break;
		}
		case EDamageType::Magic:
		{
			LocalDamageDone = DamageValue * (1.f - SourceMagicDefRate) * (1.f - DefendRateDone);
			break;
		}
		case EDamageType::Fall:
		{
			if(SourceCharacter)
			{
				const float FallHeight = SourceCharacter->GetFSMComponent()->GetStateByClass<UDWCharacterState_Fall>()->GetFallHeight();
				if(FallHeight > 350.f)
				{
					LocalDamageDone = DamageValue * (FallHeight / 10.f);
				}
			}
			break;
		}
	}

	if (LocalDamageDone > 0.f)
	{
		if (TargetVitality && !TargetVitality->IsDead())
		{
			TargetVitality->HandleDamage(DamageType, LocalDamageDone, bAttackCrited, DefendRateDone > 0.f, HitResult, SourceTags, SourceActor);
		}
	}
}

namespace GameplayTags
{
	////////////////////////////////////////////////////
	// Input_Shortcut
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_InventoryAll, "Input.Shortcut.InventoryAll", "Inventory All");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_InventorySingle, "Input.Shortcut.InventorySingle", "Inventory Single");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_InventorySplit, "Input.Shortcut.InventorySplit", "Inventory Split");
	
	////////////////////////////////////////////////////
	// Input_Player
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Sprint, "Input.Player.Sprint", "Sprint");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Interact1, "Input.Player.Interact1", "Interact1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Interact2, "Input.Player.Interact2", "Interact2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Interact3, "Input.Player.Interact3", "Interact3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Interact4, "Input.Player.Interact4", "Interact4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Interact5, "Input.Player.Interact5", "Interact5");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_NextInteract, "Input.Player.NextInteract", "NextInteract");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Dodge, "Input.Player.Dodge", "Dodge");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_ToggleCrouch, "Input.Player.ToggleCrouch", "Toggle Crouch");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_ToggleControlMode, "Input.Player.ToggleControlMode", "Toggle Control Mode");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_ToggleLockSightTarget, "Input.Player.ToggleLockSightTarget", "Toggle LockSight Target");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_ChangeHand, "Input.Player.ChangeHand", "ChangeHand");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Primary, "Input.Player.Primary", "Primary");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Secondary, "Input.Player.Secondary", "Secondary");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Third, "Input.Player.Third", "Third");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_ReleaseSkillAbility1, "Input.Player.ReleaseSkillAbility1", "Release Skill Ability1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_ReleaseSkillAbility2, "Input.Player.ReleaseSkillAbility2", "Release Skill Ability2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_ReleaseSkillAbility3, "Input.Player.ReleaseSkillAbility3", "Release Skill Ability3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_ReleaseSkillAbility4, "Input.Player.ReleaseSkillAbility4", "Release Skill Ability4");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_UseInventoryItem, "Input.Player.UseInventoryItem", "Use Inventory Item");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_DiscardInventoryItem, "Input.Player.DiscardInventoryItem", "Discard Inventory Item");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_PrevInventorySlot, "Input.Player.PrevInventorySlot", "Prev Inventory Slot");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_NextInventorySlot, "Input.Player.NextInventorySlot", "Next Inventory Slot");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SelectInventorySlot1, "Input.Player.SelectInventorySlot1", "Select Inventory Slot1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SelectInventorySlot2, "Input.Player.SelectInventorySlot2", "Select Inventory Slot2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SelectInventorySlot3, "Input.Player.SelectInventorySlot3", "Select Inventory Slot3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SelectInventorySlot4, "Input.Player.SelectInventorySlot4", "Select Inventory Slot4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SelectInventorySlot5, "Input.Player.SelectInventorySlot5", "Select Inventory Slot5");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SelectInventorySlot6, "Input.Player.SelectInventorySlot6", "Select Inventory Slot6");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SelectInventorySlot7, "Input.Player.SelectInventorySlot7", "Select Inventory Slot7");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SelectInventorySlot8, "Input.Player.SelectInventorySlot8", "Select Inventory Slot8");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SelectInventorySlot9, "Input.Player.SelectInventorySlot9", "Select Inventory Slot9");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SelectInventorySlot10, "Input.Player.SelectInventorySlot10", "Select Inventory Slot10");
	
	////////////////////////////////////////////////////
	// Input_System
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_ZoomInMiniMap, "Input.System.MiniMap.ZoomIn", "ZoomIn MiniMap");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_ZoomOutMiniMap, "Input.System.MiniMap.ZoomOut", "ZoomOut MiniMap");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_OpenMaxMapBox, "Input.System.OpenMaxMapBox", "Open MaxMap Box");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_OpenInventoryPanel, "Input.System.OpenInventoryPanel", "Open Inventory Panel");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_OpenGeneratePanel, "Input.System.OpenGeneratePanel", "Open Generate Panel");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_OpenContextInputBox, "Input.System.OpenContextInputBox", "Open Context Input Box");

	////////////////////////////////////////////////////
	// State_Character
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Dodging, "State.Character.Dodging", "Character Dodging");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Sprinting, "State.Character.Sprinting", "Character Sprinting");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Crouching, "State.Character.Crouching", "Character Crouching");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Swimming, "State.Character.Swimming", "Character Swimming");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Floating, "State.Character.Floating", "Character Floating");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Climbing, "State.Character.Climbing", "Character Climbing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Riding, "State.Character.Riding", "Character Riding");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Flying, "State.Character.Flying", "Character Flying");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Aiming, "State.Character.Aiming", "Character Aiming");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Attacking, "State.Character.Attacking", "Character Attacking");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Defending, "State.Character.Defending", "Character Defending");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Interrupting, "State.Character.Interrupting", "Character Interrupting");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Animating, "State.Character.Animating", "Character Animating");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Exhausted, "State.Character.Exhausted", "Character Exhausted");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_FreeToAnim, "State.Character.FreeToAnim", "Character FreeToAnim");

	////////////////////////////////////////////////////
	// Ability_Character_Action
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Death, "Ability.Character.Action.Death", "Character Action Death");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Revive, "Ability.Character.Action.Revive", "Character Action Revive");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Jump, "Ability.Character.Action.Jump", "Character Action Jump");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Fall, "Ability.Character.Action.Fall", "Character Action Fall");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Walk, "Ability.Character.Action.Walk", "Character Action Walk");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Crouch, "Ability.Character.Action.Crouch", "Character Action Crouch");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Dodge, "Ability.Character.Action.Dodge", "Character Action Dodge");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Sprint, "Ability.Character.Action.Sprint", "Character Action Sprint");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Climb, "Ability.Character.Action.Climb", "Character Action Climb");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Swim, "Ability.Character.Action.Swim", "Character Action Swim");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Float, "Ability.Character.Action.Float", "Character Action Float");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Ride, "Ability.Character.Action.Ride", "Character Action Ride");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Fly, "Ability.Character.Action.Fly", "Character Action Fly");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Take, "Ability.Character.Action.Take", "Character Action Take");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Use, "Ability.Character.Action.Use", "Character Action Use");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Discard, "Ability.Character.Action.Discard", "Character Action Discard");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Generate, "Ability.Character.Action.Generate", "Character Action Generate");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Destroy, "Ability.Character.Action.Destroy", "Character Action Destroy");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Attack, "Ability.Character.Action.Attack", "Character Action Attack");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_GetHit, "Ability.Character.Action.GetHit", "Character Action GetHit");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Aim, "Ability.Character.Action.Aim", "Character Action Aim");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Defend, "Ability.Character.Action.Defend", "Character Action Defend");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_DefendBlock, "Ability.Character.Action.DefendBlock", "Character Action DefendBlock");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Interrupt, "Ability.Character.Action.Interrupt", "Character Action Interrupt");
}
