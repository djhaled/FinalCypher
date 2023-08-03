#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeamComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FINALCYPHER_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTeamComponent();

protected:
	virtual void BeginPlay() override;
public:
	int GetPlayerTeam(APlayerController* NewPlayer);

	TArray<class APlayerCharacter*> GetPlayersInTeam(int32 TeamID) const;

	class AShooterGameMode* ShooterMode;
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		int32 GetAvailableTeam() const;
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		bool IsTeammate(APlayerCharacter* Player1, APlayerCharacter* Player2);


};
