// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/DWVitality.h"

#include "Ability/Components/DWAbilitySystemComponent.h"
#include "Ability/AbilityModuleStatics.h"
#include "Ability/Vitality/DWVitalityAttributeSet.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Character/DWCharacter.h"
#include "FSM/Components/FSMComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Vitality/States/DWVitalityState_Death.h"
#include "Vitality/States/DWVitalityState_Default.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/VoxelModule.h"
#include "Widget/World/WidgetVitalityHP.h"
#include "Widget/World/WorldWidgetComponent.h"
#include "Inventory/DWVitalityInventory.h"

// Sets default values

ADWVitality::ADWVitality(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UDWAbilitySystemComponent>("AbilitySystem").
		SetDefaultSubobjectClass<UDWVitalityAttributeSet>("AttributeSet").
		SetDefaultSubobjectClass<UDWVitalityInventory>("Inventory"))
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

	FSM->DefaultState = UDWVitalityState_Default::StaticClass();
	FSM->States.Empty();
	FSM->States.Add(UDWVitalityState_Default::StaticClass());
	FSM->States.Add(UDWVitalityState_Death::StaticClass());
}

void ADWVitality::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);
}

void ADWVitality::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
}

void ADWVitality::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
}

void ADWVitality::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWVitalitySaveData>();

	if(PHASEC(InPhase, EPhase::PrimaryAndLesser))
	{
		if(!SaveData.InventoryData.IsSaved())
		{
			auto PropDatas = UAssetModuleStatics::LoadPrimaryAssets<UAbilityPropDataBase>(FName("Prop"));
			const int32 PropNum = FMath::Clamp(FMath::Rand() < 0.2f ? FMath::RandRange(1, 3) : 0, 0, PropDatas.Num());
			for (int32 i = 0; i < PropNum; i++)
			{
				FAbilityItem tmpItem = FAbilityItem(PropDatas[FMath::RandRange(0, PropDatas.Num() - 1)]->GetPrimaryAssetId(), 1);
				SaveData.InventoryData.AddItem(tmpItem, { ESlotSplitType::Default });
			}
		}
	}

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

void ADWVitality::OnInteract(EInteractAction InInteractAction, IInteractionAgentInterface* InInteractionAgent, bool bPassivity)
{
	Super::OnInteract(InInteractAction, InInteractionAgent, bPassivity);

	if(!bPassivity) return;
	
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

void ADWVitality::OnActiveItem(const FAbilityItem& InItem, bool bPassive, bool bSuccess)
{
	Super::OnActiveItem(InItem, bPassive, bSuccess);
}

void ADWVitality::OnCancelItem(const FAbilityItem& InItem, bool bPassive)
{
	Super::OnCancelItem(InItem, bPassive);
}

void ADWVitality::OnAssembleItem(const FAbilityItem& InItem)
{
	Super::OnAssembleItem(InItem);
}

void ADWVitality::OnDischargeItem(const FAbilityItem& InItem)
{
	Super::OnDischargeItem(InItem);
}

void ADWVitality::OnDiscardItem(const FAbilityItem& InItem, bool bInPlace)
{
	Super::OnDiscardItem(InItem, bInPlace);
}

void ADWVitality::OnSelectItem(const FAbilityItem& InItem)
{
	Super::OnSelectItem(InItem);
}

void ADWVitality::OnAuxiliaryItem(const FAbilityItem& InItem)
{
	Super::OnAuxiliaryItem(InItem);
}

bool ADWVitality::OnGenerateVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(!GenerateVoxelID.IsValid()) return false;
	
	FItemQueryInfo ItemQueryInfo = Inventory->QueryItemByRange(EItemQueryType::Remove, FAbilityItem(GenerateVoxelID, 1), -1);
	if(ItemQueryInfo.IsValid())
	{
		if(Super::OnGenerateVoxel(InVoxelHitResult))
		{
			Inventory->RemoveItemByQueryInfo(ItemQueryInfo);
			return true;
		}
	}
	return false;
}

bool ADWVitality::OnDestroyVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	return Super::OnDestroyVoxel(InVoxelHitResult);
}

void ADWVitality::SetNameV(FName InName)
{
	Super::SetNameV(InName);

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

void ADWVitality::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;
	
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

void ADWVitality::HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, bool bHasDefend, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleDamage(DamageType, LocalDamageDone, bHasCrited, bHasDefend, HitResult, SourceTags, SourceActor);
	
	if(SourceActor && SourceActor != this)
	{
		if(ADWCharacter* SourceCharacter = Cast<ADWCharacter>(SourceActor))
		{
			if(DamageType == EDamageType::Physics)
			{
				SourceCharacter->ModifyHealth(LocalDamageDone * SourceCharacter->GetAttackStealRate());
			}
		}
	}
}
