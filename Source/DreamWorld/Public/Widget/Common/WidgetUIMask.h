// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/Widget/WidgetUIMaskBase.h"
#include "WidgetUIMask.generated.h"

/**
 * 
 */
UCLASS()
class DREAMWORLD_API UWidgetUIMask : public UWidgetUIMaskBase
{
	GENERATED_BODY()
	
public:
	UWidgetUIMask(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual void OnCreate(UObject* InOwner, const TArray<FParameter>& InParams) override;
	
	virtual void OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams) override;

	virtual void OnOpen(const TArray<FParameter>& InParams, bool bInstant) override;

	virtual void OnClose(bool bInstant) override;

public:
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
};
