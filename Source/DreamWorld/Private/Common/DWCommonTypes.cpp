#include "Common/DWCommonTypes.h"

#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Character/DWCharacter.h"
#include "Ability/Vitality/AbilityVitalityInterface.h"
#include "Scene/SceneModuleStatics.h"

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
			if(DefendRateDone > 0.f && !TargetCharacter->DoAction(EDWCharacterActionType::DefendBlock))
			{
				DefendRateDone = 0.f;
			}
		}
	}

	switch(DamageType)
	{
		case EDamageType::Physics:
		{
			bAttackCrited = FMath::FRand() <= SourceAttackCritRate;
			LocalDamageDone = SourceAttackForce * DamageValue * (1.f - SourcePhysicsDefRate) * (bAttackCrited ? 2.f : 1.f);
			break;
		}
		case EDamageType::Magic:
		{
			LocalDamageDone = DamageValue * (1.f - SourceMagicDefRate);
			break;
		}
	}

	if (DefendRateDone > 0.f)
	{
		if (TargetCharacter)
		{
			USceneModuleStatics::SpawnWorldText(FString::FromInt(LocalDamageDone * DefendRateDone), FColor::Cyan, EWorldTextStyle::Normal, TargetCharacter->GetActorLocation(), FVector(20.f));
		}
		LocalDamageDone *= (1.f - DefendRateDone);
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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_InventoryAll, "Input.Shortcut.InventoryAll", "Inventory All");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_InventorySingle, "Input.Shortcut.InventorySingle", "Inventory Single");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_InventorySplit, "Input.Shortcut.InventorySplit", "Inventory Split");
	
	////////////////////////////////////////////////////
	// Input_Player
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Sprint, "Input.Player.Sprint", "Sprint");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Interact1, "Input.Player.Interact1", "Interact1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Interact2, "Input.Player.Interact2", "Interact2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Interact3, "Input.Player.Interact3", "Interact3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Interact4, "Input.Player.Interact4", "Interact4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Interact5, "Input.Player.Interact5", "Interact5");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Dodge, "Input.Player.Dodge", "Dodge");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ToggleCrouch, "Input.Player.ToggleCrouch", "Toggle Crouch");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ToggleControlMode, "Input.Player.ToggleControlMode", "Toggle Control Mode");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ToggleLockSightTarget, "Input.Player.ToggleLockSightTarget", "Toggle LockSight Target");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ChangeHand, "Input.Player.ChangeHand", "ChangeHand");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Primary, "Input.Player.Primary", "PrimaryAction");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Secondary, "Input.Player.Secondary", "Secondary");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Third, "Input.Player.Third", "Third");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ReleaseSkillAbility1, "Input.Player.ReleaseSkillAbility1", "Release Skill Ability1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ReleaseSkillAbility2, "Input.Player.ReleaseSkillAbility2", "Release Skill Ability2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ReleaseSkillAbility3, "Input.Player.ReleaseSkillAbility3", "Release Skill Ability3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ReleaseSkillAbility4, "Input.Player.ReleaseSkillAbility4", "Release Skill Ability4");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_UseInventoryItem, "Input.Player.UseInventoryItem", "Use Inventory Item");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_DiscardInventoryItem, "Input.Player.DiscardInventoryItem", "Discard Inventory Item");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_PrevInventorySlot, "Input.Player.PrevInventorySlot", "Prev Inventory Slot");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_NextInventorySlot, "Input.Player.NextInventorySlot", "Next Inventory Slot");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SelectInventorySlot1, "Input.Player.SelectInventorySlot1", "Select Inventory Slot1");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SelectInventorySlot2, "Input.Player.SelectInventorySlot2", "Select Inventory Slot2");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SelectInventorySlot3, "Input.Player.SelectInventorySlot3", "Select Inventory Slot3");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SelectInventorySlot4, "Input.Player.SelectInventorySlot4", "Select Inventory Slot4");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SelectInventorySlot5, "Input.Player.SelectInventorySlot5", "Select Inventory Slot5");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SelectInventorySlot6, "Input.Player.SelectInventorySlot6", "Select Inventory Slot6");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SelectInventorySlot7, "Input.Player.SelectInventorySlot7", "Select Inventory Slot7");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SelectInventorySlot8, "Input.Player.SelectInventorySlot8", "Select Inventory Slot8");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SelectInventorySlot9, "Input.Player.SelectInventorySlot9", "Select Inventory Slot9");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_SelectInventorySlot10, "Input.Player.SelectInventorySlot10", "Select Inventory Slot10");
	
	////////////////////////////////////////////////////
	// Input_System
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ZoomInMiniMap, "Input.System.MiniMap.ZoomIn", "ZoomIn MiniMap");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_ZoomOutMiniMap, "Input.System.MiniMap.ZoomOut", "ZoomOut MiniMap");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_OpenMaxMapBox, "Input.System.OpenMaxMapBox", "Open MaxMap Box");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_OpenInventoryPanel, "Input.System.OpenInventoryPanel", "Open Inventory Panel");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_OpenGeneratePanel, "Input.System.OpenGeneratePanel", "Open Generate Panel");

	////////////////////////////////////////////////////
	// State_Character
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Dodging, "State.Character.Dodging", "Character Dodging");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Sprinting, "State.Character.Sprinting", "Character Sprinting");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Crouching, "State.Character.Crouching", "Character Crouching");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Swimming, "State.Character.Swimming", "Character Swimming");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Floating, "State.Character.Floating", "Character Floating");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Climbing, "State.Character.Climbing", "Character Climbing");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Riding, "State.Character.Riding", "Character Riding");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Flying, "State.Character.Flying", "Character Flying");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Attacking, "State.Character.Attacking", "Character Attacking");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_NormalAttacking, "State.Character.NormalAttacking", "Character NormalAttacking");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_FallingAttacking, "State.Character.FallingAttacking", "Character FallingAttacking");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_SkillAttacking, "State.Character.SkillAttacking", "Character SkillAttacking");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Defending, "State.Character.Defending", "Character Defending");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Interrupting, "State.Character.Interrupting", "Character Interrupting");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_Exhausted, "State.Character.Exhausted", "Character Exhausted");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_FreeToAnim, "State.Character.FreeToAnim", "Character FreeToAnim");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_LockRotation, "State.Character.LockRotation", "Character LockRotation");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(StateTag_Character_BreakAllInput, "State.Character.BreakAllInput", "Character BreakAllInput");
}
