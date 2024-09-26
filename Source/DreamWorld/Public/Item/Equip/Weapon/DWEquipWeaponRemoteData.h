#pragma once

#include "DWEquipWeaponData.h"

#include "DWEquipWeaponRemoteData.generated.h"

class AAbilityProjectileBase;

UCLASS(BlueprintType)
class DREAMWORLD_API UDWEquipWeaponRemoteData : public UDWEquipWeaponData
{
	GENERATED_BODY()

public:
	UDWEquipWeaponRemoteData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAbilityProjectileBase> ProjectileClass;
};
