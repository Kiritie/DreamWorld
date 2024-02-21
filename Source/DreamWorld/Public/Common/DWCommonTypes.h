#pragma once

#include "GameplayEffectTypes.h"
#include "NativeGameplayTags.h"
#include "Ability/AbilityModuleTypes.h"
#include "Achievement/AchievementModuleTypes.h"
#include "SaveGame/SaveGameModuleTypes.h"
#include "Voxel/VoxelModuleTypes.h"
#include "Asset/AssetModuleTypes.h"
#include "Audio/AudioModuleTypes.h"
#include "Camera/CameraModuleTypes.h"
#include "Common/CommonTypes.h"
#include "Input/InputModuleTypes.h"
#include "Task/TaskModuleTypes.h"
#include "Team/DWTeamModuleTypes.h"
#include "Video/VideoModuleTypes.h"

#include "DWCommonTypes.generated.h"

class UBehaviorTree;
class UVoxelModule;
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
class UAbilityInventoryBase;
class UAbilityCharacterInventoryBase;
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
class UAbilityInventorySlot;

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
	None,
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
	}

	FORCEINLINE FDWPlayerBasicSaveData(const FDWCharacterSaveData& InCharacterSaveData) : FDWCharacterSaveData(InCharacterSaveData)
	{
	}
};

UENUM(BlueprintType)
enum class EDWInventoryInitType : uint8
{
	// 默认
	Default UMETA(DisplayName = "默认"),
	// 无
	Empty UMETA(DisplayName = "无"),
	// 充满（调试用）
	All UMETA(DisplayName = "充满（调试用）")
};

UENUM(BlueprintType)
enum class EDWTimeSection : uint8
{
	// 早上
	Morn UMETA(DisplayName = "早上"),
	// 中午
	Noon UMETA(DisplayName = "中午"),
	// 晚上
	Night UMETA(DisplayName = "晚上")
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
		InventoryInitType = EDWInventoryInitType::Default;
	}
		
	FORCEINLINE FDWPlayerSaveData(const FDWCharacterSaveData& InCharacterSaveData) : FDWPlayerBasicSaveData(InCharacterSaveData)
	{
		ArchiveID = 0;
		BodyColorIndex = 0;
		CapeColorIndex = 0;
		InventoryInitType = EDWInventoryInitType::Default;
	}

	FORCEINLINE FDWPlayerSaveData(const FDWPlayerBasicSaveData& InBasicSaveData) : FDWPlayerBasicSaveData(InBasicSaveData)
	{
		ArchiveID = 0;
		BodyColorIndex = 0;
		CapeColorIndex = 0;
		InventoryInitType = EDWInventoryInitType::Default;
	}

	UPROPERTY(BlueprintReadOnly)
	int32 ArchiveID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BodyColorIndex;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CapeColorIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWInventoryInitType InventoryInitType;
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
struct DREAMWORLD_API FDWWorldSaveData : public FVoxelWorldSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWWorldSaveData()
	{
		ChunkDatas = TMap<FVector, FDWVoxelChunkSaveData>();
	}
	
	FORCEINLINE FDWWorldSaveData(const FVoxelWorldBasicSaveData& InBasicSaveData) : FVoxelWorldSaveData(InBasicSaveData)
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
		for(auto& Iter : ChunkDatas)
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
		return ChunkDatas.Find(InChunkIndex.ToVector());
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
struct DREAMWORLD_API FDWAudioModuleSaveData : public FAudioModuleSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWAudioModuleSaveData()
	{
	}

	FORCEINLINE FDWAudioModuleSaveData(const FAudioModuleSaveData& InAudioModuleSaveData) : FAudioModuleSaveData(InAudioModuleSaveData)
	{
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCameraModuleSaveData : public FCameraModuleSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWCameraModuleSaveData()
	{
	}

	FORCEINLINE FDWCameraModuleSaveData(const FCameraModuleSaveData& InCameraModuleSaveData) : FCameraModuleSaveData(InCameraModuleSaveData)
	{
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWVideoModuleSaveData : public FVideoModuleSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWVideoModuleSaveData()
	{
	}

	FORCEINLINE FDWVideoModuleSaveData(const FVideoModuleSaveData& InVideoModuleSaveData) : FVideoModuleSaveData(InVideoModuleSaveData)
	{
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWTaskModuleSaveData : public FTaskModuleSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWTaskModuleSaveData()
	{
	}

	FORCEINLINE FDWTaskModuleSaveData(const FTaskModuleSaveData& InTaskModuleSaveData) : FTaskModuleSaveData(InTaskModuleSaveData)
	{
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWAchievementModuleSaveData : public FAchievementModuleSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWAchievementModuleSaveData()
	{
	}

	FORCEINLINE FDWAchievementModuleSaveData(const FAchievementModuleSaveData& InAchievementModuleSaveData) : FAchievementModuleSaveData(InAchievementModuleSaveData)
	{
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWInputModuleSaveData : public FInputModuleSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWInputModuleSaveData()
	{
	}

	FORCEINLINE FDWInputModuleSaveData(const FInputModuleSaveData& InInputModuleSaveData) : FInputModuleSaveData(InInputModuleSaveData)
	{
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWArchiveSaveData : public FDWArchiveBasicSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWArchiveSaveData()
	{
		WorldData = FDWWorldSaveData();
		PlayerData = FDWPlayerSaveData();
		AchievementData = FDWAchievementModuleSaveData();
		TaskData = FDWTaskModuleSaveData();
		TeamData = FDWTeamModuleSaveData();
	}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FDWWorldSaveData WorldData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FDWPlayerSaveData PlayerData;

	UPROPERTY(VisibleAnywhere)
	FDWAchievementModuleSaveData AchievementData;

	UPROPERTY(VisibleAnywhere)
	FDWTaskModuleSaveData TaskData;

	UPROPERTY(VisibleAnywhere)
	FDWTeamModuleSaveData TeamData;

public:
	virtual void MakeSaved() override
	{
		Super::MakeSaved();
		
		PlayerData.MakeSaved();
		WorldData.MakeSaved();
		AchievementData.MakeSaved();
		TaskData.MakeSaved();
		TeamData.MakeSaved();
	}
};

UENUM(BlueprintType)
enum class EDWGameLevel : uint8
{
	// 简单
	Sample UMETA(DisplayName = "简单"),
	// 普通
	Normal UMETA(DisplayName = "普通"),
	// 困难
	Hard UMETA(DisplayName = "困难")
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWGameSaveData : public FSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWGameSaveData()
	{
		GameLevel = EDWGameLevel::Normal;
		bAutoJump = true;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDWGameLevel GameLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoJump;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWSettingModuleSaveData : public FSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWSettingModuleSaveData()
	{
		GameData = FDWGameSaveData();
		AudioData = FDWAudioModuleSaveData();
		VideoData = FDWVideoModuleSaveData();
		CameraData = FDWCameraModuleSaveData();
		InputData = FDWInputModuleSaveData();
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FDWGameSaveData GameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	FDWAudioModuleSaveData AudioData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video")
	FDWVideoModuleSaveData VideoData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FDWCameraModuleSaveData CameraData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FDWInputModuleSaveData InputData;

public:
	virtual void MakeSaved() override
	{
		Super::MakeSaved();
		
		GameData.MakeSaved();
		AudioData.MakeSaved();
		VideoData.MakeSaved();
		CameraData.MakeSaved();
		InputData.MakeSaved();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWGenerateRawData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWGenerateRawData()
	{
		Raws = TArray<FAbilityItem>();
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAbilityItem> Raws;
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWGenerateItemData : public FDataTableRowBase
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWGenerateItemData()
	{
		ToolID = FPrimaryAssetId();
		Item = FAbilityItem();
		RawDatas = TArray<FDWGenerateRawData>();
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPrimaryAssetId ToolID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAbilityItem Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDWGenerateRawData> RawDatas;
};

/**
 * ???????????
 */
UENUM(BlueprintType)
enum class EDWGameTraceChannel : uint8
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

namespace GameplayTags
{
	////////////////////////////////////////////////////
	// Input_Shortcut
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_InventoryAll);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_InventorySingle);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_InventorySplit);

	////////////////////////////////////////////////////
	// Input_Player
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Sprint);
	
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Interact1);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Interact2);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Interact3);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Interact4);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Interact5);
	
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Dodge);
	
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ToggleCrouch);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ToggleControlMode);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ToggleLockSightTarget);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ChangeHand);
	
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Primary);
	
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Minor);
	
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ReleaseSkillAbility1);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ReleaseSkillAbility2);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ReleaseSkillAbility3);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ReleaseSkillAbility4);
	
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UseInventoryItem);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_DiscardInventoryItem);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_PrevInventorySlot);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_NextInventorySlot);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SelectInventorySlot1);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SelectInventorySlot2);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SelectInventorySlot3);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SelectInventorySlot4);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SelectInventorySlot5);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SelectInventorySlot6);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SelectInventorySlot7);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SelectInventorySlot8);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SelectInventorySlot9);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_SelectInventorySlot10);

	////////////////////////////////////////////////////
	// Input_System
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ZoomInMiniMap);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ZoomOutMiniMap);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_OpenMaxMapBox);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_OpenInventoryPanel);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_OpenGeneratePanel);

	////////////////////////////////////////////////////
	// State_Character
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Dodging);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Sprinting);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Crouching);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Swimming);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Floating);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Climbing);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Riding);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Flying);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Attacking);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_NormalAttacking);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_FallingAttacking);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_SkillAttacking);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Defending);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Interrupting);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_Exhausted);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_FreeToAnim);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_LockRotation);
	DREAMWORLD_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTag_Character_BreakAllInput);
};
