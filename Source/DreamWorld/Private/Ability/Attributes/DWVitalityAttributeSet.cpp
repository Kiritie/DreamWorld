#include "Ability/Attributes/DWVitalityAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Common/DWCommonTypes.h"

UDWVitalityAttributeSet::UDWVitalityAttributeSet()
{
	DamageHandleClass = UDWDamageHandle::StaticClass();
}

void UDWVitalityAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UDWVitalityAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UDWVitalityAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UDWVitalityAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
