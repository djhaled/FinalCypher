// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameMode.h"
#include "DominationGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FINALCYPHER_API ADominationGameMode : public AShooterGameMode
{
	GENERATED_BODY()

protected:
	/** Please add a variable description */
	virtual void BeginPlay();
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", Replicated, meta = (MultiLine = "true"))
		TArray<int32> ControlPointOwnerTeams;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", Replicated, meta = (MultiLine = "true"))
		TArray<int32> TeamScoresArray;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	UFUNCTION(BlueprintCallable)
	void CalculateScore();
	UFUNCTION(BlueprintCallable)
		int32 GetTeamScore(int32 teamIndex);
	UFUNCTION(BlueprintCallable)
		bool UpdateAndCheckWin();
	UFUNCTION(BlueprintCallable)
		void HandleVictory(int WinnerIndex);
};
