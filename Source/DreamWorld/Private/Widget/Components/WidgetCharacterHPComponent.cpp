// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Components/WidgetCharacterHPComponent.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Character/DWCharacter.h"
#include "Global/GlobalBPLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Widget/World/WidgetCharacterHP.h"

UWidgetCharacterHPComponent::UWidgetCharacterHPComponent()
{
	static ConstructorHelpers::FClassFinder<UWidgetCharacterHP> CharacterHPClassFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/World/WB_CharacterHP.WB_CharacterHP_C'"));
	if(CharacterHPClassFinder.Succeeded())
	{
		SetWidgetClass(CharacterHPClassFinder.Class);
	}
	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawSize(FVector2D(220, 60));
	SetPivot(FVector2D(0.5f, 1));
}

void UWidgetCharacterHPComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ADWCharacter>(GetOwner());
}

void UWidgetCharacterHPComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!OwnerCharacter) return;

	RefreshVisibility();
}

void UWidgetCharacterHPComponent::RefreshVisibility() const
{
	if(GetUserWidgetObject())
	{
		ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>(this);
		if(!OwnerCharacter->IsDead() && PlayerCharacter && FVector::Distance(OwnerCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation()) < 1000)
		{
			GetUserWidgetObject()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
