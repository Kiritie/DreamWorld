// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Talent/WidgetTalentContainer.h"

#include "Widget/Talent/WidgetTalentItem.h"

UWidgetTalentContainer::UWidgetTalentContainer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TalentContent = nullptr;
}

void UWidgetTalentContainer::OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
}

void UWidgetTalentContainer::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetTalentContainer::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetTalentContainer::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);

	ClearTalentItem();
}

void UWidgetTalentContainer::AddTalentItem(UWidgetTalentItem* InTalentItem)
{
	if(TalentItems.Contains(InTalentItem)) return;
	
	TalentItems.Add(InTalentItem);
	InTalentItem->OnAddToContainer(this);
}

void UWidgetTalentContainer::ClearTalentItem()
{
	for(auto Iter : TalentItems)
	{
		Iter->Destroy(true);
	}
	TalentItems.Empty();
}
