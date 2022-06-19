// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Item/Skill/Remote/AbilitySkillRemoteBase.h"
#include "DWSkillRemote.generated.h"

/**
 * װ��
 */
UCLASS()
class DREAMWORLD_API ADWSkillRemote : public AAbilitySkillRemoteBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADWSkillRemote();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
	virtual void Initialize(AAbilityCharacterBase* InOwnerCharacter) override;
};
