// Copyright Underflow Studios 2017

#pragma once

#include "CoreMinimal.h"
#include "Dialogue.h"
#include "Blueprint/UserWidget.h"
#include "DialogueUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGUEPLUGIN_API UDialogueUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// Conditions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue UI")
	bool IsConditionsMetForNode(FDialogueNode Node);

	// Events
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue UI")
	void RunEventsForNode(FDialogueNode Node);

	//VarStrings
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintPure, Category = "Dialogue UI")
	FText ReplaceVarStrings(const FText & InText);

	//VarStrings
	UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
	bool RunStringReplacer(FString originalString, FString& resultString);

	//VarStrings
	UFUNCTION(BlueprintCallable, Category = "Dialogue UI")
	TArray<FString> FindVarStrings(FText inText);
	
	//The actor this dialogue belongs to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( ExposeOnSpawn = true ), Category = "Dialogue UI")
	AActor* NPCActor;
};
