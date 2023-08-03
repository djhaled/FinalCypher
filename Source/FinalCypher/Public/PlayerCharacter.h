// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterPlayerStart.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FINALCYPHER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(ReplicatedUsing = OnRep_TeamID, EditAnywhere, BlueprintReadOnly, Category = "Team")
		uint8 TeamID;
	// Replicated TeamID property to any relevant client
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	UFUNCTION(BlueprintImplementableEvent)
		void OnRep_TeamID();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = "Team", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UHealthComponent* HealthComp;

	UFUNCTION(BlueprintCallable, Category = "Team")
		uint8 GetTeamID() const;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable, Category = "Team")
		uint8 SetTeamID(uint8 tID);
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "PlayerCharacter")
		bool IsTeammate(APlayerCharacter* ComparedPlayer);
	FVector LastSpawnPoint;
	bool IsAlive = true;
	void AvoidSameSpawn(TArray< FSpawnpointInfo*> SpawnPoints);
	void AvoidSpawnReuse(TArray< FSpawnpointInfo*> SpawnPoints);
	void AvoidVisibleEnemies(TArray< FSpawnpointInfo*> SpawnPoints);

	APlayerCharacter* LastAttacker;
	FVector LastDeathPosition;


};
