// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Skill/Skill.h"
#include "SkillMelee.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

/**
 * ��ս����
 */
UCLASS()
class DREAMWORLD_API ASkillMelee : public ASkill
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillMelee();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
