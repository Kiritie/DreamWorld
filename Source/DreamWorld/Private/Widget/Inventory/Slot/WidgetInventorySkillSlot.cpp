// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Slot/WidgetInventorySkillSlot.h"

#include "Ability/Item/AbilityItemDataBase.h"
#include "Character/DWCharacter.h"
#include "Ability/Inventory/Slot/InventorySlot.h"
#include "Ability/Inventory/Slot/InventorySkillSlot.h"
#include "Components/TextBlock.h"
#include "Ability/Inventory/Inventory.h"
#include "Kismet/KismetTextLibrary.h"

UWidgetInventorySkillSlot::UWidgetInventorySkillSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWidgetInventorySkillSlot::OnInitialize(UInventorySlot* InOwnerSlot)
{
	Super::OnInitialize(InOwnerSlot);
}

void UWidgetInventorySkillSlot::UseItem(int InCount)
{
	if(OwnerSlot)
	{
		OwnerSlot->ActiveItem();
	}
}

void UWidgetInventorySkillSlot::OnRefresh()
{
	Super::OnRefresh();

	if(!IsEmpty())
	{
		TxtName->SetVisibility(ESlateVisibility::Visible);
		TxtName->SetText(GetItem().GetData().Name);

		TxtCost->SetVisibility(ESlateVisibility::Visible);
		TxtCost->SetText(FText::FromString(FString::FromInt(FMath::Abs(OwnerSlot->GetAbilityInfo().CostValue))));
		if(OwnerSlot->GetAbilityInfo().CostAttribute == GetInventory()->GetOwnerAgent<ADWCharacter>()->GetHealthAttribute())
		{
			TxtCost->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else if(OwnerSlot->GetAbilityInfo().CostAttribute == GetInventory()->GetOwnerAgent<ADWCharacter>()->GetManaAttribute())
		{
			TxtCost->SetColorAndOpacity(FLinearColor(0.f, 0.65f, 1.f, 1.f));
		}
		else if(OwnerSlot->GetAbilityInfo().CostAttribute == GetInventory()->GetOwnerAgent<ADWCharacter>()->GetStaminaAttribute())
		{
			TxtCost->SetColorAndOpacity(FLinearColor(0.f, 0.7f, 0.04f, 1.f));
		}
	}
	else
	{
		TxtName->SetVisibility(ESlateVisibility::Hidden);
		TxtCost->SetVisibility(ESlateVisibility::Hidden);
	}
}
