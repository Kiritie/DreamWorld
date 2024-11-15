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

class UCharacterAbilityBase;
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
class AAbilityProjectileBase;
class UPropEffectBase;
class UEquipEffectBase;
class UAbilityBase;
class UAttributeSetBase;
class UDWCharacterAttributeSet;
class UTargetType;
class UAbilityInventorySlotBase;

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
	// ???????
	Player,
	// ????AI
	AIFriendly,
	// ??????AI
	AINeutral,
	// ?ж??AI
	AIHostile
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
	// 武器
	Weapon,
	// 防具
	Armor
};

UENUM(BlueprintType)
enum class EDWEquipPart : uint8
{
	// 头部
	Head,
	// 胸部
	Chest,
	// 手臂
	Arm,
	// 腿部
	Leg,
	// 主武器
	Primary,
	// 次武器
	Secondary
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
	Wand,
	// 盾牌
	Shield
};

UENUM(BlueprintType)
enum class EDWWeaponPart : uint8
{
	// 无
	None,
	// 主要
	Primary = EDWEquipPart::Primary,
	// 次要
	Secondary = EDWEquipPart::Secondary
};

UENUM(BlueprintType)
enum class EDWWeaponHand : uint8
{
	// 无
	None,
	// ????
	Single,
	// ???
	Both
};

/**
* 技能类型
*/
UENUM(BlueprintType)
enum class EDWSkillType : uint8
{
	// 无
	None,
	// 攻击
	Attack,
	// 法术
	Magic,
	// 盾
	Shield,
	// 传送
	Teleport,
	// 隐身
	Stealth
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterAttackAbilityData : public FAbilityData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAbilityBase> AbilityClass;

	FORCEINLINE FDWCharacterAttackAbilityData()
	{
		AbilityClass = nullptr;
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterAttackAbilities
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDWCharacterAttackAbilityData> Abilities;

	FORCEINLINE FDWCharacterAttackAbilities()
	{
		Abilities = TArray<FDWCharacterAttackAbilityData>();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterAttackAbilityQueue
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Index;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FDWCharacterAttackAbilityData> Abilities;

	FORCEINLINE FDWCharacterAttackAbilityQueue()
	{
		Index = 0;
		Abilities = TArray<FDWCharacterAttackAbilityData>();
	}

public:
	FORCEINLINE void Reset()
	{
		Index = 0;
	}

	FORCEINLINE void Prev()
	{
		if (--Index < 0)
		{
			Index = Abilities.Num();
		}
	}

	FORCEINLINE void Next()
	{
		if (++Index == Abilities.Num())
		{
			Index = 0;
		}
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterFallingAttackAbilityData : public FDWCharacterAttackAbilityData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWCharacterFallingAttackAbilityData()
	{
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterSkillAttackAbilityData : public FDWCharacterAttackAbilityData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWCharacterSkillAttackAbilityData()
	{
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWCharacterSkillAttackAbilities
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDWCharacterSkillAttackAbilityData> Abilities;

	FORCEINLINE FDWCharacterSkillAttackAbilities()
	{
		Abilities = TArray<FDWCharacterSkillAttackAbilityData>();
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
		ControlMode = EDWCharacterControlMode::Fighting;
		AttackAbilityQueues = TMap<EDWWeaponType, FDWCharacterAttackAbilityQueue>();
		FallingAttackAbilities = TMap<EDWWeaponType, FDWCharacterFallingAttackAbilityData>();
		SkillAttackAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAttackAbilityData>();
	}
	
	FORCEINLINE FDWCharacterSaveData(const FCharacterSaveData& InCharacterSaveData) : FCharacterSaveData(InCharacterSaveData)
	{
		TeamID = NAME_None;
		ControlMode = EDWCharacterControlMode::Fighting;
		AttackAbilityQueues = TMap<EDWWeaponType, FDWCharacterAttackAbilityQueue>();
		FallingAttackAbilities = TMap<EDWWeaponType, FDWCharacterFallingAttackAbilityData>();
		SkillAttackAbilities = TMap<FPrimaryAssetId, FDWCharacterSkillAttackAbilityData>();
	}
	
	UPROPERTY(BlueprintReadOnly)
	FName TeamID;

	UPROPERTY()
	EDWCharacterControlMode ControlMode;

	UPROPERTY()
	TMap<EDWWeaponType, FDWCharacterAttackAbilityQueue> AttackAbilityQueues;

	UPROPERTY()
	TMap<EDWWeaponType, FDWCharacterFallingAttackAbilityData> FallingAttackAbilities;

	UPROPERTY()
	TMap<FPrimaryAssetId, FDWCharacterSkillAttackAbilityData> SkillAttackAbilities;
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

public:
	virtual void InitInventoryData() override;
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
	// ???????
	SkillAttack
};

UENUM(BlueprintType)
enum class EDWCharacterPart : uint8
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

	virtual void HandleDamage(AActor* SourceActor, AActor* TargetActor, float DamageValue, const FGameplayAttribute& DamageAttribute, const FHitResult& HitResult, const FGameplayTagContainer& SourceTags) override;
};

namespace GameplayTags
{
	////////////////////////////////////////////////////
	// Input_Shortcut
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_InventoryAll);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_InventorySingle);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_InventorySplit);

	////////////////////////////////////////////////////
	// Input_Player
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Sprint);
	
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Interact1);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Interact2);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Interact3);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Interact4);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Interact5);
	
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_NextInteract);
	
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Dodge);
	
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ToggleCrouch);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ToggleControlMode);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ToggleLockSightTarget);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ChangeHand);
	
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Primary);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Secondary);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Third);
	
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ReleaseSkillAbility1);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ReleaseSkillAbility2);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ReleaseSkillAbility3);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ReleaseSkillAbility4);
	
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_UseInventoryItem);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_DiscardInventoryItem);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_PrevInventorySlot);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_NextInventorySlot);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SelectInventorySlot1);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SelectInventorySlot2);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SelectInventorySlot3);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SelectInventorySlot4);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SelectInventorySlot5);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SelectInventorySlot6);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SelectInventorySlot7);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SelectInventorySlot8);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SelectInventorySlot9);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SelectInventorySlot10);

	////////////////////////////////////////////////////
	// Input_System
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ZoomInMiniMap);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ZoomOutMiniMap);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_OpenMaxMapBox);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_OpenInventoryPanel);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_OpenGeneratePanel);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_OpenTaskPanel);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_OpenContextInputBox);

	////////////////////////////////////////////////////
	// State_Character
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Character_Dodging);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Character_Sprinting);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Character_Riding);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Character_Aiming);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Character_Attacking);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Character_Defending);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Character_Exhausted);

	////////////////////////////////////////////////////
	// Ability_Character_Action
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Dodge);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Sprint);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Ride);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Take);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Use);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Discard);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Generate);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Destroy);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Aim);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Attack);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_GetHit);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_Defend);
	DREAMWORLD_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Character_Action_DefendBlock);
};
