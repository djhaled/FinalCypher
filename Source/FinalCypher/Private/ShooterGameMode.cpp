#include "PlayerCharacter.h"
#include "ShooterPlayerStart.h"
#include "SpawnComponent.h"
#include "TeamComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameMode.h"

void AShooterGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    SpawnPlayer(NewPlayer);
}



void AShooterGameMode::BeginPlay()
{
    // Get all PlayerStarts in the level
    Super::BeginPlay();


}

AShooterGameMode::AShooterGameMode()
{
    // Create and attach the component to the actor
    SpawnComp = CreateDefaultSubobject<USpawnComponent>(TEXT("Spawn Component"));
    TeamComp = CreateDefaultSubobject<UTeamComponent>(TEXT("Team Component"));
}

void AShooterGameMode::SpawnPlayer(APlayerController* NewPlayer)
{
    int TeamID = TeamComp->GetPlayerTeam(NewPlayer);
    FVector SpawnLocation = SpawnComp->PredictNewPlayerSpawn(TeamID);
    APlayerCharacter* SpawnedActor = nullptr;
    // Spawn the new player character at the chosen location
    if (PlayerCharClass)
    {
        //int32 TeamID = SGameMode->TeamComp->GetAvailableTeam();
        SpawnedActor = GetWorld()->SpawnActor<APlayerCharacter>(PlayerCharClass, SpawnLocation, FRotator::ZeroRotator);
        NewPlayer->Possess(SpawnedActor);
        SpawnedActor->SetTeamID(TeamID);
        AllPlayerCharacters.Add(SpawnedActor);
    }
}











