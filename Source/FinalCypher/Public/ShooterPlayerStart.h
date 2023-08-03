// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "ShooterPlayerStart.generated.h"

class APlayerCharacter;


UENUM()
enum TeamStarts
{
	TeamA     UMETA(DisplayName = "TeamA"),
	TeamB      UMETA(DisplayName = "TeamB")
};
/**
 * 
 */

USTRUCT(BlueprintType)
struct FSpawnpointInfo
{
	GENERATED_BODY()

		// Add members for spawn point information
		UPROPERTY(BlueprintReadWrite)
		float Weight = 0;


	UPROPERTY(BlueprintReadWrite)
		int32 NumPlayersAtLastUpdate;
	UPROPERTY()
		FVector SightTracePoint;
	UPROPERTY()
		double LastSightTraceTime;
	UPROPERTY()
		TArray<float> DistSum; // Assuming an array to store distance sums for two teams

	UPROPERTY()
		TArray<float> EnemyDistSum; // Assuming an array to store enemy distance sums for two teams

	UPROPERTY()
		TArray<float> MinEnemyDist; // Assuming an array to store enemy distance sums for two teams
	UPROPERTY()
		FTransform SpawnTransform;

		//TMap<int32, TArray<APlayerCharacter*>> NearbyPlayers; // Assuming an array to store enemy distance sums for two teams]
	TArray<TArray<APlayerCharacter*>> NearbyPlayers;
	UPROPERTY()
		TArray<float>  EnemySights;

	bool bIsSpawned;
	APlayerCharacter* LastSpawnedPlayer;
	float LastSpawnTime;
};
UCLASS()
class FINALCYPHER_API AShooterPlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TeamSelect)
		TEnumAsByte<TeamStarts> TeamStart;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TeamSelect)
		bool bUseForInitialSpawn;

		FSpawnpointInfo* SpawnData;

		virtual void BeginPlay() override;
	
};
