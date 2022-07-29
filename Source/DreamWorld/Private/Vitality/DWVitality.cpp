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
#include "Inventory/VitalityInventory.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Vitality/DWVitalityData.h"
#include "Vitality/States/DWVitalityState_Death.h"
#include "Vitality/States/DWVitalityState_Default.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Datas/VoxelData.h"
#include "Widget/Components/WidgetVitalityHPComponent.h"
#include "Widget/World/WidgetVitalityHP.h"

// Sets default values
ADWVitality::ADWVitality()
{
	VitalityHP = CreateDefaultSubobject<UWidgetVitalityHPComponent>(FName("VitalityHP"));
	VitalityHP->SetupAttachment(RootComponent);
	VitalityHP->SetRelativeLocation(FVector(0, 0, 50));

	AbilitySystem = CreateDefaultSubobject<UDWAbilitySystemComponent>(FName("AbilitySystem"));

	AttributeSet = CreateDefaultSubobject<UDWVitalityAttributeSet>(FName("AttributeSet"));
	
	Inventory = CreateDefaultSubobject<UVitalityInventory>(FName("Inventory"));

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

	Inventory->UnloadSaveData(true);
}

void ADWVitality::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead()) return;

	Inventory->Refresh(DeltaTime);
}

void ADWVitality::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
}

void ADWVitality::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	Super::LoadData(InSaveData, bForceMode);
	
	auto& SaveData = InSaveData->CastRef<FDWVitalitySaveData>();

	if(bForceMode)
	{
		if(!SaveData.IsSaved())
		{
			const UDWVitalityData& vitalityData = GetVitalityData<UDWVitalityData>();
			if(vitalityData.IsValid())
			{
				SaveData.InventoryData = vitalityData.InventoryData;
			}
			auto PropDatas = UAssetModuleBPLibrary::LoadPrimaryAssets<UAbilityPropDataBase>(UAbilityModuleBPLibrary::ItemTypeToAssetType(EAbilityItemType::Equip));
			const int32 PropNum = FMath::Clamp(FMath::Rand() < 0.2f ? FMath::RandRange(1, 3) : 0, 0, PropDatas.Num());
			for (int32 i = 0; i < PropNum; i++)
			{
				FAbilityItem tmpItem = FAbilityItem(PropDatas[FMath::RandRange(0, PropDatas.Num() - 1)]->GetPrimaryAssetId(), 1);
				SaveData.InventoryData.AddItem(tmpItem);
			}
		}
		Inventory->SetOwnerActor(this);
	}

	Inventory->LoadSaveData(&SaveData.InventoryData, bForceMode);
}

FSaveData* ADWVitality::ToData()
{
	static FDWVitalitySaveData SaveData;
	SaveData = Super::ToData()->CastRef<FVitalitySaveData>();

	SaveData.ID = AssetID;
	SaveData.Name = Name;
	SaveData.RaceID = RaceID;
	SaveData.Level = Level;
	
	SaveData.InventoryData = Inventory->ToSaveDataRef<FInventorySaveData>();

	SaveData.SpawnLocation = GetActorLocation();
	SaveData.SpawnRotation = GetActorRotation();

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

bool ADWVitality::GenerateVoxel(FVoxelItem& InVoxelItem)
{
	bool bSuccess = false;
	FQueryItemInfo QueryItemInfo = Inventory->GetItemInfoByRange(EQueryItemType::Remove, static_cast<FAbilityItem>(InVoxelItem));
	if(QueryItemInfo.Item.IsValid())
	{
		bSuccess = Super::GenerateVoxel(InVoxelItem);
	}
	if(bSuccess) Inventory->RemoveItemBySlots(QueryItemInfo.Item, QueryItemInfo.Slots);
	return bSuccess;
}

bool ADWVitality::GenerateVoxel(FVoxelItem& InVoxelItem, const FVoxelHitResult& InVoxelHitResult)
{
	bool bSuccess = false;
	FQueryItemInfo QueryItemInfo = Inventory->GetItemInfoByRange(EQueryItemType::Remove, static_cast<FAbilityItem>(InVoxelItem));
	if(QueryItemInfo.Item.IsValid())
	{
		bSuccess = Super::GenerateVoxel(InVoxelItem, InVoxelHitResult);
	}
	if(bSuccess) Inventory->RemoveItemBySlots(QueryItemInfo.Item, QueryItemInfo.Slots);
	return bSuccess;
}

bool ADWVitality::DestroyVoxel(FVoxelItem& InVoxelItem)
{
	return Super::DestroyVoxel(InVoxelItem);
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

void ADWVitality::SetLevelV(int32 InLevel)
{
	Super::SetLevelV(InLevel);

	if (GetVitalityHPWidget())
	{
		GetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

UWidgetVitalityHP* ADWVitality::GetVitalityHPWidget() const
{
	if (VitalityHP->GetWorldWidget())
	{
		return Cast<UWidgetVitalityHP>(VitalityHP->GetWorldWidget());
	}
	return nullptr;
}

UInventory* ADWVitality::GetInventory() const
{
	return Inventory;
}

void ADWVitality::OnInventorySlotSelected(UInventorySlot* InInventorySlot)
{
	const FAbilityItem tmpItem = InInventorySlot->GetItem();
	if(tmpItem.IsValid() && tmpItem.GetData().GetItemType() == EAbilityItemType::Voxel)
	{
		SetGenerateVoxelItem(tmpItem);
	}
	else
	{
		SetGenerateVoxelItem(FVoxelItem::EmptyVoxel);
	}
}

void ADWVitality::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	Super::OnAttributeChange(InAttributeChangeData);
	
	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;
	
	if(InAttributeChangeData.Attribute == GetAttributeSet<UDWVitalityAttributeSet>()->GetHealthAttribute())
	{
		if (GetVitalityHPWidget())
		{
			GetVitalityHPWidget()->SetHealthPercent(InAttributeChangeData.NewValue, GetMaxHealth());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWVitalityAttributeSet>()->GetExpAttribute())
	{
		if (GetVitalityHPWidget())
		{
			GetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
		}
	}
	else if(InAttributeChangeData.Attribute == GetAttributeSet<UDWVitalityAttributeSet>()->GetMaxHealthAttribute())
	{
		if (GetVitalityHPWidget())
		{
			GetVitalityHPWidget()->SetHealthPercent(GetHealth(), InAttributeChangeData.NewValue);
		}
	}
}

void ADWVitality::HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleDamage(DamageType, LocalDamageDone, bHasCrited, HitResult, SourceTags, SourceActor);
	
	if(SourceActor && SourceActor != this)
	{
		if(ADWCharacter* SourceVitality = Cast<ADWCharacter>(SourceActor))
		{
			if(DamageType == EDamageType::Physics)
			{
				SourceVitality->ModifyHealth(LocalDamageDone * SourceVitality->GetAttackStealRate());
			}
		}
	}
}
