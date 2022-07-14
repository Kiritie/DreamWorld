#pragma once

#include "Engine/DataTable.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "Ability/AbilityModuleTypes.h"
#include "Asset/AssetModuleBPLibrary.h"
#include "Inventory/InventoryTypes.h"
#include "Parameter/ParameterModuleTypes.h"
#include "SaveGame/SaveGameModuleTypes.h"
#include "Voxel/VoxelModuleTypes.h"

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
enum class EDWControlMode : uint8
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
	Crouch,
	// ????
	Dodge,
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
	GetHit,
	// ????????
	AttackHit,
	// ???????
	AttackMiss,
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
	// 物理近战
	PhysicsMelee,
	// 物理远程
	PhysicsRemote,
	// 魔法近战
	MagicMelee,
	// 魔法远程
	MagicRemote,
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCancelable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedFreeToAnim;

	FORCEINLINE FDWCharacterActionAbilityData()
	{
		ActionType = EDWCharacterActionType::None;
		AbilityClass = nullptr;
		bCancelable = true;
		bNeedActive = true;
		bNeedFreeToAnim = false;
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
struct DREAMWORLD_API FDWCharacterSkillAbilityData : public FAbilityData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWWeaponType WeaponType;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDWCharacterSkillAbility> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCancelable;

	FORCEINLINE FDWCharacterSkillAbilityData()
	{
		WeaponType = EDWWeaponType::None;
		AbilityClass = nullptr;
		bCancelable = false;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWEffectData : public FTableRowBase
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
	
public:
	UPROPERTY(BlueprintReadWrite)
	FInventorySaveData InventoryData;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterSaveData : public FCharacterSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWCharacterSaveData()
	{
		TeamID = NAME_None;
		Nature = EDWCharacterNature::AIHostile;
		AttackDistance = 100.f;
		InteractDistance = 500.f;
		FollowDistance = 500.f;
		PatrolDistance = 1000.f;
		PatrolDuration = 10.f;
		FallingAttackAbility = FDWCharacterAttackAbilityData();
		AttackAbilities = TArray<FDWCharacterAttackAbilityData>();
		SkillAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData>();
		ActionAbilities = TMap<EDWCharacterActionType, FDWCharacterActionAbilityData>();
	}
	
	UPROPERTY(BlueprintReadOnly)
	FName TeamID;

	UPROPERTY()
	EDWCharacterNature Nature;
	
	UPROPERTY()
	float AttackDistance;
	
	UPROPERTY()
	float InteractDistance;
		
	UPROPERTY()
	float FollowDistance;
			
	UPROPERTY()
	float PatrolDistance;
			
	UPROPERTY()
	float PatrolDuration;
	
	UPROPERTY(BlueprintReadWrite)
	FInventorySaveData InventoryData;

	UPROPERTY()
	FDWCharacterAttackAbilityData FallingAttackAbility;

	UPROPERTY()
	TArray<FDWCharacterAttackAbilityData> AttackAbilities;

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

	UPROPERTY(EditDefaultsOnly)
	FRotator CameraRotation;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWPlayerSaveData : public FDWPlayerBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWPlayerSaveData()
	{
		ArchiveID = 0;
		ControlMode = EDWControlMode::Fighting;
	}
	
	FORCEINLINE FDWPlayerSaveData(FDWPlayerBasicSaveData InBasicSaveData)
	{
		ID = InBasicSaveData.ID;
	
		ArchiveID = 0;
		ControlMode = EDWControlMode::Fighting;
	}

	UPROPERTY(BlueprintReadOnly)
	int32 ArchiveID;

	UPROPERTY(BlueprintReadOnly)
	EDWControlMode ControlMode;
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
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWVoxelWorldSaveData : public FVoxelWorldSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWVoxelWorldSaveData()
	{
		WorldSeed = 0;
		TimeSeconds = 0.f;

		LastVitalityRaceIndex = FIndex::ZeroIndex;
		LastCharacterRaceIndex = FIndex::ZeroIndex;
	}
	
	FORCEINLINE FDWVoxelWorldSaveData(FVoxelWorldBasicSaveData InBasicSaveData)
	{
		WorldSeed = 0;
		TimeSeconds = 0;

		BlockSize = InBasicSaveData.BlockSize;
		ChunkSize = InBasicSaveData.ChunkSize;
		
		ChunkHeightRange = InBasicSaveData.ChunkHeightRange;

		TerrainBaseHeight = InBasicSaveData.TerrainBaseHeight;
		TerrainPlainScale = InBasicSaveData.TerrainPlainScale;
		TerrainMountainScale = InBasicSaveData.TerrainMountainScale;
		TerrainStoneVoxelScale = InBasicSaveData.TerrainStoneVoxelScale;
		TerrainSandVoxelScale = InBasicSaveData.TerrainSandVoxelScale;
		TerrainWaterVoxelHeight = InBasicSaveData.TerrainWaterVoxelHeight;
		TerrainBedrockVoxelHeight = InBasicSaveData.TerrainBedrockVoxelHeight;

		ChunkMaterials = InBasicSaveData.ChunkMaterials;

		VitalityRaceDensity = InBasicSaveData.VitalityRaceDensity;
		CharacterRaceDensity = InBasicSaveData.CharacterRaceDensity;

		LastVitalityRaceIndex = FIndex::ZeroIndex;
		LastCharacterRaceIndex = FIndex::ZeroIndex;
	}

public:
	static const FDWVoxelWorldSaveData Empty;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FVector, FDWVoxelChunkSaveData> ChunkDatas;

	FIndex LastVitalityRaceIndex;

	FIndex LastCharacterRaceIndex;

public:
	bool IsExistChunkData(FIndex InChunkIndex) const
	{
		return ChunkDatas.Contains(InChunkIndex.ToVector());
	}

	FDWVoxelChunkSaveData& GetChunkData(FIndex InChunkIndex) const
	{
		static FDWVoxelChunkSaveData ChunkData;
		if (ChunkDatas.Contains(InChunkIndex.ToVector()))
		{
			ChunkData = ChunkDatas[InChunkIndex.ToVector()];
		}
		return ChunkData;
	}

	void SetChunkData(FIndex InChunkIndex, FDWVoxelChunkSaveData InChunkData)
	{
		if (!ChunkDatas.Contains(InChunkIndex.ToVector()))
			ChunkDatas.Add(InChunkIndex.ToVector(), InChunkData);
		else
			ChunkDatas[InChunkIndex.ToVector()] = InChunkData;
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
		PlayerBasicData = FDWPlayerBasicSaveData();
		WorldBasicData = FVoxelWorldBasicSaveData();
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDWPlayerBasicSaveData PlayerBasicData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVoxelWorldBasicSaveData WorldBasicData;
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
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWGeneralSaveData : public FGeneralSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWGeneralSaveData()
	{
		bAutoJump = true;
		CameraDistance = 150.f;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraDistance;
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
	Chunk = (int32)ECC_GameTraceChannel6,
	// ????
	Voxel = (int32)ECC_GameTraceChannel7,
	// ????
	Sight = (int32)ECC_GameTraceChannel8,
	// ???
	Step = (int32)ECC_GameTraceChannel9
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
