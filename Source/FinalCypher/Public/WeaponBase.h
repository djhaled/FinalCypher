// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSAnimBlueprintSettings
{
	GENERATED_BODY()
public:
	/** Pointer to the animation settings to use. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Row Handle Settings Animation"))
	FDataTableRowHandle RowHandleSettingsAnimation;

	/** All of this character's poses. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Poses Character", MakeStructureDefaultValue="DataTable'/Game/LowPolyShooterPack/Data/Testing/Abilities/Character/DT_LPSP_CH_AR_01_Poses.DT_LPSP_CH_AR_01_Poses'"))
	TObjectPtr<UDataTable> DataTablePosesCharacter;

	/** All of this character's montages. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Montages Character", MakeStructureDefaultValue="DataTable'/Game/LowPolyShooterPack/Data/Testing/Abilities/Character/DT_LPSP_CH_AR_01_Montages.DT_LPSP_CH_AR_01_Montages'"))
	TObjectPtr<UDataTable> DataTableMontagesCharacter;

	/** All of this weapon's montages. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Montages Weapon", MakeStructureDefaultValue="DataTable'/Game/LowPolyShooterPack/Data/Testing/Abilities/Weapon/DT_LPSP_WEP_AR_01_Montages.DT_LPSP_WEP_AR_01_Montages'"))
	TObjectPtr<UDataTable> DataTableMontagesWeapon;
};


/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSAttachmentSettings
{
	GENERATED_BODY()
public:
	/** Scope Settings. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Settings Scope", MakeStructureDefaultValue="DataTable'/Game/LowPolyShooterPack/Data/Testing/Settings/DT_LPSP_WEP_Settings_Scopes.DT_LPSP_WEP_Settings_Scopes'"))
	TObjectPtr<UDataTable> DataTableSettingsScope;

	/** Laser Settings. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Settings Laser", MakeStructureDefaultValue="DataTable'/Game/LowPolyShooterPack/Data/Testing/Settings/DT_LPSP_WEP_Settings_Lasers.DT_LPSP_WEP_Settings_Lasers'"))
	TObjectPtr<UDataTable> DataTableSettingsLaser;

	/** Muzzle Settings. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Settings Muzzle", MakeStructureDefaultValue="DataTable'/Game/LowPolyShooterPack/Data/Testing/Settings/DT_LPSP_WEP_AR_01_Settings_Muzzles.DT_LPSP_WEP_AR_01_Settings_Muzzles'"))
	TObjectPtr<UDataTable> DataTableSettingsMuzzle;

	/** Grip Settings. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Settings Grip", MakeStructureDefaultValue="DataTable'/Game/LowPolyShooterPack/Data/Testing/Settings/DT_LPSP_WEP_Settings_Grips.DT_LPSP_WEP_Settings_Grips'"))
	TObjectPtr<UDataTable> DataTableSettingsGrip;
};


/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSInformation : public FTableRowBase
{
	GENERATED_BODY()
public:
	/** The name of the weapon using this data. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Weapon Name"))
	FText WeaponName;

	/** User Widget used as the player's crosshair while wielding this weapon. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Widget Class Crosshair", MakeStructureDefaultValue="None"))
	TObjectPtr<UClass> WidgetClassCrosshair;

	/** Information for all the attachment's settings. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Settings Attachments", MakeStructureDefaultValue="(DataTableSettingsScope_9_C9BB0468415F89979F3AE1B26A90DC96=None,DataTableSettingsLaser_10_C588AE4C45BC513E7DB17EB5CE711B4F=None,DataTableSettingsMuzzle_11_1443C9F14E0B8318B88A74B6F20598E5=None,DataTableSettingsGrip_12_5321571345181DA7A816DDABE411C090=None)"))
	FSAttachmentSettings SettingsAttachments;

	/** Animation Blueprint Settings. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Settings Animation Blueprint"))
	FSAnimBlueprintSettings SettingsAnimationBlueprint;

	/** Weapon Settings. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Row Handle Settings Weapon"))
	FDataTableRowHandle RowHandleSettingsWeapon;

	/** Character Abilities. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Row Handle Settings Character Abilities"))
	FDataTableRowHandle RowHandleSettingsCharacterAbilities;

	/** Character Movement. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Row Handle Settings Character Movement"))
	FDataTableRowHandle RowHandleSettingsCharacterMovement;

	/** All icons that could be used for this weapon. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Icons", MakeStructureDefaultValue="None"))
	TObjectPtr<UDataTable> DataTableIcons;

	/** All sound cues that can be used for this weapon. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Sound Cues", MakeStructureDefaultValue="None"))
	TObjectPtr<UDataTable> DataTableSoundCues;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="WeaponMesh"))
	TObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="WeightSpawn"))
	float WeightSpawn;
};
/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSInventorySlot: public FTableRowBase
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Item", MakeStructureDefaultValue="None"))
	TObjectPtr<UClass> Item;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="SkelMesh", MakeStructureDefaultValue="None"))
	TObjectPtr<USkeletalMesh> SkelMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="SkelMesh", MakeStructureDefaultValue="None"))
	float Weight;
};


UCLASS()
class FINALCYPHER_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeapMesh;

	UFUNCTION(BlueprintCallable)
	void UpdateMesh(USkeletalMesh* Skelmesh);
	
	/** Contains the Weapon’s Information. A struct that has lots of the important values a Weapon needs. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Infima Games | Low Poly Shooter Pack | Data", meta=(MultiLine="true"))
	FSInformation WeaponInfo;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Infima Games | Low Poly Shooter Pack | Weapon | Settings", meta=(MultiLine="true"))
	FDataTableRowHandle RowHandleWeaponInformation;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	USkeletalMesh* GetWeaponMesh();
};
