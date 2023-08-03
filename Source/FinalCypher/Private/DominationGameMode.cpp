// Fill out your copyright notice in the Description page of Project Settings.

#include "Net/UnrealNetwork.h" // Include this header for networking functionality
#include "PlayerCharacter.h"
#include "Misc/OutputDeviceNull.h"

#include "DominationGameMode.h"

void ADominationGameMode::BeginPlay()
{
    Super::BeginPlay();
    TeamScoresArray.Init(0, 2);
}

void ADominationGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate ReplicatedValue with a condition (COND_None means always replicate)
	DOREPLIFETIME(ADominationGameMode, ControlPointOwnerTeams)
}

// Function to calculate the score based on control point ownership
void ADominationGameMode::CalculateScore()
{
    TArray<int32> ControlPointCounts; // Array to store the number of control points owned by each team
    int32 NumControlPoints = ControlPointOwnerTeams.Num();
    int32 NumTeams = 2; // Replace this with the number of teams in your game

    // Initialize ControlPointCounts array with zeros for each team
    ControlPointCounts.Init(0, NumTeams);

    // Count the number of control points owned by each team
    for (int32 i = 0; i < NumControlPoints; i++)
    {
        int32 OwnerTeam = ControlPointOwnerTeams[i];
        if (OwnerTeam >= 0 && OwnerTeam < NumTeams)
        {
            ControlPointCounts[OwnerTeam]++;
        }
    }

    // Calculate and update the score for each team
    for (int32 TeamIndex = 0; TeamIndex < NumTeams; TeamIndex++)
    {
        int32 ControlPointsCaptured = ControlPointCounts[TeamIndex];
        int32 PointsToAdd = ControlPointsCaptured; // You can adjust the scoring formula here if needed
        // Add PointsToAdd to the score of the current team
        TeamScoresArray[TeamIndex] += PointsToAdd;
    }
}

int32 ADominationGameMode::GetTeamScore(int32 teamIndex)
{
    return TeamScoresArray[teamIndex];
}

bool ADominationGameMode::UpdateAndCheckWin()
{
    // Variables to store the team scores
    int32 TeamScore = 0;
    int32 EnemyTeamScore = 0;

    // Loop through all player characters to get their team scores
    for (APlayerCharacter* PlayerCharacter : AllPlayerCharacters)
    {
        if (PlayerCharacter)
        {
            int32 PlayerTeamID = PlayerCharacter->GetTeamID();
            TeamScore = GetTeamScore(PlayerTeamID);

            int32 EnemyTeamID = PlayerTeamID == 1 ? 0 : 1; // Calculate the corresponding enemy team ID
            EnemyTeamScore = GetTeamScore(EnemyTeamID);



            // Call the UpdateHUDomination function on the player character to update the HUD
            FString TeamScoreString = FString::Printf(TEXT("%d"), TeamScore);
            FString EnemyTeamScoreString = FString::Printf(TEXT("%d"), EnemyTeamScore);

            // Concatenate the command string with the scores
            FString CmdAndParams = FString::Printf(TEXT("UpdateHUDomination %s %s"), *TeamScoreString, *EnemyTeamScoreString);
            const TCHAR* CmdAndParamsPtr = *CmdAndParams;
            FOutputDeviceNull OutputDeviceNull;
            PlayerCharacter->CallFunctionByNameWithArguments(CmdAndParamsPtr, OutputDeviceNull, nullptr, true);





            // Check if any of the scores are greater than or equal to the TargetScore for victory
            if (TeamScore >= TargetScore || EnemyTeamScore >= TargetScore)
            {
                // Determine the winner team index
                int32 WinnerTeamIndex = TeamScore >= TargetScore ? 1 : 2;

                // Call the HandleVictory function on the player character with the winner team index
                HandleVictory(WinnerTeamIndex);

                // Return true to indicate victory
                return true;
            }
        }
    }

    // Return false if no team has reached the TargetScore
    return false;
}

void ADominationGameMode::HandleVictory(int WinnerIndex)
{
}


