// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterPlayerStart.h"
#include "Components/ActorComponent.h"
#include "SpawnComponent.generated.h"

class AShooterGameMode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALCYPHER_API USpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Predict the new player spawn location based on the TeamID
	FVector PredictNewPlayerSpawn(int32 TeamID);

	// Pointer to the ShooterGameMode instance
	AShooterGameMode* ShooterMode;

	// Timer handle for updating spawn points
	FTimerHandle TimerHandle_UpdateSpawnPoints;

	// Array of player starts for Team A
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnLogic")
		TArray<class AShooterPlayerStart*> TeamAStarts;

	// Array of player starts for Team B
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnLogic")
		TArray<class AShooterPlayerStart*> TeamBStarts;

	// Array of player starts for respawning
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnLogic")
		TArray<class AShooterPlayerStart*> RespawnStarts;

	// Respawns
	TArray<FSpawnpointInfo*> SpawnPoints;

	// Index of the current spawn point
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnLogic")
		int32 SpawnPointIndex;

	bool LineTraceSpawn(FVector Start, FVector End);
	// Initialize player starts
	UFUNCTION(BlueprintCallable, Category = "SpawnLogic")
		void InitializePlayerStarts();

	// Get the nearest spawn point to the team
		FSpawnpointInfo* GetSpawnpointNearTeam(class APlayerCharacter* PCSummoned);

	// Calculate the optimal respawn location for the player
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		FVector CalculateOptimalRespawn(class APlayerCharacter* PlayerToRespawn);

	// Respawn the player for debugging purposes
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		void RespawnPlayer_DEBUG(class APlayerCharacter* PlayerToRespawn);

	// Perform a line trace to check if a spawn point is clear
	bool PerformLineTrace(FHitResult& OutHitResult, class APlayerCharacter* Player, FSpawnpointInfo* Spawnpoint);

	// Update the spawn point based on certain criteria
	void UpdateSpawnPoint(FSpawnpointInfo* SpawnPoint);

	// Get the sum of non-team members
	UFUNCTION(BlueprintCallable, Category = "SpawnLogic")
		static int32 GetNonTeamSum(int32 SkipTeam, TArray<int32> Sums);

	// Get the minimum distance of non-team members
	UFUNCTION(BlueprintCallable, Category = "SpawnLogic")
		static int32 GetNonTeamMinDist(int32 SkipTeam, TArray<int32> MinDists);

	// Spawn logic per frame update
	void SpawnPerFrameUpdate();

	// Get the final spawn point based on certain criteria
	FSpawnpointInfo* GetSpawnpointFinal(class APlayerCharacter* Player);

	// Get the best weighted spawn point for the player
	FSpawnpointInfo* GetBestWeightedSpawnpoint(class APlayerCharacter* Player);

	// Perform sight traces for last-minute spawn point decisions
	bool LastMinuteSightTraces(FSpawnpointInfo* Spawnpoint, class APlayerCharacter* Player);

	// Check if a player start is occupied by any player in the team
	bool IsPlayerStartOccupied(AShooterPlayerStart* PlayerStart, const TArray<class APlayerCharacter*>& PlayersInTeam) const;

};