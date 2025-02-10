// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Switcher/WidgetSwitcherBox.h"

#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "Widget/Common/CommonButtonGroup.h"

UWidgetSwitcherBox::UWidgetSwitcherBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UWidgetSwitcherBox::OnCreate(UUserWidget* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	SwitcherGroup = UObjectPoolModuleStatics::SpawnObject<UCommonButtonGroup>();
	SwitcherGroup->SetSelectionRequiredN(true);
	SwitcherGroup->OnSelectedButtonBaseChanged.AddDynamic(this, &UWidgetSwitcherBox::OnSwitcherButtonSelected);

	if(SwitcherContent)
	{
		for(auto Iter : SwitcherContent->GetAllChildren())
		{
			SwitcherGroup->AddWidget(Iter);
		}
	}
}

void UWidgetSwitcherBox::OnInitialize(const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InParams);
}

void UWidgetSwitcherBox::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetSwitcherBox::OnSwitcherButtonSelected(UCommonButtonBase* AssociatedButton, int32 ButtonIndex)
{
	if(OnButtonSelected.IsBound())
	{
		OnButtonSelected.Broadcast(ButtonIndex);
	}
}

int32 UWidgetSwitcherBox::GetSelectedButtonIndex() const
{
	return SwitcherGroup->GetSelectedButtonIndex();
}

void UWidgetSwitcherBox::SetSelectedButtonIndex(int32 InButtonIndex) const
{
	SwitcherGroup->SelectButtonAtIndex(InButtonIndex, true);
}
