// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponPickupManager.generated.h"



class AWeaponPickup;
class AWeaponBase;

USTRUCT(BlueprintType)
struct FPickupData
{
	GENERATED_BODY() // Make sure to include this macro

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeaponBase> Pickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PickupTime;

	AWeaponPickup* CurrentPickup;

	USkeletalMesh* PickupMesh;
};
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALCYPHER_API UWeaponPickupManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponPickupManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Function to spawn a weapon pickup
	UFUNCTION(BlueprintCallable, Category = "WeaponPickup")
	void SpawnInitialPickups();
	UFUNCTION(BlueprintCallable, Category = "WeaponPickup")
	void SpawnPickup(FPickupData Pickup);
	UFUNCTION(BlueprintCallable, Category = "WeaponPickup")
	FPickupData GetAvailableWeapon();
	UFUNCTION(BlueprintCallable, Category = "WeaponPickup")
	AWeaponPickup* GetAvailablePickup();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Info", MakeStructureDefaultValue="None"))
	TObjectPtr<UDataTable> WeaponsDataTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Data Table Main", MakeStructureDefaultValue="None"))
	TObjectPtr<UDataTable> WeaponsOtherDT;
private:
	// Array of available weapon data
	UPROPERTY(EditAnywhere, Category = "WeaponPickup")
	TArray<FPickupData> AvailableWeapons;

	TArray<AWeaponPickup*> WeaponPickups;
	UFUNCTION(BlueprintCallable)
	void InitDataToStruct();
	UFUNCTION(BlueprintCallable)
	void InitializeEmptyPickups();
	// Interval between pickups
	UPROPERTY(EditAnywhere, Category = "WeaponPickup")
	float SpawnInterval;

	// Duration of each pickup
	UPROPERTY(EditAnywhere, Category = "WeaponPickup")
	float SpawnDuration;

	// Timer handle for managing pickup duration
	FTimerHandle PickupDurationTimerHandle;

	// Function to choose a weapon randomly based on probabilities
	AWeaponBase* ChooseWeaponToSpawn();

	// Function to handle pickup expiration
	void ExpirePickup();
};
