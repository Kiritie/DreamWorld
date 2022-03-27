// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Inventory.generated.h"

class UInventorySlot;

/**
 * ��Ʒ��
 */
UCLASS(DefaultToInstanced, Blueprintable)
class DREAMWORLD_API UInventory : public UObject
{
	GENERATED_BODY()

public:
	UInventory();
	
	//////////////////////////////////////////////////////////////////////////
	/// Properties
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	TMap<ESplitSlotType, FSplitSlotInfo> SplitInfos;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<UInventorySlot*> Slots;
				
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	AActor* OwnerActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	UInventory* ConnectInventory;
				
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	UInventorySlot* SelectedSlot;

	//////////////////////////////////////////////////////////////////////////
	/// Initialize
public:
	virtual void Initialize(AActor* InOwner, TMap<ESplitSlotType, FSplitSlotInfo> InSplitInfos);
	
	virtual void Refresh(float DeltaSeconds);

	virtual void LoadData(FInventorySaveData InInventoryData, AActor* InOwner);

	virtual FInventorySaveData ToData(bool bSaved = true);

	//////////////////////////////////////////////////////////////////////////
	/// Actions
public:
	UFUNCTION(BlueprintPure)
	virtual FQueryItemInfo GetItemInfoByRange(EQueryItemType InQueryType, FItem InItem, int32 InStartIndex = 0, int32 InEndIndex = -1);

	UFUNCTION(BlueprintPure)
	virtual FQueryItemInfo GetItemInfoBySplitType(EQueryItemType InQueryType, FItem InItem, ESplitSlotType InSplitSlotType);
		
	UFUNCTION(BlueprintCallable)
	virtual void AdditionItemBySlots(FItem& InItem, const TArray<UInventorySlot*>& InSlots);

	UFUNCTION(BlueprintCallable)
	virtual void AdditionItemByRange(FItem& InItem, int32 InStartIndex = 0, int32 InEndIndex = -1);

	UFUNCTION(BlueprintCallable)
	virtual void AdditionItemBySplitType(FItem& InItem, ESplitSlotType InSplitSlotType);
		
	UFUNCTION(BlueprintCallable)
	virtual void RemoveItemBySlots(FItem& InItem, const TArray<UInventorySlot*>& InSlots);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItemByRange(FItem& InItem, int32 InStartIndex = 0, int32 InEndIndex = -1);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItemBySplitType(FItem& InItem, ESplitSlotType InSplitSlotType);

	UFUNCTION(BlueprintCallable)
	virtual void MoveItemByRange(UInventory* InTargetInventory, FItem& InItem, int32 InSelfStartIndex = 0, int32 InSelfEndIndex = -1, int32 InTargetStartIndex = 0, int32 InTargetEndIndex = -1);

	UFUNCTION(BlueprintCallable)
	virtual void MoveItemBySplitType(UInventory* InTargetInventory, FItem& InItem, ESplitSlotType InSelfSplitSlotType, ESplitSlotType InTargetSplitSlotType);

	UFUNCTION(BlueprintCallable)
	virtual void ClearItem(FItem& InItem);

	UFUNCTION(BlueprintCallable)
	virtual void ClearAllItem();

	UFUNCTION(BlueprintCallable)
	virtual void DiscardAllItem();

	//////////////////////////////////////////////////////////////////////////
	/// Setter/Getter
public:
	UFUNCTION(BlueprintPure)
	AActor* GetOwnerActor() const { return OwnerActor; }
	
	UFUNCTION(BlueprintCallable)
	void SetOwnerActor(AActor* val) { OwnerActor = val; }
	
	UFUNCTION(BlueprintPure)
	UInventory* GetConnectInventory() const { return ConnectInventory; }

	UFUNCTION(BlueprintCallable)
	void SetConnectInventory(UInventory* val) { ConnectInventory = val; }
	
	UFUNCTION(BlueprintPure)
	int32 GetSlotsNum() const;

	UFUNCTION(BlueprintPure)
	TArray<UInventorySlot*> GetSlots() const { return Slots; }
					
	UFUNCTION(BlueprintPure)
	UInventorySlot* GetSelectedSlot() const { return SelectedSlot; }

	UFUNCTION(BlueprintCallable)
	void SetSelectedSlot(UInventorySlot* InSelectedSlot) { this->SelectedSlot = InSelectedSlot; }
	
	UFUNCTION(BlueprintPure)
	FItem& GetSelectedItem() const;

	UFUNCTION(BlueprintPure)
	bool HasSplitSlotInfo(ESplitSlotType InSplitSlotType) const;

	UFUNCTION(BlueprintPure)
	FSplitSlotInfo GetSplitSlotInfo(ESplitSlotType InSplitSlotType) const;
	
	UFUNCTION(BlueprintPure)
	TMap<ESplitSlotType, FSplitSlotInfo> GetSplitSlotInfos() const { return SplitInfos; }

	template<typename T>
	TArray<T*> GetSplitSlots(ESplitSlotType InSplitSlotType)
	{
		TArray<T*> SplitSlots = TArray<T*>();
		for (auto Iter : SplitInfos)
		{
			if(Iter.Key == InSplitSlotType)
			{
				for (int32 i = Iter.Value.StartIndex; i < Iter.Value.StartIndex + Iter.Value.TotalCount; i++)
				{
					SplitSlots.Add(Cast<T>(Slots[i]));
				}
				break;
			}
		}
		return SplitSlots;
	}
		
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetSplitSlots"))
	TArray<UInventorySlot*> K2_GetSplitSlots(ESplitSlotType InSplitSlotType);

	UFUNCTION(BlueprintPure)
	TMap<ESplitSlotType, FSplitSlotData> GetSplitSlotDatas();
};
