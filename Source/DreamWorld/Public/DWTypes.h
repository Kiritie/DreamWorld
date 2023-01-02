#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "Ability/AbilityModuleTypes.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "Parameter/ParameterModuleTypes.h"
#include "SaveGame/SaveGameModuleTypes.h"
#include "Voxel/VoxelModuleTypes.h"
#include "Asset/AssetModuleTypes.h"

#include "DWTypes.generated.h"

class UBehaviorTree;
class AVoxelModule;
class ADWPlayerCharacter;
class ADWGameMode;
class UParticleSystem;
class USoundBase;
class UAnimMontage;
class UGameUI;
class AVoxelChunk;
class UVoxel;
class UMaterialInterface;
class UTexture2D;
class ADWCharacter;
class UWidgetCharacterHP;
class UWidgetVitalityHP;
class UWidgetWorldText;
class AAbilityVitalityBase;
class AVoxelAuxiliary;
class ADWPlayerController;
class UWidgetInventorySlot;
class UDWGameInstance;
class UInventory;
class UCharacterInventory;
class ADWGameState;
class AAbilityEquipBase;
class AAbilitySkillBase;
class UPropEffectBase;
class UEquipEffectBase;
class UAbilityBase;
class UAttributeSetBase;
class UDWCharacterAttributeSet;
class UDWCharacterAttackAbility;
class UDWCharacterActionAbility;
class UDWCharacterSkillAbility;
class UTargetType;
class UInventorySlot;

/**
 * ??????
 */
UENUM(BlueprintType)
enum class EDWCharacterControlMode : uint8
{
	// ???
	Fighting,
	// ????
	Creating
};

/**
 * ???????
 */
UENUM(BlueprintType)
enum class EDWCharacterNature : uint8
{
	// ???
	Player,
	// ???????
	NPC,
	// ????AI
	AIFriendly,
	// ??????AI
	AINeutral,
	// ?ж??AI
	AIHostile
};

/**
 * ??????????
 */
UENUM(BlueprintType)
enum class EDWCharacterActionType : uint8
{
	// ??
	None,
	// ????
	Death,
	// ????
	Revive,
	// ???
	Jump,
	// ???
	Fall,
	// ???
	Walk,
	// ???
	Crouch,
	// ????
	Dodge,
	// ????
	Sprint,
	// ????
	Climb,
	// ???
	Swim,
	// ???
	Float,
	// ???
	Ride,
	// ????
	Fly,
	// ???
	Take,
	// ???
	Use,
	// ????
	Discard,
	// ????
	Generate,
	// ????
	Destroy,
	// ???????
	Attack,
	// ???????
	GetHit,
	// ???????
	Defend,
	// ???????
	DefendBlock,
	// ???
	Interrupt
};

UENUM(BlueprintType)
enum class EDWPropType : uint8
{
	// ??
	Potion,
	// ???
	Food,
	// ????
	Container
};

UENUM(BlueprintType)
enum class EDWEquipType : uint8
{
	// ????
	Weapon,
	// ????
	Shield,
	// ????
	Armor
};

UENUM(BlueprintType)
enum class EDWEquipPartType : uint8
{
	// 头部
	Head	UMETA(DisplayName="头部"),
	// 胸部
	Chest UMETA(DisplayName="胸部"),
	// 手臂
	Arm UMETA(DisplayName="手臂"),
	// 腿部
	Leg UMETA(DisplayName="腿部"),
	// 左手
	LeftHand UMETA(DisplayName="左手"),
	// 右手
	RightHand UMETA(DisplayName="右手")
};

UENUM(BlueprintType)
enum class EDWWeaponType : uint8
{
	// 无
	None,
	// 小刀
	Knife,
	// 剑
	Sword,
	// 弓
	Bow,
	// 弩
	Crossbow,
	// 法杖
	Wand
};

UENUM(BlueprintType)
enum class EDWShieldType : uint8
{
	// 无
	None,
	// 物理
	Physics,
	// 魔法
	Magic
};

UENUM(BlueprintType)
enum class EDWWeaponHandType : uint8
{
	// ????
	Single,
	// ???
	Both
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterActionAbilityData : public FAbilityData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWCharacterActionType ActionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDWCharacterActionAbility> AbilityClass;

	FORCEINLINE FDWCharacterActionAbilityData()
	{
		ActionType = EDWCharacterActionType::None;
		AbilityClass = nullptr;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterAttackAbilityData : public FAbilityData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDWCharacterAttackAbility> AbilityClass;

	FORCEINLINE FDWCharacterAttackAbilityData()
	{
		WeaponType = EDWWeaponType::None;
		AbilityClass = nullptr;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterAttackAbilityDatas
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FDWCharacterAttackAbilityData> Array;

	FORCEINLINE FDWCharacterAttackAbilityDatas()
	{
		Array = TArray<FDWCharacterAttackAbilityData>();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterSkillAbilityData : public FAbilityItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCancelAble;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDWCharacterSkillAbility> AbilityClass;

	FORCEINLINE FDWCharacterSkillAbilityData()
	{
		WeaponType = EDWWeaponType::None;
		bCancelAble = false;
		AbilityClass = nullptr;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWEffectData : public FDataTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EffectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AbilityLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> EffectClass;

	FActiveGameplayEffectHandle EffectHandle;

	FORCEINLINE FDWEffectData()
	{
		EffectName = NAME_None;
		AbilityLevel = 1;
		EffectHandle = FActiveGameplayEffectHandle();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWVitalitySaveData : public FVitalitySaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWVitalitySaveData()
	{
		
	}
	
	FORCEINLINE FDWVitalitySaveData(const FVitalitySaveData& InVitalitySaveData) : FVitalitySaveData(InVitalitySaveData)
	{
		
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterSaveData : public FCharacterSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWCharacterSaveData()
	{
		TeamID = NAME_None;
		BirthLocation = FVector::ZeroVector;
		ControlMode = EDWCharacterControlMode::Fighting;
		FallingAttackAbility = FDWCharacterAttackAbilityData();
		AttackAbilities = TMap<EDWWeaponType, FDWCharacterAttackAbilityDatas>();
		SkillAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData>();
		ActionAbilities = TMap<EDWCharacterActionType, FDWCharacterActionAbilityData>();
	}
	
	FORCEINLINE FDWCharacterSaveData(const FCharacterSaveData& InCharacterSaveData) : FCharacterSaveData(InCharacterSaveData)
	{
		TeamID = NAME_None;
		BirthLocation = FVector::ZeroVector;
		ControlMode = EDWCharacterControlMode::Fighting;
		FallingAttackAbility = FDWCharacterAttackAbilityData();
		AttackAbilities = TMap<EDWWeaponType, FDWCharacterAttackAbilityDatas>();
		SkillAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData>();
		ActionAbilities = TMap<EDWCharacterActionType, FDWCharacterActionAbilityData>();
	}
	
	UPROPERTY(BlueprintReadOnly)
	FName TeamID;
			
	UPROPERTY()
	FVector BirthLocation;

	UPROPERTY()
	EDWCharacterControlMode ControlMode;

	UPROPERTY()
	FDWCharacterAttackAbilityData FallingAttackAbility;

	UPROPERTY()
	TMap<EDWWeaponType, FDWCharacterAttackAbilityDatas> AttackAbilities;

	UPROPERTY()
	TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData> SkillAbilities;

	UPROPERTY()
	TMap<EDWCharacterActionType, FDWCharacterActionAbilityData> ActionAbilities;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWPlayerBasicSaveData : public FDWCharacterSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWPlayerBasicSaveData()
	{
		CameraRotation = FRotator::ZeroRotator;
	}

	FORCEINLINE FDWPlayerBasicSaveData(const FDWCharacterSaveData& InCharacterSaveData) : FDWCharacterSaveData(InCharacterSaveData)
	{
		CameraRotation = FRotator::ZeroRotator;
	}

	UPROPERTY()
	FRotator CameraRotation;
};

UENUM(BlueprintType)
enum class EDWPlayerInventoryInitType : uint8
{
	// 默认
	Default,
	// 空
	Empty,
	// 充满（调试用）
	All
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWPlayerSaveData : public FDWPlayerBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWPlayerSaveData()
	{
		ArchiveID = 0;
		BodyColorIndex = 0;
		CapeColorIndex = 0;
		InventoryInitType = EDWPlayerInventoryInitType::Default;
	}
		
	FORCEINLINE FDWPlayerSaveData(const FDWCharacterSaveData& InCharacterSaveData) : FDWPlayerBasicSaveData(InCharacterSaveData)
	{
		ArchiveID = 0;
		BodyColorIndex = 0;
		CapeColorIndex = 0;
		InventoryInitType = EDWPlayerInventoryInitType::Default;
	}

	FORCEINLINE FDWPlayerSaveData(const FDWPlayerBasicSaveData& InBasicSaveData) : FDWPlayerBasicSaveData(InBasicSaveData)
	{
		ArchiveID = 0;
		BodyColorIndex = 0;
		CapeColorIndex = 0;
		InventoryInitType = EDWPlayerInventoryInitType::Default;
	}

	UPROPERTY(BlueprintReadOnly)
	int32 ArchiveID;
	
	UPROPERTY(BlueprintReadWrite)
	int32 BodyColorIndex;
		
	UPROPERTY(BlueprintReadWrite)
	int32 CapeColorIndex;

	UPROPERTY(BlueprintReadWrite)
	EDWPlayerInventoryInitType InventoryInitType;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWVoxelChunkSaveData : public FVoxelChunkSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FDWCharacterSaveData> CharacterDatas;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FDWVitalitySaveData> VitalityDatas;

	FORCEINLINE FDWVoxelChunkSaveData()
	{
		CharacterDatas = TArray<FDWCharacterSaveData>();
		VitalityDatas = TArray<FDWVitalitySaveData>();
	}

	FORCEINLINE FDWVoxelChunkSaveData(const FVoxelChunkSaveData& InVoxelChunkSaveData) : FVoxelChunkSaveData(InVoxelChunkSaveData)
	{
		CharacterDatas = TArray<FDWCharacterSaveData>();
		VitalityDatas = TArray<FDWVitalitySaveData>();
	}

public:
	virtual void MakeSaved() override
	{
		Super::MakeSaved();
		for(auto& Iter : CharacterDatas)
		{
			Iter.MakeSaved();
		}
		for(auto& Iter : VitalityDatas)
		{
			Iter.MakeSaved();
		}
	}	
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWVoxelWorldSaveData : public FVoxelWorldSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWVoxelWorldSaveData()
	{
		ChunkDatas = TMap<FVector, FDWVoxelChunkSaveData>();
	}
	
	FORCEINLINE FDWVoxelWorldSaveData(const FVoxelWorldBasicSaveData& InBasicSaveData) : FVoxelWorldSaveData(InBasicSaveData)
	{
		ChunkDatas = TMap<FVector, FDWVoxelChunkSaveData>();
	}

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<FVector, FDWVoxelChunkSaveData> ChunkDatas;

public:
	virtual void MakeSaved() override
	{
		Super::MakeSaved();
		for(auto Iter : ChunkDatas)
		{
			Iter.Value.MakeSaved();
		}
	}
	
	virtual bool IsExistChunkData(FIndex InChunkIndex) const override
	{
		return ChunkDatas.Contains(InChunkIndex.ToVector());
	}

	virtual FVoxelChunkSaveData* GetChunkData(FIndex InChunkIndex) override
	{
		if (ChunkDatas.Contains(InChunkIndex.ToVector()))
		{
			return &ChunkDatas[InChunkIndex.ToVector()];
		}
		return nullptr;
	}

	virtual void SetChunkData(FIndex InChunkIndex, FVoxelChunkSaveData* InChunkData) override
	{
		ChunkDatas.Emplace(InChunkIndex.ToVector(), InChunkData->CastRef<FDWVoxelChunkSaveData>());
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWArchiveBasicSaveData : public FSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWArchiveBasicSaveData()
	{
		ID = -1;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ID;

public:
	virtual void MakeSaved() override
	{
		Super::MakeSaved();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWArchiveSaveData : public FDWArchiveBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWArchiveSaveData()
	{
		PlayerData = FDWPlayerSaveData();
		WorldData = FDWVoxelWorldSaveData();
	}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FDWPlayerSaveData PlayerData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FDWVoxelWorldSaveData WorldData;

public:
	virtual void MakeSaved() override
	{
		Super::MakeSaved();
		
		PlayerData.MakeSaved();
		WorldData.MakeSaved();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWGeneralSaveData : public FGeneralSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWGeneralSaveData()
	{
		bAutoJump = true;
		CameraDistance = 200.f;
	}

	FORCEINLINE FDWGeneralSaveData(const FGeneralSaveData& InGeneralSaveData) : FGeneralSaveData(InGeneralSaveData)
	{
		bAutoJump = true;
		CameraDistance = 200.f;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraDistance;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWGenerateItemData : public FDataTableRowBase
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWGenerateItemData()
	{
		Item = FAbilityItem();
		Raws = TArray<FAbilityItem>();
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAbilityItem Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityItem> Raws;
};

/**
 * ???????????
 */
UENUM(BlueprintType)
enum class EDWGameTraceType : uint8
{
	// ??
	None = 0,
	// ?????
	Chunk = (uint8)ECC_GameTraceChannel9,
	// ????
	Voxel = (uint8)ECC_GameTraceChannel10,
	// ????
	Sight = (uint8)ECC_GameTraceChannel11,
	// ???
	Step = (uint8)ECC_GameTraceChannel12
};

/**
 * ???AI??
 */
UENUM(BlueprintType)
enum class EDWCharacterAIState : uint8
{
	// ??
	None,
	// AI???
	AIPatrol,
	// AI????
	AIFollow,
	// AI???
	AITrack,
	// AI????
	AIAttack,
	// AI????
	AIDefend
};

/**
 * ????????
 */
UENUM(BlueprintType)
enum class EDWCharacterAttackType : uint8
{
	// ??
	None,
	// ???????
	NormalAttack,
	// ??乥??
	FallingAttack,
	// ??乥??
	RemoteAttack,
	// ???????
	SkillAttack
};

UENUM(BlueprintType)
enum class EDWCharacterPartType : uint8
{
	// 无
	None,
	// 头部
	Head,
	// 颈部
	Neck,
	// 胸部
	Chest,
	// 脚部
	Foot
};

/**
 * 交互选项
 */
UENUM(BlueprintType)
enum class EDWInteractAction : uint8
{
	// 无
	None = EInteractAction::None UMETA(DisplayName="无"),
	// 喂食
	Feed = EInteractAction::Custom3 UMETA(DisplayName="喂食"),
	// 骑乘
	Ride = EInteractAction::Custom4 UMETA(DisplayName="骑乘"),
	// 取消骑乘
	UnRide = EInteractAction::Custom5 UMETA(DisplayName="取消骑乘")
};

UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class DREAMWORLD_API UDWDamageHandle : public UDamageHandle
{
	GENERATED_BODY()

public:
	UDWDamageHandle() {}

	virtual void HandleDamage(AActor* SourceActor, AActor* TargetActor, float DamageValue, EDamageType DamageType, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags) override;
};
