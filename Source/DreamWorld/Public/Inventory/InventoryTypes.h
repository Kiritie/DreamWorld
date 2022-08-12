// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SaveGame/SaveGameModuleTypes.h"

#include "InventoryTypes.generated.h"

class UWidgetInventorySlot;
class UInventorySlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedItemChange, UInventorySlot*, InInventorySlot);

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
struct DREAMWORLD_API FQueryItemInfo
{
	GENERATED_BODY()

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FAbilityItem Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UInventorySlot*> Slots;

public:
	FORCEINLINE bool IsSuccess() const
	{
		return Item.IsValid(true);
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FSplitSlotInfo
{
	GENERATED_BODY()

public:
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 StartIndex;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 TotalCount;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FSplitSlotData
{
	GENERATED_BODY()

public:
	FORCEINLINE FSplitSlotData()
	{
		Slots = TArray<UInventorySlot*>();
	}
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UInventorySlot*> Slots;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FWidgetSplitSlotData
{
	GENERATED_BODY()

public:
	FORCEINLINE FWidgetSplitSlotData()
	{
		Slots = TArray<UWidgetInventorySlot*>();
	}

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UWidgetInventorySlot*> Slots;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FInventorySaveData : public FSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FInventorySaveData()
	{
		SplitInfos = TMap<ESplitSlotType, FSplitSlotInfo>();
		Items = TArray<FAbilityItem>();
		SelectedIndex = -1;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ESplitSlotType, FSplitSlotInfo> SplitInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityItem> Items;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SelectedIndex;

public:
	virtual void Reset() override
	{
		FSaveData::Reset();

		SplitInfos.Empty();
		Items.Empty();
		SelectedIndex = -1;
	}

	FSplitSlotInfo GetSplitSlotInfo(ESplitSlotType InSplitSlotType) const
	{
		if(SplitInfos.Contains(InSplitSlotType))
		{
			return SplitInfos[InSplitSlotType];
		}
		return FSplitSlotInfo();
	}

	void AddItem(const FAbilityItem& InItem, ESplitSlotType InSplitSlotType = ESplitSlotType::None)
	{
		FSplitSlotInfo SplitSlotInfo = GetSplitSlotInfo(InSplitSlotType);
		for(int32 i = SplitSlotInfo.StartIndex; i < (SplitSlotInfo.TotalCount > 0 ? SplitSlotInfo.TotalCount : Items.Num()); i++)
		{
			if(Items.IsValidIndex(i) && !Items[i].IsValid())
			{
				Items[i] = InItem;
				break;
			}
		}
	}
};

/**
 * 查询项类型
 */
UENUM(BlueprintType)
enum class EQueryItemType : uint8
{
	// 获取
	Get,
	// 添加
	Add,
	// 移除
	Remove,
};
