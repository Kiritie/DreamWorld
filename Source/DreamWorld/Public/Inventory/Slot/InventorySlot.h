// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "InventorySlot.generated.h"

class UInventory;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventorySlotRefresh);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventorySlotCooldownRefresh);

/**
 * 物品槽
 */
UCLASS(BlueprintType)
class DREAMWORLD_API UInventorySlot : public UObject
{
	GENERATED_BODY()

public:
	UInventorySlot();

	//////////////////////////////////////////////////////////////////////////
	/// Properties
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	FItem Item;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	UInventory* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	EItemType LimitType;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
	ESplitSlotType SplitType;

	FGameplayAbilitySpecHandle AbilityHandle;
	
public:
	FOnInventorySlotRefresh OnInventorySlotRefresh;
	FOnInventorySlotRefresh OnInventorySlotCooldownRefresh;

	//////////////////////////////////////////////////////////////////////////
	/// Initialize
public:
	UFUNCTION(BlueprintCallable)
	virtual void InitSlot(UInventory* InOwner, FItem InItem, EItemType InLimitType = EItemType::None, ESplitSlotType InSplitType = ESplitSlotType::Default);

	//////////////////////////////////////////////////////////////////////////
	/// Checks
public:
	UFUNCTION(BlueprintPure)
	virtual bool CheckSlot(FItem& InItem) const;

	UFUNCTION(BlueprintPure)
	virtual bool CanPutIn(FItem& InItem) const;
					
	UFUNCTION(BlueprintPure)
	virtual bool Contains(FItem& InItem) const;

	//////////////////////////////////////////////////////////////////////////
	/// Actions
public:
	UFUNCTION(BlueprintCallable)
	virtual void Refresh();

	UFUNCTION(BlueprintCallable)
	virtual void PreSet(FItem& InItem);
		
	UFUNCTION(BlueprintCallable)
	virtual void EndSet();

	UFUNCTION(BlueprintCallable)
	virtual void Replace(UInventorySlot* InSlot);

	UFUNCTION(BlueprintCallable)
	virtual void SetItem(FItem& InItem, bool bRefresh = true);

	UFUNCTION(BlueprintCallable)
	virtual void AddItem(FItem& InItem);

	UFUNCTION(BlueprintCallable)
	virtual void SubItem(FItem& InItem);
			
	UFUNCTION(BlueprintCallable)
	virtual void SplitItem(int InCount = -1);
			
	UFUNCTION(BlueprintCallable)
	virtual void MoveItem(int InCount = -1);

	UFUNCTION(BlueprintCallable)
	virtual void UseItem(int InCount = -1);

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
	FDWCooldownInfo CooldownInfo;

public:
	UFUNCTION(BlueprintPure)
	FDWCooldownInfo GetCooldownInfo() const { return CooldownInfo; }

	UFUNCTION(BlueprintCallable)
	void SetCooldownInfo(const FDWCooldownInfo& InCooldownInfo) { CooldownInfo = InCooldownInfo; }

	//////////////////////////////////////////////////////////////////////////
	/// Getter/Setter
public:
	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const;

	UFUNCTION(BlueprintCallable)
	int GetRemainVolume() const;

	UFUNCTION(BlueprintCallable)
	int GetMaxVolume() const;

	UFUNCTION(BlueprintPure)
	FItem& GetItem()  { return Item; }
	
	UFUNCTION(BlueprintPure)
	UInventory* GetOwner() const { return Owner; }
	
	UFUNCTION(BlueprintPure)
	EItemType GetLimitType() const { return LimitType; }

	UFUNCTION(BlueprintCallable)
	void SetLimitType(EItemType val) { LimitType = val; }
	
	UFUNCTION(BlueprintPure)
	ESplitSlotType GetSplitType() const { return SplitType; }

	UFUNCTION(BlueprintPure)
	virtual FDWAbilityInfo GetAbilityInfo() const;

	UFUNCTION(BlueprintPure)
	FGameplayAbilitySpecHandle& GetAbilityHandle() { return AbilityHandle; }

	UFUNCTION(BlueprintCallable)
	void SetAbilityHandle(FGameplayAbilitySpecHandle val) { AbilityHandle = val; }
};
