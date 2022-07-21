// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/DWVitality.h"

#include "Ability/Components/DWAbilitySystemComponent.h"
#include "Ability/Vitality/DWVitalityAttributeSet.h"
#include "Ability/Vitality/AbilityVitalityDataBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vitality/DWVitality.h"

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
	WidgetVitalityHP = CreateDefaultSubobject<UWidgetVitalityHPComponent>(FName("WidgetVitalityHP"));
	WidgetVitalityHP->SetupAttachment(RootComponent);
	WidgetVitalityHP->SetRelativeLocation(FVector(0, 0, 50));

	AbilitySystem = CreateDefaultSubobject<UDWAbilitySystemComponent>(FName("AbilitySystem"));

	AttributeSet = CreateDefaultSubobject<UDWVitalityAttributeSet>(FName("AttributeSet"));
	
	Inventory = CreateDefaultSubobject<UVitalityInventory>(FName("Inventory"));
	Inventory->SetOwnerActor(this);
	Inventory->GetOnSlotSelected().AddDynamic(this, &ADWVitality::OnInventorySlotSelected);

	FSM->DefaultState = UDWVitalityState_Default::StaticClass();
	FSM->States.Empty();
	FSM->States.Add(UDWVitalityState_Default::StaticClass());
	FSM->States.Add(UDWVitalityState_Death::StaticClass());
}

// Called when the game starts or when spawned
void ADWVitality::BeginPlay()
{
	Super::BeginPlay();

	if (GetWidgetVitalityHPWidget() && !GetWidgetVitalityHPWidget()->GetOwnerObject())
	{
		GetWidgetVitalityHPWidget()->SetOwnerObject(this);
	}
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
	
	auto SaveData = InSaveData->CastRef<FDWVitalitySaveData>();

	if(bForceMode)
	{
		if(!SaveData.IsSaved())
		{
			const UDWVitalityData& vitalityData = GetVitalityData<UDWVitalityData>();
			if(vitalityData.IsValid())
			{
				SaveData.InventoryData = vitalityData.InventoryData;
			}
			// const auto ItemDatas = UDWHelper::LoadItemDatas();
			// if(ItemDatas.Num() > 0 && FMath::FRand() < 0.2f)
			// {
			// 	SaveData.InventoryData.Items.Add(FAbilityItem(ItemDatas[FMath::RandRange(0, ItemDatas.Num() - 1)].ID, 1));
			// }
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
	SaveData.EXP = EXP;
	
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

	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::SetRaceID(FName InRaceID)
{
	Super::SetRaceID(InRaceID);
	
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::SetLevelV(int32 InLevel)
{
	Super::SetLevelV(InLevel);

	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::SetEXP(int32 InEXP)
{
	Super::SetEXP(InEXP);

	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

UWidgetVitalityHP* ADWVitality::GetWidgetVitalityHPWidget() const
{
	if (WidgetVitalityHP->GetUserWidgetObject())
	{
		return Cast<UWidgetVitalityHP>(WidgetVitalityHP->GetUserWidgetObject());
	}
	return nullptr;
}

void ADWVitality::OnInventorySlotSelected(UInventorySlot* InInventorySlot)
{
	const FAbilityItem tmpItem = InInventorySlot->GetItem();
	if(tmpItem.IsValid() && tmpItem.GetData().EqualType(EAbilityItemType::Voxel))
	{
		GeneratingVoxelItem = tmpItem;
	}
}

void ADWVitality::OnAttributeChange(const FOnAttributeChangeData& InAttributeChangeData)
{
	Super::OnAttributeChange(InAttributeChangeData);
	
	const float DeltaValue = InAttributeChangeData.NewValue - InAttributeChangeData.OldValue;
	if(InAttributeChangeData.Attribute == Cast<UDWVitalityAttributeSet>(AttributeSet)->GetHealthAttribute())
	{
		if (GetWidgetVitalityHPWidget())
		{
			GetWidgetVitalityHPWidget()->SetHealthPercent(InAttributeChangeData.NewValue, GetMaxHealth());
		}
	}
	else if(InAttributeChangeData.Attribute == Cast<UDWVitalityAttributeSet>(AttributeSet)->GetMaxHealthAttribute())
	{
		if (GetWidgetVitalityHPWidget())
		{
			GetWidgetVitalityHPWidget()->SetHealthPercent(GetHealth(), InAttributeChangeData.NewValue);
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
