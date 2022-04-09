// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SaveGame/SaveGameModuleTypes.h"

#include "InventoryTypes.generated.h"

class UWidgetInventorySlot;
class UInventorySlot;

USTRUCT(BlueprintType)
struct DREAMWORLD_API FQueryItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FAbilityItem Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UInventorySlot*> Slots;

public:
	FORCEINLINE FQueryItemInfo()
	{
		Item = FAbilityItem();
		Slots = TArray<UInventorySlot*>();
	}

	FORCEINLINE FQueryItemInfo(FAbilityItem InItem, TArray<UInventorySlot*> InSlots)
	{
		Item = InItem;
		Slots = InSlots;
	}
};

UENUM(BlueprintType)
enum class ESplitSlotType : uint8
{
	None,
	Default,
	Shortcut,
	Auxiliary,
	Generate,
	Equip,
	Skill
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FSplitSlotInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 StartIndex;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 TotalCount;

	FORCEINLINE FSplitSlotInfo()
	{
		StartIndex = 0;
		TotalCount = 0;
	}

	FORCEINLINE FSplitSlotInfo(int32 InStartIndex, int32 InTotalCount)
	{
		StartIndex = InStartIndex;
		TotalCount = InTotalCount;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FSplitSlotData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UInventorySlot*> Slots;

	FORCEINLINE FSplitSlotData()
	{
		Slots = TArray<UInventorySlot*>();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FWidgetSplitSlotData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UWidgetInventorySlot*> Slots;

	FORCEINLINE FWidgetSplitSlotData()
	{
		Slots = TArray<UWidgetInventorySlot*>();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FInventorySaveData : public FSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ESplitSlotType, FSplitSlotInfo> SplitInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityItem> Items;
		
	FORCEINLINE FInventorySaveData()
	{
		SplitInfos = TMap<ESplitSlotType, FSplitSlotInfo>();
		Items = TArray<FAbilityItem>();
	}
};

/**
 * ?????????????
 */
UENUM(BlueprintType)
enum class EQueryItemType : uint8
{
	// ???
	Addition,
	// ???
	Remove,
	// ???
	Clear
};
