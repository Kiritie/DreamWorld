// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Talent/WidgetTalentPanel.h"

#include "Ability/Abilities/ItemAbilityBase.h"
#include "Achievement/AchievementModuleStatics.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Components/Spacer.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Item/Talent/DWTalentData.h"
#include "Widget/WidgetModuleStatics.h"
#include "Widget/Common/WidgetUIMask.h"
#include "Widget/Talent/WidgetTalentContainer.h"
#include "Widget/Talent/WidgetTalentItem.h"

UWidgetTalentPanel::UWidgetTalentPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("TalentPanel");
	ParentName = FName("GameHUD");
	WidgetInputMode = EInputMode::UIOnly;
	WidgetType = EWidgetType::Temporary;

	WidgetCreateType = EWidgetCreateType::AutoCreate;

	WidgetOpenFinishType = EWidgetOpenFinishType::Procedure;
	WidgetCloseFinishType = EWidgetCloseFinishType::Procedure;

	bWidgetAutoFocus = true;
	
	SetIsFocusable(true);
}

void UWidgetTalentPanel::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);
	
	if(BtnActivate)
	{
		BtnActivate->OnClicked().AddUObject(this, &UWidgetTalentPanel::OnActivateButtonClicked);
	}
}

void UWidgetTalentPanel::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetTalentPanel::OnReset(bool bForce)
{
	Super::OnReset(bForce);

	OnTalentItemDeselected(SelectedTalentItem);

	for(auto Iter : TalentContainers)
	{
		DestroySubWidget(Iter, true);
	}
	TalentContainers.Empty();
	if(TalentContent)
	{
		TalentContent->ClearChildren();
	}
}

void UWidgetTalentPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	OnTalentContentRefresh();
	
	UWidgetModuleStatics::OpenUserWidget<UWidgetUIMask>();
}

void UWidgetTalentPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	UWidgetModuleStatics::CloseUserWidget<UWidgetUIMask>();
}

void UWidgetTalentPanel::OnRefresh()
{
	Super::OnRefresh();

	for(auto Iter : TalentContainers)
	{
		Iter->Refresh();
	}

	bool bCanTalent = true;
	FAbilityItem _SelectedTalentItem;
	if(GetSelectedTalentItem(_SelectedTalentItem))
	{
		UDWTalentData& TalentData = _SelectedTalentItem.GetData<UDWTalentData>();
		ADWPlayerCharacter* PlayerCharacter = GetOwnerObject<ADWPlayerCharacter>();
		if(_SelectedTalentItem.Level >= TalentData.MaxLevel || (PlayerCharacter && PlayerCharacter->GetTalentPoint() < TalentData.TalentPoint))
		{
			bCanTalent = false;
		}
	}
	else
	{
		bCanTalent = false;
	}
	
	if(BtnActivate)
	{
		// BtnTalent->SetIsEnabled(bCanTalent);
		BtnActivate->SetRenderOpacity(bCanTalent ? 1.f : 0.5f);
		BtnActivate->SetVisibility(bCanTalent ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::HitTestInvisible);
	}

	OnPreviewContentRefresh();
}

void UWidgetTalentPanel::OnDestroy(bool bRecovery)
{
	Super::OnDestroy(bRecovery);
}

void UWidgetTalentPanel::OnTalentItemSelected_Implementation(UWidgetTalentItem* InItem)
{
	if(InItem == SelectedTalentItem) return;

	if(SelectedTalentItem)
	{
		SelectedTalentItem->SetIsSelected(false, false);
	}

	SelectedTalentItem = InItem;

	Refresh();
}

void UWidgetTalentPanel::OnTalentItemDeselected_Implementation(UWidgetTalentItem* InItem)
{
	if(InItem != SelectedTalentItem) return;

	if(SelectedTalentItem && SelectedTalentItem->GetSelected())
	{
		SelectedTalentItem->SetIsSelected(false, false);
	}

	SelectedTalentItem = nullptr;

	Refresh();
}

void UWidgetTalentPanel::OnTalentContentRefresh()
{
	Reset();
	
	if(TalentContent)
	{
		if(ADWPlayerCharacter* PlayerCharacter = GetOwnerObject<ADWPlayerCharacter>())
		{
			for(auto& Iter1 : PlayerCharacter->GetTalentItems())
			{
				if(UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>(TalentContent->AddChild(NewObject<USpacer>())))
				{
					VerticalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
					VerticalBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
				}
				if(UWidgetTalentContainer* TalentContainer = CreateSubWidget<UWidgetTalentContainer>(nullptr, TalentContainerClass))
				{
					TalentContainers.Add(TalentContainer);
					if(UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>(TalentContent->AddChild(TalentContainer)))
					{
						VerticalBoxSlot->SetPadding(FMargin(20.f, 0.f));
					}
					FAbilityItem ParentItem;
					for(auto& Iter2 : Iter1.Items)
					{
						if(UWidgetTalentItem* TalentItem = CreateSubWidget<UWidgetTalentItem>({ &Iter2, &ParentItem }, TalentItemClass))
						{
							TalentContainer->AddTalentItem(TalentItem);
						}
						ParentItem = Iter2;
					}
				}
			}
			if(UVerticalBoxSlot* VerticalBoxSlot = Cast<UVerticalBoxSlot>(TalentContent->AddChild(NewObject<USpacer>())))
			{
				VerticalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				VerticalBoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
			}
		}
	}

	Refresh();
}

void UWidgetTalentPanel::OnPreviewContentRefresh()
{
	if(PreviewContent)
	{
		
	}
}

void UWidgetTalentPanel::OnActivateButtonClicked()
{
	FAbilityItem _SelectedTalentItem;
	if(GetSelectedTalentItem(_SelectedTalentItem))
	{
		_SelectedTalentItem.Level++;
		_SelectedTalentItem.GetPayload<UWidgetTalentItem>()->Init({ &_SelectedTalentItem });
		
		if(ADWPlayerCharacter* PlayerCharacter = GetOwnerObject<ADWPlayerCharacter>())
		{
			PlayerCharacter->SetTalentPoint(PlayerCharacter->GetTalentPoint() - _SelectedTalentItem.GetData<UDWTalentData>().TalentPoint);
			
			TArray<FAbilityItems> _TalentItems;
			for(auto Iter1 : TalentContainers)
			{
				FAbilityItems _AbilityItems;
				for(auto Iter2 : Iter1->GetTalentItems())
				{
					_AbilityItems.Items.Add(Iter2->GetItem());
				}
				_TalentItems.Add(_AbilityItems);
			}
			for(auto& Iter1 : _TalentItems)
			{
				for(auto& Iter2 : Iter1.Items)
				{
					PlayerCharacter->GetAbilitySystemComponent()->CancelAbilityHandle(Iter2.Handle);
					Iter2.Handle = PlayerCharacter->GetAbilitySystemComponent()->K2_GiveAbility(Iter2.GetData().AbilityClass, Iter2.Level);
					PlayerCharacter->GetAbilitySystemComponent()->TryActivateAbility(Iter2.Handle);
				}
			}
			PlayerCharacter->SetTalentItems(_TalentItems);
		}

		OnTalentContentRefresh();
		
		UAchievementModuleStatics::UnlockAchievement(FName("FirstActivateTalentItem"));
	}
}

bool UWidgetTalentPanel::GetSelectedTalentItem(FAbilityItem& OutItemData) const
{
	if(SelectedTalentItem)
	{
		OutItemData = SelectedTalentItem->GetItem();
		return true;
	}
	return false;
}
