#include "TeamComponent.h"
#include "ShooterGameMode.h"
#include "PlayerCharacter.h"



UTeamComponent::UTeamComponent()
{
}

void UTeamComponent::BeginPlay()
{
    Super::BeginPlay();
    ShooterMode = CastChecked<AShooterGameMode>(GetOwner());
}

int UTeamComponent::GetPlayerTeam(APlayerController* NewPlayer)
{
	return GetAvailableTeam();
}
TArray<class APlayerCharacter*> UTeamComponent::GetPlayersInTeam(int32 TeamID) const
{
    TArray<APlayerCharacter*> PlayersInTeam;
    for (APlayerCharacter* PlayerCharacter : ShooterMode->AllPlayerCharacters)
    {
        if (PlayerCharacter && PlayerCharacter->GetTeamID() == TeamID)
        {
            PlayersInTeam.Add(PlayerCharacter);
        }
    }
    return PlayersInTeam;
}
// Function to predict the team the player will join based on balancing
int32 UTeamComponent::GetAvailableTeam() const
{
    int32 NumPlayersInTeam1 = 0;
    int32 NumPlayersInTeam2 = 0;
    for (APlayerCharacter* PlayerCharacter : ShooterMode->AllPlayerCharacters)
    {
        if (PlayerCharacter)
        {
            if (PlayerCharacter->GetTeamID() == 1)
            {
                NumPlayersInTeam1++;
            }
            else if (PlayerCharacter->GetTeamID() == 2)
            {
                NumPlayersInTeam2++;
            }
        }
    }
    int32 PredictedTeamID = (NumPlayersInTeam1 <= NumPlayersInTeam2) ? 1 : 2;

    return PredictedTeamID;
}

bool UTeamComponent::IsTeammate(APlayerCharacter* Player1, APlayerCharacter* Player2)
{
    
    return Player1->GetTeamID() == Player2->GetTeamID();
}

// Function to predict the team the player will join based on balancing
