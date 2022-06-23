	// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/DWVitality.h"

#include "Ability/Components/DWAbilitySystemComponent.h"
#include "Ability/Vitality/DWVitalityAttributeSet.h"
#include "Ability/Vitality/AbilityVitalityDataBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vitality/DWVitality.h"

#include "Character/DWCharacter.h"
#include "Inventory/VitalityInventory.h"
#include "Vitality/DWVitalityData.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/Chunks/VoxelChunk.h"
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

	OwnerChunk = nullptr;
}

// Called when the game starts or when spawned
void ADWVitality::BeginPlay()
{
	Super::BeginPlay();

	if (GetWidgetVitalityHPWidget() && !GetWidgetVitalityHPWidget()->GetOwnerObject())
	{
		GetWidgetVitalityHPWidget()->SetOwnerObject(this);
	}

	Spawn();
}

// Called every frame
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

void ADWVitality::LoadData(FSaveData* InSaveData)
{
	auto SaveData = *static_cast<FDWVitalitySaveData*>(InSaveData);
	if (SaveData.bSaved)
	{
		AssetID = SaveData.ID;
		SetNameV(SaveData.Name);
		SetRaceID(SaveData.RaceID);
		SetLevelV(SaveData.Level);
		SetEXP(SaveData.EXP);

		SetActorLocation(SaveData.SpawnLocation);
		SetActorRotation(SaveData.SpawnRotation);

		Inventory->LoadData(SaveData.InventoryData, this);
	}
	else
	{
		AssetID = SaveData.ID;
		SetNameV(SaveData.Name);
		SetRaceID(SaveData.RaceID);
		SetLevelV(SaveData.Level);

		SetActorLocation(SaveData.SpawnLocation);
		SetActorRotation(SaveData.SpawnRotation);

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
		
		Inventory->LoadData(SaveData.InventoryData, this);
	}
}

FSaveData* ADWVitality::ToData(bool bSaved)
{
	static FDWVitalitySaveData SaveData;

	SaveData.bSaved = bSaved;

	SaveData.ID = AssetID;
	SaveData.Name = Name;
	SaveData.RaceID = RaceID;
	SaveData.Level = Level;
	SaveData.EXP = EXP;
	
	SaveData.InventoryData = Inventory->ToData();

	SaveData.SpawnLocation = GetActorLocation();
	SaveData.SpawnRotation = GetActorRotation();

	return &SaveData;
}

void ADWVitality::ResetData()
{
	Super::ResetData();
}

void ADWVitality::Death(AActor* InKiller /*= nullptr*/)
{
	if (!IsDead())
	{
		Inventory->DiscardAllItem();
		OwnerChunk->DestroySceneActor(this);
	}
	Super::Death(InKiller);
}

void ADWVitality::Spawn()
{
	Super::Spawn();
}

void ADWVitality::Revive()
{
	Super::Revive();
}

bool ADWVitality::GenerateVoxel(const FVoxelHitResult& InVoxelHitResult, FAbilityItem& InItem)
{
	return false;
}

bool ADWVitality::DestroyVoxel(const FVoxelHitResult& InVoxelHitResult)
{
	return false;
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

FAbilityItem& ADWVitality::GetGeneratingVoxelItem()
{
	FAbilityItem tmpItem = Inventory->GetSelectedItem();
	if(tmpItem.IsValid() && tmpItem.GetData().EqualType(EAbilityItemType::Voxel))
	{
		return tmpItem;
	}
	return FAbilityItem::Empty;
}

FVoxelItem& ADWVitality::GetSelectedVoxelItem()
{
	return SelectedVoxelItem;
}

UWidgetVitalityHP* ADWVitality::GetWidgetVitalityHPWidget() const
{
	if (WidgetVitalityHP->GetUserWidgetObject())
	{
		return Cast<UWidgetVitalityHP>(WidgetVitalityHP->GetUserWidgetObject());
	}
	return nullptr;
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
