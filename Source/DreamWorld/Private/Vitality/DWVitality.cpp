// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/DWVitality.h"

#include "Ability/AbilityModuleStatics.h"
#include "Ability/Attributes/DWVitalityAttributeSet.h"
#include "Ability/Components/DWAbilitySystemComponent.h"
#include "Ability/Vitality/States/AbilityVitalityState_Spawn.h"
#include "Character/DWCharacter.h"
#include "Event/EventModuleStatics.h"
#include "Event/Handle/Voxel/EventHandle_VoxelWorldModeChanged.h"
#include "FSM/Components/FSMComponent.h"
#include "Inventory/DWVitalityInventory.h"
#include "UObject/ConstructorHelpers.h"
#include "Voxel/DWVoxelChunk.h"
#include "Widget/World/WidgetVitalityHP.h"
#include "Widget/World/WorldWidgetComponent.h"

// Sets default values

ADWVitality::ADWVitality(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UDWAbilitySystemComponent>(FName("AbilitySystem")).
		SetDefaultSubobjectClass<UDWVitalityAttributeSet>(FName("AttributeSet")).
		SetDefaultSubobjectClass<UDWVitalityInventory>(FName("Inventory")))
{
	VitalityHP = CreateDefaultSubobject<UWorldWidgetComponent>(FName("VitalityHP"));
	VitalityHP->SetupAttachment(RootComponent);
	VitalityHP->SetRelativeLocation(FVector(0, 0, 50));
	VitalityHP->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UWidgetVitalityHP> VitalityHPClassFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/World/WBP_VitalityHP.WBP_VitalityHP_C'"));
	if(VitalityHPClassFinder.Succeeded())
	{
		VitalityHP->SetWorldWidgetClass(VitalityHPClassFinder.Class);
	}
}

void ADWVitality::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);

	UEventModuleStatics::SubscribeEvent<UEventHandle_VoxelWorldModeChanged>(this, GET_FUNCTION_NAME_THISCLASS(OnWorldModeChanged));
}

void ADWVitality::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);

	UEventModuleStatics::UnsubscribeEvent<UEventHandle_VoxelWorldModeChanged>(this, GET_FUNCTION_NAME_THISCLASS(OnWorldModeChanged));
}

void ADWVitality::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
}

void ADWVitality::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* ADWVitality::ToData()
{
	static FDWVitalitySaveData SaveData;
	SaveData = Super::ToData()->CastRef<FVitalitySaveData>();

	return &SaveData;
}

void ADWVitality::Death(IAbilityVitalityInterface* InKiller)
{
	Super::Death(InKiller);
}

void ADWVitality::Revive(IAbilityVitalityInterface* InRescuer)
{
	Super::Revive(InRescuer);
}

bool ADWVitality::CanInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent)
{
	switch (InInteractAction)
	{
		case EInteractAction::Revive:
		{
			return !IsDead(true);
		}
		default: break;
	}
	return Super::CanInteract(InInteractAction, InInteractionAgent);
}

void ADWVitality::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassive)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassive);

	if(bPassive)
	{
		switch (InInteractAction)
		{
			case EInteractAction::Revive:
			{
				Revive();
				break;
			}
			default: break;
		}
	}
}

void ADWVitality::OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess)
{
	Super::OnActiveItem(InItem, bPassive, bSuccess);
}

void ADWVitality::OnDeactiveItem(const FAbilityItem& InItem, bool bPassive)
{
	Super::OnDeactiveItem(InItem, bPassive);
}

void ADWVitality::OnDiscardItem(const FAbilityItem& InItem, bool bInPlace)
{
	Super::OnDiscardItem(InItem, bInPlace);
}

void ADWVitality::OnSelectItem(ESlotSplitType InSplitType, const FAbilityItem& InItem)
{
	Super::OnSelectItem(InSplitType, InItem);
}

void ADWVitality::OnAuxiliaryItem(const FAbilityItem& InItem)
{
	Super::OnAuxiliaryItem(InItem);
}

bool ADWVitality::OnGenerateVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(!GenerateVoxelID.IsValid()) return false;
	
	FItemQueryData ItemQueryData = Inventory->QueryItemByRange(EItemQueryType::Remove, FAbilityItem(GenerateVoxelID, 1), -1);
	if(ItemQueryData.IsValid())
	{
		if(Super::OnGenerateVoxel(InVoxelHitResult))
		{
			Inventory->RemoveItemByQueryData(ItemQueryData);
			return true;
		}
	}
	return false;
}

bool ADWVitality::OnDestroyVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	return Super::OnDestroyVoxel(InVoxelHitResult);
}

void ADWVitality::OnWorldModeChanged(UObject* InSender, UEventHandle_VoxelWorldModeChanged* InEventHandle)
{
	if(FSM->IsCurrentStateClass<UAbilityVitalityState_Spawn>()) return;
	
	switch(InEventHandle->WorldMode)
	{
		case EVoxelWorldMode::Preview:
		{
			Static();
			break;
		}
		case EVoxelWorldMode::Default:
		{
			UnStatic();
			break;
		}
		default: break;
	}
}

void ADWVitality::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	if(InAttributeChangeData.Attribute == GetAttributeSet<UDWVitalityAttributeSet>()->GetExpAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWVitalityAttributeSet>()->GetMaxExpAttribute())
	{
		if (GetVitalityHPWidget())
		{
			GetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWVitalityAttributeSet>()->GetHealthAttribute() || InAttributeChangeData.Attribute == GetAttributeSet<UDWVitalityAttributeSet>()->GetMaxHealthAttribute())
	{
		if (GetVitalityHPWidget())
		{
			GetVitalityHPWidget()->SetHealthPercent(GetHealth(), GetMaxHealth());
		}
	}
	Super::OnAttributeChange(InAttributeChangeData);
}

void ADWVitality::HandleDamage(EDamageType DamageType, float DamageValue, bool bHasCrited, bool bHasDefend, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleDamage(DamageType, DamageValue, bHasCrited, bHasDefend, HitResult, SourceTags, SourceActor);
}

void ADWVitality::HandleRecovery(float RecoveryValue, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleRecovery(RecoveryValue, HitResult, SourceTags, SourceActor);
}

void ADWVitality::HandleInterrupt(float InterruptDuration, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleInterrupt(InterruptDuration, HitResult, SourceTags, SourceActor);
}

void ADWVitality::SetNameA(FName InName)
{
	Super::SetNameA(InName);

	if (GetVitalityHPWidget())
	{
		GetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::SetRaceID(FName InRaceID)
{
	Super::SetRaceID(InRaceID);
	
	if (GetVitalityHPWidget())
	{
		GetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

bool ADWVitality::SetLevelA(int32 InLevel)
{
	if(!Super::SetLevelA(InLevel)) return false;

	if (GetVitalityHPWidget())
	{
		GetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
	return true;
}

UWidgetVitalityHP* ADWVitality::GetVitalityHPWidget() const
{
	if (VitalityHP->GetWorldWidget())
	{
		return Cast<UWidgetVitalityHP>(VitalityHP->GetWorldWidget());
	}
	return nullptr;
}
