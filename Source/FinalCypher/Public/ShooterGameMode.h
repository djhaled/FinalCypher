// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterPlayerStart.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterGameMode.generated.h"

class APlayerCharacter;
class AShooterPlayerStart;


/**
 * 
 */
UCLASS()
class FINALCYPHER_API AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;


	void SpawnPlayer(APlayerController* NewPlayer);

	AShooterGameMode();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UTeamComponent* TeamComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpawnComponent* SpawnComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Players")
		TSubclassOf<APlayerCharacter> PlayerCharClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Players")
	TArray<APlayerCharacter*> AllPlayerCharacters;
protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (MultiLine = "true"))
		int32 TargetScore;

	
	
};
