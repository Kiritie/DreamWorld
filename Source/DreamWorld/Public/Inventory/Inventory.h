// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Global/Base/WHObject.h"
#include "SaveGame/Base/SaveDataInterface.h"
#include "Inventory.generated.h"

/**
 * ��Ʒ��
 */
UCLASS(DefaultToInstanced, Blueprintable)
class DREAMWORLD_API UInventory : public UWHObject, public ISaveDataInterface
{
	GENERATED_BODY()

public:
	UInventory();
	
	//////////////////////////////////////////////////////////////////////////
	/// Properties
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	TMap<ESplitSlotType, FSplitSlotInfo> SplitInfos;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	TArray<UInventorySlot*> Slots;
				
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	AActor* OwnerActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	UInventory* ConnectInventory;
				
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Default")
	UInventorySlot* SelectedSlot;

	UPROPERTY(BlueprintAssignable)
	FOnSelectedItemChange OnSlotSelected;
	
	//////////////////////////////////////////////////////////////////////////
	/// Data
protected:
	virtual void LoadData(FSaveData* InSaveData, bool bForceMode) override;

	virtual FSaveData* ToData() override;

	virtual void UnloadData(bool bForceMode) override;

	//////////////////////////////////////////////////////////////////////////
	/// Actions
public:
	UFUNCTION(BlueprintPure)
	virtual FQueryItemInfo QueryItemByRange(EQueryItemType InQueryType, FAbilityItem InItem, int32 InStartIndex = 0, int32 InEndIndex = -1);

	UFUNCTION(BlueprintPure)
	virtual FQueryItemInfo QueryItemBySplitType(EQueryItemType InQueryType, FAbilityItem InItem, ESplitSlotType InSplitSlotType);

public:
	UFUNCTION(BlueprintCallable)
	virtual void AddItemBySlots(FAbilityItem& InItem, const TArray<UInventorySlot*>& InSlots);

	UFUNCTION(BlueprintCallable)
	virtual void AddItemByRange(FAbilityItem& InItem, int32 InStartIndex = 0, int32 InEndIndex = -1);

	UFUNCTION(BlueprintCallable)
	virtual void AddItemBySplitType(FAbilityItem& InItem, ESplitSlotType InSplitSlotType);
		
	UFUNCTION(BlueprintCallable)
	virtual void AddItemByQueryInfo(FQueryItemInfo& InQueryInfo);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItemBySlots(FAbilityItem& InItem, const TArray<UInventorySlot*>& InSlots);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItemByRange(FAbilityItem& InItem, int32 InStartIndex = 0, int32 InEndIndex = -1);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItemBySplitType(FAbilityItem& InItem, ESplitSlotType InSplitSlotType);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItemByQueryInfo(FQueryItemInfo& InQueryInfo);

	UFUNCTION(BlueprintCallable)
	virtual void MoveItemByRange(UInventory* InTargetInventory, FAbilityItem& InItem, int32 InSelfStartIndex = 0, int32 InSelfEndIndex = -1, int32 InTargetStartIndex = 0, int32 InTargetEndIndex = -1);

	UFUNCTION(BlueprintCallable)
	virtual void MoveItemBySplitType(UInventory* InTargetInventory, FAbilityItem& InItem, ESplitSlotType InSelfSplitSlotType, ESplitSlotType InTargetSplitSlotType);

	UFUNCTION(BlueprintCallable)
	virtual void ClearItem(FAbilityItem& InItem);

	UFUNCTION(BlueprintCallable)
	virtual void ClearAllItem();

	UFUNCTION(BlueprintCallable)
	virtual void DiscardAllItem();

	//////////////////////////////////////////////////////////////////////////
	/// Setter/Getter
public:
	template<class T>
	T* GetOwnerActor() const
	{
		return Cast<T>(OwnerActor);
	}
	
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
	void SetSelectedSlot(UInventorySlot* InSelectedSlot);

	FOnSelectedItemChange& GetOnSlotSelected() { return OnSlotSelected; }
	
	UFUNCTION(BlueprintPure)
	FAbilityItem& GetSelectedItem() const;

	UFUNCTION(BlueprintPure)
	bool HasSplitSlotInfo(ESplitSlotType InSplitSlotType) const;

	UFUNCTION(BlueprintPure)
	FSplitSlotInfo GetSplitSlotInfo(ESplitSlotType InSplitSlotType) const;
	
	UFUNCTION(BlueprintPure)
	TMap<ESplitSlotType, FSplitSlotInfo> GetSplitSlotInfos() const { return SplitInfos; }

	template<typename T>
	TArray<T*> GetSplitSlots(ESplitSlotType InSplitSlotType)
	{
		TArray<T*> SplitSlots;
		for (auto Iter : GetSplitSlots(InSplitSlotType))
		{
			SplitSlots.Add(Cast<T>(Iter));
		}
		return SplitSlots;
	}
		
	UFUNCTION(BlueprintPure)
	TArray<UInventorySlot*> GetSplitSlots(ESplitSlotType InSplitSlotType);

	UFUNCTION(BlueprintPure)
	TMap<ESplitSlotType, FSplitSlotData> GetSplitSlotDatas();
};
