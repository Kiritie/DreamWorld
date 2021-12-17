// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Components/WidgetWorldTextComponent.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Widget/Other/WidgetWorldText.h"

UWidgetWorldTextComponent::UWidgetWorldTextComponent()
{
	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawSize(FVector2D(200, 50));
	SetPivot(FVector2D(0.5f, 1));

	static ConstructorHelpers::FClassFinder<UWidgetWorldText> NormalWorldTextClassFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Other/WB_WorldText_Normal.WB_WorldText_Normal_C'"));
	if(NormalWorldTextClassFinder.Succeeded())
	{
		WorldTextMap.Add(EWorldTextStyle::Normal, NormalWorldTextClassFinder.Class);
	}

	static ConstructorHelpers::FClassFinder<UWidgetWorldText> StressWorldTextClassFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/Other/WB_WorldText_Stress.WB_WorldText_Stress_C'"));
	if(StressWorldTextClassFinder.Succeeded())
	{
		WorldTextMap.Add(EWorldTextStyle::Stress, StressWorldTextClassFinder.Class);
	}
}

void UWidgetWorldTextComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UWidgetWorldTextComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RefreshVisibility();
}

void UWidgetWorldTextComponent::InitContent(FString InContent, EWorldTextType InContentType, EWorldTextStyle InContentStyle)
{
	FTimerHandle DestroyTimer;
	FTimerDelegate DestroyDelegate;
	DestroyDelegate.BindUObject(this, &UWidgetWorldTextComponent::DestroyComponent, false);
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, DestroyDelegate, InContentStyle == EWorldTextStyle::Normal ? 1.f : 1.5f, false);

	if(WorldTextMap.Contains(InContentStyle) && WorldTextMap[InContentStyle])
	{
		SetWidgetClass(WorldTextMap[InContentStyle]);
		if(UWidgetWorldText* WidgetWorldText = Cast<UWidgetWorldText>(GetUserWidgetObject()))
		{
			WidgetWorldText->InitContent(InContent, InContentType);
		}
	}
}

void UWidgetWorldTextComponent::RefreshVisibility()
{
	ADWPlayerCharacter* PlayerCharacter = UDWHelper::GetPlayerCharacter(this);
	SetVisibility(PlayerCharacter && FVector::Distance(GetOwner()->GetActorLocation(), PlayerCharacter->GetActorLocation()) < 1000);
}
