#include "Item/Equip/Weapon/DWEquipWeaponData.h"

UDWEquipWeaponData::UDWEquipWeaponData()
{
	EquipType = EDWEquipType::Weapon;
	HandType = EDWWeaponHandType::Single;
	WeaponType = EDWWeaponType::None;

	AttackHitSound = nullptr;
	AttackMissSound = nullptr;
	AttackHitEffect = nullptr;
	AttackMissEffect = nullptr;
}
