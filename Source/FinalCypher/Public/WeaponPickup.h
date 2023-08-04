// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FinalCypher/FinalCypher.h"
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
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* Pickup;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* WeaponM;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class AWeaponBase> WeaponClass;

	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called when the player interacts with the pickup
	UFUNCTION(BlueprintCallable)
	void UpdatePickup(FPickupData WeaponPickupData);
	
	//

	UPROPERTY(VisibleAnywhere)
	AWeaponBase* WeaponPickupActor;
	UFUNCTION()
	void OnInteract(AActor* Interactor);
};
