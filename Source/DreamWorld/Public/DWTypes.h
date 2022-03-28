#pragma once

#include "Engine/DataTable.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "Ability/AbilityModuleTypes.h"
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

#define Vector_Empty FVector(MAX_flt)

/**
 * 游戏状态
 */
UENUM(BlueprintType)
enum class EGameState : uint8
{
	// ??
	None,
	// ?????
	Initializing,
	// ?????
	MainMenu,
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
enum class EControlMode : uint8
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
enum class ECharacterNature : uint8
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
struct DREAMWORLD_API FTeamData
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
	
	static FTeamData Empty;

	FORCEINLINE FTeamData()
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
enum class ECharacterActionType : uint8
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
enum class EPropType : uint8
{
	// ??
	Potion,
	// ???
	Food,
	// ????
	Container
};

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	// ????
	Weapon,
	// ????
	Shield,
	// ????
	Armor
};

UENUM(BlueprintType)
enum class EEquipPartType : uint8
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
enum class EWeaponType : uint8
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
enum class EShieldType : uint8
{
	// 无
	None,
	// 物理
	Physics,
	// 魔法
	Magic
};

UENUM(BlueprintType)
enum class EWeaponHandType : uint8
{
	// ????
	Single,
	// ???
	Both
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FQueryItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FItem Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UInventorySlot*> Slots;

public:
	FORCEINLINE FQueryItemInfo()
	{
		Item = FItem();
		Slots = TArray<UInventorySlot*>();
	}

	FORCEINLINE FQueryItemInfo(FItem InItem, TArray<UInventorySlot*> InSlots)
	{
		Item = InItem;
		Slots = InSlots;
	}
};

/**
 * ???????
 */
USTRUCT(BlueprintType)
struct DREAMWORLD_API FRaceData : public FTableRowBase
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

	FORCEINLINE FRaceData()
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
struct DREAMWORLD_API FVitalityRaceData : public FRaceData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItem> Items;

	FORCEINLINE FVitalityRaceData()
	{
		Items = TArray<FItem>();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FCharacterRaceData : public FRaceData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItem> Items;

	FORCEINLINE FCharacterRaceData()
	{
		Items = TArray<FItem>();
	}
};

UENUM(BlueprintType)
enum class ESplitSlotType : uint8
{
	None,
	Default,
	Shortcut,
	Auxiliary,
	Generate,
	Equip,
	Skill
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FSplitSlotInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 StartIndex;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 TotalCount;

	FORCEINLINE FSplitSlotInfo()
	{
		StartIndex = 0;
		TotalCount = 0;
	}

	FORCEINLINE FSplitSlotInfo(int32 InStartIndex, int32 InTotalCount)
	{
		StartIndex = InStartIndex;
		TotalCount = InTotalCount;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FSplitSlotData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UInventorySlot*> Slots;

	FORCEINLINE FSplitSlotData()
	{
		Slots = TArray<UInventorySlot*>();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FWidgetSplitSlotData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UWidgetInventorySlot*> Slots;

	FORCEINLINE FWidgetSplitSlotData()
	{
		Slots = TArray<UWidgetInventorySlot*>();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FInventorySaveData : public FSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ESplitSlotType, FSplitSlotInfo> SplitInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItem> Items;
		
	FORCEINLINE FInventorySaveData()
	{
		SplitInfos = TMap<ESplitSlotType, FSplitSlotInfo>();
		Items = TArray<FItem>();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterActionAbilityData : public FAbilityData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterActionType ActionType;
	
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
		ActionType = ECharacterActionType::None;
		AbilityClass = nullptr;
		bCancelable = true;
		bNeedActive = true;
		bNeedFreeToAnim = false;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FCharacterAttackAbilityData : public FAbilityData
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDWCharacterAttackAbility> AbilityClass;

	FORCEINLINE FCharacterAttackAbilityData()
	{
		WeaponType = EWeaponType::None;
		AbilityClass = nullptr;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterAttackAbilityData : public FAbilityData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDWCharacterAttackAbility> AbilityClass;

	FORCEINLINE FDWCharacterAttackAbilityData()
	{
		WeaponType = EWeaponType::None;
		AbilityClass = nullptr;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterSkillAbilityData : public FAbilityData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDWCharacterSkillAbility> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCancelable;

	FORCEINLINE FDWCharacterSkillAbilityData()
	{
		WeaponType = EWeaponType::None;
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
struct DREAMWORLD_API FVitalityBasicSaveData : public FSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FVitalityBasicSaveData()
	{
		Name = TEXT("");
		RaceID = TEXT("");
		Level = 0;
		EXP = 0;
		SpawnLocation = FVector::ZeroVector;
		SpawnRotation = FRotator::ZeroRotator;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPrimaryAssetId ID;

	UPROPERTY(BlueprintReadWrite)
	FString Name;
		
	UPROPERTY(BlueprintReadOnly)
	FString RaceID;

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
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FVitalitySaveData : public FVitalityBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FVitalitySaveData()
	{
	}

public:
	class UVitalityAssetBase* GetVitalityData() const;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FCharacterBasicSaveData : public FVitalityBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FCharacterBasicSaveData()
	{
		TeamID = TEXT("");
	}

public:
	UPROPERTY(BlueprintReadOnly)
	FString TeamID;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FCharacterSaveData : public FCharacterBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FCharacterSaveData()
	{
		Nature = ECharacterNature::AIHostile;
		AttackDistance = 100.f;
		InteractDistance = 500.f;
		FollowDistance = 500.f;
		PatrolDistance = 1000.f;
		PatrolDuration = 10.f;
		FallingAttackAbility = FDWCharacterAttackAbilityData();
		AttackAbilities = TArray<FDWCharacterAttackAbilityData>();
		SkillAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAbilityData>();
		ActionAbilities = TMap<ECharacterActionType, FDWCharacterActionAbilityData>();
	}

	UPROPERTY()
	ECharacterNature Nature;
			
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
	TMap<ECharacterActionType, FDWCharacterActionAbilityData> ActionAbilities;

public:
	class UCharacterAssetBase* GetCharacterData() const;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FPlayerBasicSaveData : public FCharacterSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FPlayerBasicSaveData()
	{
		CameraRotation = FRotator::ZeroRotator;
	}

	UPROPERTY(EditDefaultsOnly)
	FRotator CameraRotation;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FPlayerSaveData : public FPlayerBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FPlayerSaveData()
	{
		ArchiveID = 0;
	}
	
	FORCEINLINE FPlayerSaveData(FPlayerBasicSaveData InBasicSaveData)
	{
		ID = InBasicSaveData.ID;
	
		ArchiveID = 0;
	}
	
	struct FArchiveBasicSaveData GetArchiveData() const;

	UPROPERTY(BlueprintReadOnly)
	int32 ArchiveID;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWChunkSaveData : public FChunkSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FCharacterSaveData> CharacterDatas;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FVitalitySaveData> VitalityDatas;

	FORCEINLINE FDWChunkSaveData()
	{
		CharacterDatas = TArray<FCharacterSaveData>();
		VitalityDatas = TArray<FVitalitySaveData>();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWWorldSaveData : public FWorldSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWWorldSaveData()
	{
		ArchiveID = 0;
		WorldSeed = 0;
		TimeSeconds = 0.f;

		LastVitalityRaceIndex = FIndex::ZeroIndex;
		LastCharacterRaceIndex = FIndex::ZeroIndex;
	}
	
	FORCEINLINE FDWWorldSaveData(FWorldBasicSaveData InBasicSaveData)
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
	static const FDWWorldSaveData Empty;

	UPROPERTY(BlueprintReadOnly)
	int32 ArchiveID;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FVector, FDWChunkSaveData> ChunkDatas;

	FIndex LastVitalityRaceIndex;

	FIndex LastCharacterRaceIndex;

public:
	FArchiveBasicSaveData GetArchiveData() const;

	FORCEINLINE bool IsSameArchive(FDWWorldSaveData InSaveData) const
	{
		return InSaveData.ArchiveID == ArchiveID;
	}

	FORCEINLINE float GetChunkLength() const
	{
		return ChunkSize * BlockSize;
	}

	FORCEINLINE int32 GetWorldHeight() const
	{
		return ChunkSize * ChunkHeightRange;
	}

	FORCEINLINE FChunkMaterial GetChunkMaterial(ETransparency Transparency) const
	{
		const int32 Index = FMath::Clamp((int32)Transparency, 0, ChunkMaterials.Num());
		if(ChunkMaterials.IsValidIndex(Index))
		{
			return ChunkMaterials[Index];
		}
		return FChunkMaterial();
	}

	FORCEINLINE FVector GetBlockSizedNormal(FVector InNormal, float InLength = 0.25f) const
	{
		return BlockSize * InNormal * InLength;
	}
	
	bool IsExistChunkData(FIndex InChunkIndex) const
	{
		return ChunkDatas.Contains(InChunkIndex.ToVector());
	}

	FDWChunkSaveData& GetChunkData(FIndex InChunkIndex) const
	{
		static FDWChunkSaveData ChunkData;
		if (ChunkDatas.Contains(InChunkIndex.ToVector()))
		{
			ChunkData = ChunkDatas[InChunkIndex.ToVector()];
		}
		return ChunkData;
	}

	void SetChunkData(FIndex InChunkIndex, FDWChunkSaveData InChunkData)
	{
		if (!ChunkDatas.Contains(InChunkIndex.ToVector()))
			ChunkDatas.Add(InChunkIndex.ToVector(), InChunkData);
		else
			ChunkDatas[InChunkIndex.ToVector()] = InChunkData;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FArchiveBasicSaveData : public FSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FArchiveBasicSaveData()
	{
		ID = -1;
		PlayerBasicData = FPlayerBasicSaveData();
		WorldBasicData = FWorldBasicSaveData();
		bPreview = false;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPlayerBasicSaveData PlayerBasicData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FWorldBasicSaveData WorldBasicData;

	UPROPERTY(BlueprintReadOnly)
	bool bPreview;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FArchiveSaveData : public FArchiveBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FArchiveSaveData()
	{
		PlayerData = FPlayerSaveData();
		WorldData = FDWWorldSaveData();
	}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FPlayerSaveData PlayerData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FDWWorldSaveData WorldData;

public:
	void Initialize()
	{
		PlayerData.ArchiveID = ID;
		WorldData.ArchiveID = ID;
		if(WorldData.WorldSeed == 0)
		{
			WorldData.WorldSeed = FMath::Rand();
		}
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FGeneralSaveData : public FSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FGeneralSaveData()
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FArchiveBasicSaveData> ArchiveBasicDatas;
};

/**
 * ???????????
 */
UENUM(BlueprintType)
enum class EGameTraceType : uint8
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
 * ?????????????
 */
UENUM(BlueprintType)
enum class EQueryItemType : uint8
{
	// ???
	Addition,
	// ???
	Remove,
	// ???
	Clear
};

/**
 * ???AI??
 */
UENUM(BlueprintType)
enum class ECharacterAIState : uint8
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
enum class EAttackType : uint8
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
enum class ECharacterPartType : uint8
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
