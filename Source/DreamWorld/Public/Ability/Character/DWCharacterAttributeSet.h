#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Ability/Attributes/CharacterAttributeSetBase.h"
#include "DWCharacterAttributeSet.generated.h"

/**
 * 角色属性集
 */
UCLASS()
class DREAMWORLD_API UDWCharacterAttributeSet : public UCharacterAttributeSetBase
{
	GENERATED_BODY()

public:
	UDWCharacterAttributeSet();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData Mana;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Mana)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData MaxMana;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MaxMana)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData Stamina;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Stamina)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData MaxStamina;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MaxStamina)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData SwimSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, SwimSpeed)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData RideSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, RideSpeed)
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData FlySpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, FlySpeed)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData DodgeForce;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DodgeForce)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData AttackForce;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackForce)
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData RepulseForce;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, RepulseForce)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData AttackSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackSpeed)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData AttackCritRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackCritRate)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData AttackStealRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackStealRate)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData DefendRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DefendRate)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData DefendScope;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DefendScope)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData PhysicsDefRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, PhysicsDefRate)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData MagicDefRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MagicDefRate)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData ToughnessRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, ToughnessRate)
			
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData StaminaRegenSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, StaminaRegenSpeed)
			
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttributes")
	FGameplayAttributeData StaminaExpendSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, StaminaExpendSpeed)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interrupt")
	FGameplayAttributeData Interrupt;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Interrupt)

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
};
