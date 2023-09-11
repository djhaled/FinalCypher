// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponPickup.generated.h"




UCLASS()
class FINALCYPHER_API AWeaponPickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponPickup();


protected:
	// Called when the game starts or when spawned

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Pickup;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponM;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class AWeaponBase> WeaponClass;

	

public:

	
	UFUNCTION(BlueprintCallable)
	void UpdatePickup(FPickupData NewWeaponPickupData);

	UFUNCTION(Server, Reliable)
	void Server_UpdatePickup(FPickupData NewWeaponPickupData);

	UFUNCTION(BlueprintImplementableEvent,Category="Test")
	void UpdatePik_TEST(FPickupData NewWeaponPickupData);
	// Multicast function to update clients
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdatePickup(FPickupData NewWeaponPickupData);
	//

	UPROPERTY(VisibleAnywhere)
	AWeaponBase* WeaponPickupActor;

};
