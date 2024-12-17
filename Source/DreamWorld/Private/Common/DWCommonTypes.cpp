#include "Common/DWCommonTypes.h"

#include "Ability/Character/States/AbilityCharacterState_Fall.h"
#include "Ability/Effects/EffectBase.h"
#include "Ability/Item/Coin/AbilityCoinDataBase.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Ability/Item/Raw/AbilityRawDataBase.h"
#include "Ability/Item/Skill/AbilitySkillDataBase.h"
#include "Character/DWCharacter.h"
#include "Ability/Vitality/AbilityVitalityInterface.h"
#include "Asset/AssetModuleStatics.h"
#include "FSM/Components/FSMComponent.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Voxel/Datas/VoxelData.h"

void FDWPlayerSaveData::InitInventoryData(FRandomStream InRandomStream)
{
	switch (InventoryInitType)
	{
		case EDWInventoryInitType::Empty:
		{
			InventoryData.ClearItems();
			break;
		}
		case EDWInventoryInitType::Default:
		{
			FDWPlayerBasicSaveData::InitInventoryData(InRandomStream);
			break;
		}
		case EDWInventoryInitType::All:
		{
			FDWPlayerBasicSaveData::InitInventoryData(InRandomStream);

			auto CoinDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilityCoinDataBase>(FName("Coin"));
			for (int32 i = 0; i < CoinDatas.Num(); i++)
			{
				InventoryData.AddItem(FAbilityItem(CoinDatas[i]->GetPrimaryAssetId(), CoinDatas[i]->MaxCount, CoinDatas[i]->ClampLevel(Level)), true);
			}

			auto EquipDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilityEquipDataBase>(FName("Equip"));
			for (int32 i = 0; i < EquipDatas.Num(); i++)
			{
				InventoryData.AddItem(FAbilityItem(EquipDatas[i]->GetPrimaryAssetId(), EquipDatas[i]->MaxCount, EquipDatas[i]->ClampLevel(Level)), true);
			}

			auto SkillDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilitySkillDataBase>(FName("Skill"));
			for (int32 i = 0; i < SkillDatas.Num(); i++)
			{
				InventoryData.AddItem(FAbilityItem(SkillDatas[i]->GetPrimaryAssetId(), SkillDatas[i]->MaxCount, SkillDatas[i]->ClampLevel(Level)), true);
			}

			auto PropDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilityPropDataBase>(FName("Prop"));
			for (int32 i = 0; i < PropDatas.Num(); i++)
			{
				InventoryData.AddItem(FAbilityItem(PropDatas[i]->GetPrimaryAssetId(), PropDatas[i]->MaxCount, PropDatas[i]->ClampLevel(Level)), true);
			}

			auto RawDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilityRawDataBase>(FName("Raw"));
			for (int32 i = 0; i < RawDatas.Num(); i++)
			{
				InventoryData.AddItem(FAbilityItem(RawDatas[i]->GetPrimaryAssetId(), RawDatas[i]->MaxCount, RawDatas[i]->ClampLevel(Level)), true);
			}

			// auto VoxelDatas = UAssetModuleStatics::LoadPrimaryAssets<UVoxelData>(FName("Voxel"));
			// for (int32 i = 0; i < VoxelDatas.Num(); i++)
			// {
			// 	if(!VoxelDatas[i]->IsEmpty() && !VoxelDatas[i]->IsUnknown() && VoxelDatas[i]->IsMainPart())
			// 	{
			// 		InventoryData.AddItem(FAbilityItem(VoxelDatas[i]->GetPrimaryAssetId(), VoxelDatas[i]->MaxCount, VoxelDatas[i]->ClampLevel(Level)), true);
			// 	}
			// }
			break;
		}
		default: break;
	}
}

void UDWDamageHandle::HandleDamage(AActor* SourceActor, AActor* TargetActor, float DamageValue, const FGameplayAttribute& DamageAttribute, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags)
{
	IAbilityVitalityInterface* SourceVitality = Cast<IAbilityVitalityInterface>(SourceActor);
	
	IAbilityVitalityInterface* TargetVitality = Cast<IAbilityVitalityInterface>(TargetActor);

	if(!SourceVitality || !TargetVitality) return;
	
	ADWCharacter* SourceCharacter = Cast<ADWCharacter>(SourceActor);

	ADWCharacter* TargetCharacter = Cast<ADWCharacter>(TargetActor);

	float SourceAttackCritRate = 0.f;
	float SourceAttackStealRate = 0.f;

	float TargetDefendRate = 0.f; 
	float TargetDefendScopeRate = 0.f;
	float TargetPhysicsRes = 0.f;
	float TargetMagicRes = 0.f;
	
	if(SourceCharacter)
	{
		SourceAttackCritRate = SourceCharacter->GetAttackCritRate();
		SourceAttackStealRate = SourceCharacter->GetAttackStealRate();
	}
	
	if(TargetCharacter)
	{
		TargetDefendRate = TargetCharacter->GetDefendRate();
		TargetDefendScopeRate = TargetCharacter->GetDefendScopeRate();
		
		TargetPhysicsRes = TargetCharacter->GetPhysicsRes();
		TargetMagicRes = TargetCharacter->GetMagicRes();
	}

	bool bAttackHasCrited = FMath::FRand() <= SourceAttackCritRate;

	float PhysicsDefendRate = TargetPhysicsRes / (100.f + TargetPhysicsRes);
	float MagicDefendRate = TargetMagicRes / (100.f + TargetMagicRes);

	float LocalDamageValue = 0.f;
	float LocalDefendValue = 0.f;
	float LocalDefendRate = 0.f;

	FVector DamageDirection = SourceActor->GetActorLocation() - TargetActor->GetActorLocation();
	DamageDirection.Normalize();
	if(FVector::DotProduct(DamageDirection, TargetActor->GetActorForwardVector()) > (1.f - TargetDefendScopeRate))
	{
		LocalDefendRate = TargetDefendRate;
	}

	if(DamageAttribute == GET_GAMEPLAYATTRIBUTE(UVitalityAttributeSetBase, PhysicsDamage))
	{
		LocalDamageValue = DamageValue * (1.f - PhysicsDefendRate) * (bAttackHasCrited ? 2.f : 1.f);
	}
	else if(DamageAttribute == GET_GAMEPLAYATTRIBUTE(UVitalityAttributeSetBase, MagicDamage))
	{
		LocalDamageValue = DamageValue * (1.f - MagicDefendRate) * (bAttackHasCrited ? 2.f : 1.f);
	}
	else if(DamageAttribute == GET_GAMEPLAYATTRIBUTE(UVitalityAttributeSetBase, FallDamage))
	{
		if(SourceCharacter)
		{
			const float FallHeight = SourceCharacter->GetFSMComponent()->GetStateByClass<UAbilityCharacterState_Fall>()->GetFallHeight();
			if(FallHeight > 350.f)
			{
				LocalDamageValue = DamageValue * (1.f - PhysicsDefendRate) * (FallHeight / 10.f);
			}
		}
	}

	const float _DamageValue = LocalDamageValue;
	LocalDamageValue *= (1.f - LocalDefendRate);
	LocalDefendValue = _DamageValue - LocalDamageValue;

	LocalDamageValue = FMath::Clamp(LocalDamageValue, 0.f, TargetVitality->GetHealth());

	if(LocalDamageValue > 0.f)
	{
		TargetVitality->HandleDamage(DamageAttribute, LocalDamageValue, LocalDefendValue, bAttackHasCrited, HitResult, SourceTags, SourceActor);

		if(SourceAttackStealRate > 0.f && SourceVitality != TargetVitality)
		{
			UEffectBase* Effect = UObjectPoolModuleStatics::SpawnObject<UEffectBase>();

			FGameplayModifierInfo ModifierInfo;
			ModifierInfo.Attribute = GET_GAMEPLAYATTRIBUTE(UVitalityAttributeSetBase, HealthRecovery);
			ModifierInfo.ModifierOp = EGameplayModOp::Override;
			ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(LocalDamageValue * SourceAttackStealRate);

			Effect->Modifiers.Add(ModifierInfo);
		
			FGameplayEffectContextHandle EffectContext = SourceVitality->GetAbilitySystemComponent()->MakeEffectContext();
			EffectContext.AddSourceObject(SourceActor);
			SourceVitality->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(Effect, 0, EffectContext);

			UObjectPoolModuleStatics::DespawnObject(Effect);
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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_OpenTaskPanel, "Input.System.OpenTaskPanel", "Open Task Panel");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_OpenContextInputBox, "Input.System.OpenContextInputBox", "Open Context Input Box");

	////////////////////////////////////////////////////
	// State_Character
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Dodging, "State.Character.Dodging", "Character Dodging");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Sprinting, "State.Character.Sprinting", "Character Sprinting");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Riding, "State.Character.Riding", "Character Riding");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Aiming, "State.Character.Aiming", "Character Aiming");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Attacking, "State.Character.Attacking", "Character Attacking");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Defending, "State.Character.Defending", "Character Defending");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Character_Exhausted, "State.Character.Exhausted", "Character Exhausted");

	////////////////////////////////////////////////////
	// Ability_Character_Action
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Dodge, "Ability.Character.Action.Dodge", "Character Action Dodge");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Sprint, "Ability.Character.Action.Sprint", "Character Action Sprint");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Ride, "Ability.Character.Action.Ride", "Character Action Ride");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Take, "Ability.Character.Action.Take", "Character Action Take");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Use, "Ability.Character.Action.Use", "Character Action Use");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Discard, "Ability.Character.Action.Discard", "Character Action Discard");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Generate, "Ability.Character.Action.Generate", "Character Action Generate");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Destroy, "Ability.Character.Action.Destroy", "Character Action Destroy");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Aim, "Ability.Character.Action.Aim", "Character Action Aim");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Attack, "Ability.Character.Action.Attack", "Character Action Attack");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_GetHit, "Ability.Character.Action.GetHit", "Character Action GetHit");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_Defend, "Ability.Character.Action.Defend", "Character Action Defend");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Character_Action_DefendBlock, "Ability.Character.Action.DefendBlock", "Character Action DefendBlock");
}
