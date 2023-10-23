// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widget/Setting/Misc/WidgetPressAnyKeyPanel.h"

#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"

class ICursor;

class FPressAnyKeyInputProcessor : public IInputProcessor
{
public:
	FPressAnyKeyInputProcessor()
	{

	}

	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override { }

	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		HandleKey(InKeyEvent.GetKey());
		return true;
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		return true;
	}

	virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		HandleKey(MouseEvent.GetEffectingButton());
		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		return true;
	}

	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		HandleKey(MouseEvent.GetEffectingButton());
		return true;
	}

	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override
	{
		if (InWheelEvent.GetWheelDelta() != 0)
		{
			const FKey Key = InWheelEvent.GetWheelDelta() < 0 ? EKeys::MouseScrollDown : EKeys::MouseScrollUp;
			HandleKey(Key);
		}
		return true;
	}

	DECLARE_MULTICAST_DELEGATE(FSettingsPressAnyKeyInputPreProcessorCanceled);
	FSettingsPressAnyKeyInputPreProcessorCanceled OnKeySelectionCanceled;

	DECLARE_MULTICAST_DELEGATE_OneParam(FSettingsPressAnyKeyInputPreProcessorKeySelected, FKey);
	FSettingsPressAnyKeyInputPreProcessorKeySelected OnKeySelected;

private:
	void HandleKey(const FKey& Key)
	{
		// Cancel this process if it's Escape, Touch, or a gamepad key.
		if (Key == EKeys::LeftCommand || Key == EKeys::RightCommand)
		{
			// Ignore
		}
		else if (Key == EKeys::Escape || Key.IsTouch() || Key.IsGamepadKey())
		{
			OnKeySelectionCanceled.Broadcast();
		}
		else
		{
			OnKeySelected.Broadcast(Key);
		}
	}
};

UWidgetPressAnyKeyPanel::UWidgetPressAnyKeyPanel(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	WidgetName = FName("PressAnyKeyPanel");

	WidgetType = EWidgetType::Permanent;
	WidgetInputMode = EInputMode::UIOnly;
}

void UWidgetPressAnyKeyPanel::OnInitialize(UObject* InOwner)
{
	Super::OnInitialize(InOwner);
}

void UWidgetPressAnyKeyPanel::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);

	bKeySelected = false;

	InputProcessor = MakeShared<FPressAnyKeyInputProcessor>();
	InputProcessor->OnKeySelected.AddUObject(this, &ThisClass::HandleKeySelected);
	InputProcessor->OnKeySelectionCanceled.AddUObject(this, &ThisClass::HandleKeySelectionCanceled);
	FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor, 0);
}

void UWidgetPressAnyKeyPanel::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
	}
}

void UWidgetPressAnyKeyPanel::HandleKeySelected(FKey InKey)
{
	if (!bKeySelected)
	{
		bKeySelected = true;
		Dismiss([this, InKey]() {
			OnKeySelected.Broadcast(InKey);
		});
	}
}

void UWidgetPressAnyKeyPanel::HandleKeySelectionCanceled()
{
	if (!bKeySelected)
	{
		bKeySelected = true;
		Dismiss([this]() {
			OnKeySelectionCanceled.Broadcast();
		});
	}
}

void UWidgetPressAnyKeyPanel::Dismiss(TFunction<void()> PostDismissCallback)
{
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this, PostDismissCallback](float DeltaTime)
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_UWidgetPressAnyKey_Dismiss);

		FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);

		Close();

		PostDismissCallback();

		return false;
	}));
}
