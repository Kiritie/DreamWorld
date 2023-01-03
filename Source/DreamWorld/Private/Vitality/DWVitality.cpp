// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/DWVitality.h"

#include "Ability/Components/DWAbilitySystemComponent.h"
#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Vitality/DWVitalityAttributeSet.h"
#include "Ability/Vitality/AbilityVitalityDataBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vitality/DWVitality.h"
#include "Ability/Item/Prop/AbilityPropDataBase.h"
#include "Character/DWCharacter.h"
#include "FSM/Components/FSMComponent.h"
#include "Ability/Inventory/Slot/InventorySlot.h"
#include "UObject/ConstructorHelpers.h"
#include "Vitality/DWVitalityData.h"
#include "Vitality/States/DWVitalityState_Death.h"
#include "Vitality/States/DWVitalityState_Default.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Datas/VoxelData.h"
#include "Widget/World/WidgetVitalityHP.h"
#include "Widget/World/WorldWidgetComponent.h"
#include "Inventory/DWVitalityInventory.h"

// Sets default values
ADWVitality::ADWVitality()
{
	VitalityHP = CreateDefaultSubobject<UWorldWidgetComponent>(FName("VitalityHP"));
	VitalityHP->SetupAttachment(RootComponent);
	VitalityHP->SetRelativeLocation(FVector(0, 0, 50));
	VitalityHP->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UWidgetVitalityHP> VitalityHPClassFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/World/WB_VitalityHP.WB_VitalityHP_C'"));
	if(VitalityHPClassFinder.Succeeded())
	{
		VitalityHP->SetWorldWidgetClass(VitalityHPClassFinder.Class);
	}

	AbilitySystem = CreateDefaultSubobject<UDWAbilitySystemComponent>(FName("AbilitySystem"));

	AttributeSet = CreateDefaultSubobject<UDWVitalityAttributeSet>(FName("AttributeSet"));
	
	Inventory = CreateDefaultSubobject<UDWVitalityInventory>(FName("Inventory"));

	FSM->DefaultState = UDWVitalityState_Default::StaticClass();
	FSM->States.Empty();
	FSM->States.Add(UDWVitalityState_Default::StaticClass());
	FSM->States.Add(UDWVitalityState_Death::StaticClass());
}

// Called when the game starts or when spawned
void ADWVitality::BeginPlay()
{
	Super::BeginPlay();
}

void ADWVitality::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void ADWVitality::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();

	Inventory->UnloadSaveData(EPhase::Primary);
}

void ADWVitality::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead()) return;
}

void ADWVitality::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
}

void ADWVitality::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	auto& SaveData = InSaveData->CastRef<FDWVitalitySaveData>();

	switch(InPhase)
	{
		case EPhase::Primary:
		case EPhase::Lesser:
		{
			break;
		}
		case EPhase::Final:
		{
			if(!SaveData.IsSaved())
			{
				auto PropDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilityPropDataBase>(UAbilityModuleBPLibrary::ItemTypeToAssetType(EAbilityItemType::Prop));
				const int32 PropNum = FMath::Clamp(FMath::Rand() < 0.2f ? FMath::RandRange(1, 3) : 0, 0, PropDatas.Num());
				for (int32 i = 0; i < PropNum; i++)
				{
					FAbilityItem tmpItem = FAbilityItem(PropDatas[FMath::RandRange(0, PropDatas.Num() - 1)]->GetPrimaryAssetId(), 1);
					SaveData.InventoryData.AddItem(tmpItem);
				}
			}
			break;
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

bool ADWVitality::CanInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction)
{
	switch (InInteractAction)
	{
		case EInteractAction::Revive:
		{
			if(!IsDead(true))
			{
				return true;
			}
			break;
		}
		default: return true;
	}
	return Super::CanInteract(InInteractionAgent, InInteractAction);
}

void ADWVitality::OnInteract(IInteractionAgentInterface* InInteractionAgent, EInteractAction InInteractAction)
{
	Super::OnInteract(InInteractionAgent, InInteractAction);
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

bool ADWVitality::GenerateVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	if(!GenerateVoxelID.IsValid()) return false;
	
	FQueryItemInfo QueryItemInfo = Inventory->QueryItemByRange(EQueryItemType::Remove, FAbilityItem(GenerateVoxelID, 1), -1);
	if(QueryItemInfo.IsSuccess())
	{
		if(Super::GenerateVoxel(InVoxelHitResult))
		{
			Inventory->RemoveItemByQueryInfo(QueryItemInfo);
			return true;
		}
	}
	return false;
}

bool ADWVitality::DestroyVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	return Super::DestroyVoxel(InVoxelHitResult);
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

bool ADWVitality::SetLevelV(int32 InLevel)
{
	if(!Super::SetLevelV(InLevel)) return false;

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
