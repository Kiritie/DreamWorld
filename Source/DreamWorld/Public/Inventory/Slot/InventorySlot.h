// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Global/Base/WHObject.h"
#include "InventorySlot.generated.h"

class UInventory;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventorySlotRefresh);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventorySlotCooldownRefresh);

/**
 * 物品槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UInventorySlot : public UWHObject
{
	GENERATED_BODY()

public:
	UInventorySlot();

	//////////////////////////////////////////////////////////////////////////
	/// Properties
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	FAbilityItem Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	UInventory* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	EAbilityItemType LimitType;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	ESplitSlotType SplitType;
	
public:
	FOnInventorySlotRefresh OnInventorySlotRefresh;
	FOnInventorySlotRefresh OnInventorySlotCooldownRefresh;

	//////////////////////////////////////////////////////////////////////////
	/// Initialize
public:
	UFUNCTION(BlueprintCallable)
	virtual void InitSlot(UInventory* InOwner, FAbilityItem InItem, EAbilityItemType InLimitType/* = EAbilityItemType::None*/, ESplitSlotType InSplitType/* = ESplitSlotType::Default*/);
	
	virtual void OnSpawn_Implementation(const TArray<FParameter>& InParams) override;

	virtual void OnDespawn_Implementation() override;

	//////////////////////////////////////////////////////////////////////////
	/// Checks
public:
	UFUNCTION(BlueprintPure)
	virtual bool CheckSlot(FAbilityItem& InItem) const;

	UFUNCTION(BlueprintPure)
	virtual bool CanPutIn(FAbilityItem& InItem) const;
					
	UFUNCTION(BlueprintPure)
	virtual bool Contains(FAbilityItem& InItem) const;

	//////////////////////////////////////////////////////////////////////////
	/// Actions
public:
	UFUNCTION(BlueprintCallable)
	virtual void Refresh();

	UFUNCTION(BlueprintCallable)
	virtual void PreSet(FAbilityItem& InItem);
		
	UFUNCTION(BlueprintCallable)
	virtual void EndSet();

	UFUNCTION(BlueprintCallable)
	virtual void Replace(UInventorySlot* InSlot);

	UFUNCTION(BlueprintCallable)
	virtual void SetItem(FAbilityItem& InItem, bool bRefresh = true);

	UFUNCTION(BlueprintCallable)
	virtual void AddItem(FAbilityItem& InItem);

	UFUNCTION(BlueprintCallable)
	virtual void SubItem(FAbilityItem& InItem);
			
	UFUNCTION(BlueprintCallable)
	virtual void SplitItem(int InCount = -1);
			
	UFUNCTION(BlueprintCallable)
	virtual void MoveItem(int InCount = -1);

	UFUNCTION(BlueprintCallable)
	virtual void UseItem(int InCount = -1);

	virtual void AssembleItem();

	virtual void DischargeItem();

	UFUNCTION(BlueprintCallable)
	virtual void DiscardItem(int InCount = -1);
	
	UFUNCTION(BlueprintCallable)
	virtual bool ActiveItem();
		
	UFUNCTION(BlueprintCallable)
	virtual bool CancelItem();

	UFUNCTION(BlueprintCallable)
	virtual void ClearItem();

	//////////////////////////////////////////////////////////////////////////
	/// Cooldown
public:
	void StartCooldown();

	void StopCooldown();

	void RefreshCooldown(float DeltaSeconds);

protected:
	UPROPERTY(BlueprintReadOnly)
	FCooldownInfo CooldownInfo;

public:
	UFUNCTION(BlueprintPure)
	FCooldownInfo GetCooldownInfo() const { return CooldownInfo; }

	UFUNCTION(BlueprintCallable)
	void SetCooldownInfo(const FCooldownInfo& InCooldownInfo) { CooldownInfo = InCooldownInfo; }

	//////////////////////////////////////////////////////////////////////////
	/// Getter/Setter
public:
	UFUNCTION(BlueprintPure)
	bool IsEmpty() const;
	
	UFUNCTION(BlueprintPure)
	bool IsSelected() const;

	UFUNCTION(BlueprintPure)
	int GetRemainVolume() const;

	UFUNCTION(BlueprintPure)
	int GetMaxVolume() const;

	UFUNCTION(BlueprintPure)
	FAbilityItem& GetItem()  { return Item; }

	UFUNCTION(BlueprintPure)
	UInventory* GetOwner() const { return Owner; }
	
	UFUNCTION(BlueprintPure)
	EAbilityItemType GetLimitType() const { return LimitType; }

	UFUNCTION(BlueprintCallable)
	void SetLimitType(EAbilityItemType val) { LimitType = val; }
	
	UFUNCTION(BlueprintPure)
	ESplitSlotType GetSplitType() const { return SplitType; }

	UFUNCTION(BlueprintPure)
	virtual FAbilityInfo GetAbilityInfo() const;
};
