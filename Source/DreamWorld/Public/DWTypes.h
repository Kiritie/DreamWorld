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
 * 游戏状态
 */
UENUM(BlueprintType)
enum class EDWGameState : uint8
{
	// ??
	None,
	// ?????
	Initializing,
	// ?????
	Starting,
	// ?????
	Preparing,
	// ??????
	Loading,
	// ??????
	Playing,
	// ?????
	Pausing,
	// ?????
	Overed
};

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

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWTeamData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FName ID;

	UPROPERTY(BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Detail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADWCharacter* Captain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ADWCharacter*> Members;
	
	static FDWTeamData Empty;

	FORCEINLINE FDWTeamData()
	{
		ID = NAME_None;
		Name = NAME_None;
		Detail = TEXT("");
		Captain = nullptr;
		Members = TArray<ADWCharacter*>();
	}

	void AddMember(ADWCharacter* InMember);

	void RemoveMember(ADWCharacter* InMember);

	void DissolveTeam();

	TArray<ADWCharacter*> GetMembers(ADWCharacter* InMember = nullptr);

	FORCEINLINE int GetNumMember()
	{
		return Members.Num();
	}

	FORCEINLINE bool IsCaptain(ADWCharacter* InMember)
	{
		return Captain == nullptr || Captain == InMember;
	}

	FORCEINLINE bool IsValid()
	{
		return !ID.IsNone();
	}
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

/**
 * ???????
 */
USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWRaceData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Detail;
				
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Proportion;

	FORCEINLINE FDWRaceData()
	{
		ID = NAME_None;
		Name = NAME_None;
		Detail = TEXT("");
		Proportion = 1;
	}

	FORCEINLINE bool IsValid() const
	{
		return !ID.IsNone();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWVitalityRaceData : public FDWRaceData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityItem> Items;

	FORCEINLINE FDWVitalityRaceData()
	{
		Items = TArray<FAbilityItem>();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterRaceData : public FDWRaceData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityItem> Items;

	FORCEINLINE FDWCharacterRaceData()
	{
		Items = TArray<FAbilityItem>();
	}
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
struct DREAMWORLD_API FDWVitalityBasicSaveData : public FSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWVitalityBasicSaveData()
	{
		Name = NAME_None;
		RaceID = NAME_None;
		Level = 0;
		EXP = 0;
		SpawnLocation = FVector::ZeroVector;
		SpawnRotation = FRotator::ZeroRotator;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPrimaryAssetId ID;

	UPROPERTY(BlueprintReadWrite)
	FName Name;
		
	UPROPERTY(BlueprintReadOnly)
	FName RaceID;

	UPROPERTY(BlueprintReadWrite)
	int32 Level;

	UPROPERTY(BlueprintReadOnly)
	int32 EXP;
	
	UPROPERTY()
	FVector SpawnLocation;
	
	UPROPERTY()
	FRotator SpawnRotation;

	UPROPERTY()
	FAbilityData DefaultAbility;

	UPROPERTY(BlueprintReadWrite)
	FInventorySaveData InventoryData;

public:
	virtual void Initialize();
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWVitalitySaveData : public FDWVitalityBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWVitalitySaveData()
	{
	}

public:
	virtual void Initialize() override;
	
	class UDWVitalityData& GetVitalityData() const;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterBasicSaveData : public FDWVitalityBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWCharacterBasicSaveData()
	{
		TeamID = NAME_None;
	}

public:
	UPROPERTY(BlueprintReadOnly)
	FName TeamID;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterSaveData : public FDWCharacterBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWCharacterSaveData()
	{
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

	UPROPERTY()
	FDWCharacterAttackAbilityData FallingAttackAbility;

	UPROPERTY()
	TArray<FDWCharacterAttackAbilityData> AttackAbilities;

	UPROPERTY()
	TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData> SkillAbilities;

	UPROPERTY()
	TMap<EDWCharacterActionType, FDWCharacterActionAbilityData> ActionAbilities;

public:
	virtual void Initialize() override;

	class UDWCharacterData& GetCharacterData() const;
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
	}
	
	FORCEINLINE FDWPlayerSaveData(FDWPlayerBasicSaveData InBasicSaveData)
	{
		ID = InBasicSaveData.ID;
	
		ArchiveID = 0;
	}
	
	struct FDWArchiveBasicSaveData GetArchiveData() const;

	UPROPERTY(BlueprintReadOnly)
	int32 ArchiveID;
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
		ArchiveID = 0;
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

		ArchiveID = 0;

		LastVitalityRaceIndex = FIndex::ZeroIndex;
		LastCharacterRaceIndex = FIndex::ZeroIndex;
	}

public:
	static const FDWVoxelWorldSaveData Empty;

	UPROPERTY(BlueprintReadOnly)
	int32 ArchiveID;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FVector, FDWVoxelChunkSaveData> ChunkDatas;

	FIndex LastVitalityRaceIndex;

	FIndex LastCharacterRaceIndex;

public:
	virtual void Initialize() override;
	
	FDWArchiveBasicSaveData GetArchiveData() const;

	FORCEINLINE bool IsSameArchive(FDWVoxelWorldSaveData InSaveData) const
	{
		return InSaveData.ArchiveID == ArchiveID;
	}
	
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

public:
	void Initialize()
	{
		PlayerData.ArchiveID = ID;
		PlayerData.Initialize();
		
		WorldData.ArchiveID = ID;
		WorldData.Initialize();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWGeneralSaveData : public FSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWGeneralSaveData()
	{
		bAutoJump = true;
		CameraDistance = 150.f;
		CurrentArchiveID = -1;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentArchiveID;

	UPROPERTY()
	TMap<int32, FDWArchiveBasicSaveData> ArchiveBasicDatas;
	
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
enum class EDWAttackType : uint8
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
