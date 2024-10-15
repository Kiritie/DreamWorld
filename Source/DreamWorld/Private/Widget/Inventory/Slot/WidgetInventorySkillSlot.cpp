// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Slot/WidgetInventorySkillSlot.h"

#include "Ability/Item/AbilityItemDataBase.h"
#include "Character/DWCharacter.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
#include "Components/TextBlock.h"
#include "Ability/Inventory/AbilityInventoryBase.h"

UWidgetInventorySkillSlot::UWidgetInventorySkillSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWidgetInventorySkillSlot::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetInventorySkillSlot::UseItem_Implementation(int InCount)
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

		FAbilityInfo AbilityInfo = OwnerSlot->GetAbilityInfo();
		ADWCharacter* OwnerCharacter = GetInventory()->GetOwnerAgent<ADWCharacter>();
		
		TxtCost->SetText(FText::FromString(FString::FromInt(FMath::Abs(AbilityInfo.CostValue))));
		if(AbilityInfo.CostAttribute == OwnerCharacter->GetHealthAttribute())
		{
			TxtCost->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else if(AbilityInfo.CostAttribute == OwnerCharacter->GetManaAttribute())
		{
			TxtCost->SetColorAndOpacity(FLinearColor(0.f, 0.65f, 1.f, 1.f));
		}
		else if(AbilityInfo.CostAttribute == OwnerCharacter->GetStaminaAttribute())
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
