#include "Item/Skill/DWSkillData.h"

#include "Character/DWCharacter.h"
#include "Common/CommonStatics.h"

UDWSkillData::UDWSkillData()
{
	MaxLevel = 100;

	SkillType = EDWSkillType::None;
	WeaponPart = EDWWeaponPart::None;
	WeaponType = EDWWeaponType::None;
	bCancelAble = false;
}

FString UDWSkillData::GetItemErrorInfo(AActor* InOwner, int32 InLevel) const
{
	FString ErrorInfo = Super::GetItemErrorInfo(InOwner, InLevel);

	if(ADWCharacter* Character = Cast<ADWCharacter>(InOwner))
	{
		if(!Character->CheckWeaponType(WeaponPart, WeaponType))
		{
			ErrorInfo.Appendf(TEXT("\n角色未装备[%s]"), *UCommonStatics::GetEnumDisplayNameByValue(TEXT("/Script/DreamWorld.EDWWeaponType"), (int32)WeaponType).ToString());
		}
	}
	ErrorInfo.RemoveFromStart(TEXT("\n"));
	
	return ErrorInfo;
}
