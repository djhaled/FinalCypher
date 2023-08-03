// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "ShooterGameMode.h"
#include "TeamComponent.h"
#include "SpawnComponent.h"

// Sets default values for this component's properties
USpawnComponent::USpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnComponent::BeginPlay()
{
	Super::BeginPlay();
    InitializePlayerStarts();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateSpawnPoints, this, &USpawnComponent::SpawnPerFrameUpdate, .5f, true);
    ShooterMode = CastChecked<AShooterGameMode>(GetOwner());
	// ...
	
}



// Called every frame
void USpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Helper function to check if a PlayerStart is already occupied by a player
bool USpawnComponent::IsPlayerStartOccupied(AShooterPlayerStart* PlayerStart, const TArray<APlayerCharacter*>& PlayersInTeam) const
{
    if (!PlayerStart)
    {
        return true;
    }

    FVector PlayerStartLocation = PlayerStart->GetActorLocation();

    for (APlayerCharacter* PlayerCharacter : PlayersInTeam)
    {
        if (PlayerCharacter && PlayerCharacter->GetActorLocation().Equals(PlayerStartLocation, 50.0f)) // You can adjust the tolerance (50.0f) based on your level design
        {
            return true;
        }
    }

    return false;
}
FVector USpawnComponent::PredictNewPlayerSpawn(int32 TeamID)
{
    FVector SpawnLocation = FVector::OneVector;

    TArray<APlayerCharacter*> PlayersInTeam = ShooterMode->TeamComp->GetPlayersInTeam(TeamID);

    TArray<AShooterPlayerStart*> TeamStarts = (TeamID == 1) ? TeamAStarts : TeamBStarts;

    for (AShooterPlayerStart* Start : TeamStarts)
    {
        if (!IsPlayerStartOccupied(Start, PlayersInTeam))
        {
            SpawnLocation = Start->GetActorLocation();
            break;
        }
    }
    return SpawnLocation;
}

void USpawnComponent::InitializePlayerStarts()
{
    // Get all PlayerStarts in the level
    SpawnPoints.Reserve(RespawnStarts.Num());
    TArray<AActor*> AllStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterPlayerStart::StaticClass(), AllStarts);
    for (size_t i = 0; i < AllStarts.Num(); i++)
    {
        AShooterPlayerStart* SPS = Cast<AShooterPlayerStart>(AllStarts[i]);
        if (!SPS->bUseForInitialSpawn) {
            RespawnStarts.Add(SPS);
            SpawnPoints.Add(SPS->SpawnData);
            continue;
        }
        switch (SPS->TeamStart)
        {
        case TeamStarts::TeamA:
            TeamAStarts.Add(SPS);
            break;
        case TeamStarts::TeamB:
            TeamBStarts.Add(SPS);
            break;
        default:
            // Handle other cases (if applicable)
            break;
        }
    }


}

FSpawnpointInfo* USpawnComponent::GetSpawnpointNearTeam(APlayerCharacter* PCSummoned)
{
    if (SpawnPoints.Num() == 0)
    {
        return nullptr; // Return an empty FSpawnpointInfo (assuming this struct holds the spawn point information)
    }


    // Initialize variables
    float FavoredSpawnpointBonus = 25000.0f;
    int32 NumPlayers = ShooterMode->AllPlayerCharacters.Num();
    float AlliedDistanceWeight = 2.0f;
    int32 MyTeam = PCSummoned->GetTeamID(); // Assuming you have a variable 'self.team' that holds the team ID of the current player
    // Reduce Weight considerably if only player in team.
    if (ShooterMode->TeamComp->GetPlayersInTeam(MyTeam).Num() == 1){AlliedDistanceWeight = -1;}

    for (int32 i = 0; i < SpawnPoints.Num(); i++)
    {
        FSpawnpointInfo*& Spawnpoint = SpawnPoints[i];
        if (!Spawnpoint->NumPlayersAtLastUpdate)
        {
            Spawnpoint->NumPlayersAtLastUpdate = 0;
        }
        if (Spawnpoint->NumPlayersAtLastUpdate > 0)
        {
            float AllyDistSum = Spawnpoint->DistSum[MyTeam];
            float EnemyDistSum = Spawnpoint->EnemyDistSum[MyTeam];
            float AlliedWeightedDist = AlliedDistanceWeight * AllyDistSum;
            float WeightWithoutPlayerNum = EnemyDistSum - AlliedWeightedDist;
            float Weight = WeightWithoutPlayerNum / Spawnpoint->NumPlayersAtLastUpdate;
            Spawnpoint->Weight = Weight;
            // You can ignore the part related to spawndata storage since Unreal Engine might handle things differently
            continue;
        }
        Spawnpoint->Weight = 0.0f;
        // You can ignore the part related to spawndata storage since Unreal Engine might handle things differently
    }



    // Assuming these functions are custom functions you implement based on your game's needs
    PCSummoned->AvoidSameSpawn(SpawnPoints);
    PCSummoned->AvoidSpawnReuse(SpawnPoints);
    //AvoidWeaponDamage(Spawnpoints); too advanced for now

    // making weights get fucked
    PCSummoned->AvoidVisibleEnemies(SpawnPoints);

    // Assuming there's a function named GetSpawnpointFinal that calculates and returns the best spawn point from the array
    FSpawnpointInfo* Result = GetSpawnpointFinal(PCSummoned);

    return Result;
    //return Result;
}

FVector USpawnComponent::CalculateOptimalRespawn(APlayerCharacter* PlayerToRespawn)
{
    float BestScore = 9999.f;
    FVector BestRespawnLocation;

    for (auto RespawnStart : RespawnStarts)
    {
        float Score = 0.0f;
        int otherTeam = PlayerToRespawn->GetTeamID() ? 2 : 1;
        for (APlayerCharacter* OtherPlayer : ShooterMode->TeamComp->GetPlayersInTeam(otherTeam))
        {
            if (OtherPlayer == PlayerToRespawn || OtherPlayer->GetTeamID() == PlayerToRespawn->GetTeamID())
            {
                continue; // Skip the same team or the player being respawned 
            }

            // Calculate distance and line of sight score
            float Distance = FVector::DistSquared(OtherPlayer->GetActorLocation(), RespawnStart->GetActorLocation());
            Score += Distance;

            if (OtherPlayer->GetLocalViewingPlayerController()->LineOfSightTo(RespawnStart))
            {
                Score /= 10.f; // Apply a penalty if there's line of sight
            }
        }

        // Check if the current score is better than the previous best score
        if (Score < BestScore)
        {
            BestScore = Score;
            BestRespawnLocation = RespawnStart->GetActorLocation();
        }
    }

    return BestRespawnLocation;
}

void USpawnComponent::RespawnPlayer_DEBUG(APlayerCharacter* PlayerToRespawn)
{
    FSpawnpointInfo* SpawnPoint = GetSpawnpointNearTeam(PlayerToRespawn);
    PlayerToRespawn->SetActorTransform(SpawnPoint->SpawnTransform);
    SpawnPoint->LastSpawnedPlayer = PlayerToRespawn;
    SpawnPoint->LastSpawnTime = GetWorld()->GetTimeSeconds();
}
bool USpawnComponent::PerformLineTrace(FHitResult& OutHitResult, APlayerCharacter* player, FSpawnpointInfo* spawnpoint)
{
    FVector StartTrace = player->GetActorLocation() + FVector(0, 0, 50);
    FVector EndTrace = spawnpoint->SightTracePoint;
    FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, player);
    TraceParams.bTraceComplex = true;
    TraceParams.bReturnPhysicalMaterial = false;

    // You can adjust the collision channel based on your game's needs
    ECollisionChannel CollisionChannel = ECC_Visibility; // Example: Collide with pawns

    // Perform the line trace
    return GetWorld()->LineTraceSingleByChannel(OutHitResult, StartTrace, EndTrace, CollisionChannel, TraceParams);
}
void USpawnComponent::UpdateSpawnPoint(FSpawnpointInfo* SpawnPoint)
{
    // Arrays to store data for each team (assuming 3 teams: 0, 1, and 2)
    TArray<int32> Sights;        // Number of enemy players seen by each team
    Sights.AddZeroed(3);
    TArray<int32> MinDist;  // Minimum distance to enemy player for each team
    MinDist.AddDefaulted(3);
    for (int TIndex = 1; TIndex <= 2; ++TIndex)
    {
        SpawnPoint->DistSum[TIndex] = 0;
        SpawnPoint->EnemyDistSum[TIndex] = 0;
        SpawnPoint->MinEnemyDist[TIndex] = 9999999;
        MinDist[TIndex] = 9999999;
    }
    TArray<int32> DistSum;        // Total distance to all players for each team
    DistSum.AddZeroed(3);

    // Get the forward direction of the spawn point
    FVector SpawnPointDir = SpawnPoint->SpawnTransform.GetLocation().ForwardVector;

    // Initialize the number of players seen at the last update
    SpawnPoint->NumPlayersAtLastUpdate = 0;

    // Loop through all player characters
    for (APlayerCharacter* Player : ShooterMode->AllPlayerCharacters)
    {
        // Calculate the distance between the player and the spawn point
        FVector Diff = Player->GetActorLocation() - SpawnPoint->SpawnTransform.GetLocation();
        Diff.Z = 0;
        float Dist = Diff.Size();
        int Team = Player->GetTeamID();

        //// Check if the player is within a certain distance (1024 units) of the spawn point
        //FString DebugMessage = FString::Printf(TEXT("Player: %s, Team: %d, Dist: %.2f"), *Player->GetName(), Team, Dist);
        //FColor DebugColor = FColor::Red; // Set the color for the debug message (you can change this if desired)
        //float DisplayTime = .5f; // How long (in seconds) the message should be displayed on the screen
        //GEngine->AddOnScreenDebugMessage(-1, DisplayTime, DebugColor, DebugMessage);
        if (Dist < 4096 )
        {
            // Add the player to the nearby players list for their team
            SpawnPoint->NearbyPlayers[Team].AddUnique(Player);
        }

        // Update the minimum distance to an enemy player for the current team
        if (Dist < MinDist[Team])
        {
            MinDist[Team] = Dist;
        }

        // Update the total distance to all players for the current team
        DistSum[Team] += Dist;
        SpawnPoint->NumPlayersAtLastUpdate++;

        // Calculate the dot product between the spawn point direction and the player's position relative to the spawn point
        FVector PDir = Player->GetActorForwardVector();
        float dotProductSpawn = FVector::DotProduct(SpawnPointDir, Diff);
        float dotProductPlayer = FVector::DotProduct(PDir, Diff);

        // Check if the player is facing away from the spawn point and the spawn point is facing away from the player
        // If so, skip the LOS (Line of Sight) check for this player
        if (dotProductSpawn < 0 && dotProductPlayer > 0)
        {
            continue;
        }

        // Perform a line trace to check if there is line of sight between the player and the spawn point
        FHitResult HitResult;
        bool LosExists = PerformLineTrace(HitResult, Player, SpawnPoint);
        SpawnPoint->LastSightTraceTime = GetWorld()->GetTimeSeconds();

        // If there is line of sight (LosExists is true), increment the sight count for the player's team
        if (LosExists)
        {
            Sights[Team]++;
        }
    }

    // Update the spawn point's data with the calculated values for each team
    for (int TeamIndex = 1; TeamIndex <= 2; ++TeamIndex)
    {
        // Calculate the total number of enemy players seen by the current team (excluding players from their own team)
        SpawnPoint->EnemySights[TeamIndex] = GetNonTeamSum(TeamIndex, Sights);

        // Find the minimum distance to an enemy player for the current team
        SpawnPoint->MinEnemyDist[TeamIndex] = GetNonTeamMinDist(TeamIndex, MinDist);

        // Update the total distance to all players for the current team
        SpawnPoint->DistSum[TeamIndex] = DistSum[TeamIndex];

        // Calculate the total distance to all enemy players for the current team (excluding players from their own team)
        SpawnPoint->EnemyDistSum[TeamIndex] = GetNonTeamSum(TeamIndex, DistSum);
    }
}


int32 USpawnComponent::GetNonTeamSum(int32 SkipTeam, TArray<int32> Sums)
{
    int32 Value = 0;
    for (int i = 1; i <= 2; ++i)
    {
        if (!Sums.IsValidIndex(i)) { continue; }
        if (i == SkipTeam)
        {
            continue;
        }
        Value += Sums[i];
    }
    return Value;
}

int32 USpawnComponent::GetNonTeamMinDist(int32 SkipTeam, TArray<int32> MinDists)
{
    int32 Dist = 9999999;
    for (int i = 1; i <= 2; ++i)
    {
        if (i == SkipTeam)
        {
            continue;
        }
        if (Dist > MinDists[i])
        {
            Dist = MinDists[i];
        }
    }
    return Dist;
}

void USpawnComponent::SpawnPerFrameUpdate()
{
    if (RespawnStarts.Num() == 0)
        return;

    // Increment spawn point index and wrap around the array
    SpawnPointIndex = (SpawnPointIndex + 1) % RespawnStarts.Num();

    //Debug
    // 
    FString Message = FString::Printf(TEXT("SpawnPoints[0] - MinEnemyDist: %f, MinEnemyDist2: %f"), SpawnPoints[0]->MinEnemyDist[1], SpawnPoints[0]->MinEnemyDist[2]);
    // Display the message on the screen.
    FColor Color = FColor::Green; // You can change the color if needed.
    float DisplayTime = 2.5f; // Time in seconds to display the message on the screen.
    //GEngine->AddOnScreenDebugMessage(-1, DisplayTime, Color, Message);
    // Get the current spawn point
    AShooterPlayerStart* SpawnPoint = RespawnStarts[SpawnPointIndex];
    UpdateSpawnPoint(SpawnPoints[SpawnPointIndex]);
    SpawnPoint->SpawnData = SpawnPoints[SpawnPointIndex];

}
FSpawnpointInfo* USpawnComponent::GetSpawnpointFinal(APlayerCharacter* Player)
{
    FSpawnpointInfo* BestSpawnpoint = nullptr;

    if (SpawnPoints.Num() == 0)
    {
        return BestSpawnpoint;
    }

    BestSpawnpoint = GetBestWeightedSpawnpoint(Player);

    return BestSpawnpoint;
}
FSpawnpointInfo* USpawnComponent::GetBestWeightedSpawnpoint(APlayerCharacter* Playa)
{
    int32 MaxSightTracedSpawnPoints = 3;
    float BestWeight = -FLT_MAX; // Initialize BestWeight to a very small negative value

    FSpawnpointInfo* BestSpawnPoint = nullptr;
    TArray<FSpawnpointInfo*> BestSpawnPoints;

    for (int32 Try = 0; Try < MaxSightTracedSpawnPoints; Try++)
    {
        BestSpawnPoints.Empty();
        BestWeight = -FLT_MAX;

        for (int32 i = 0; i < SpawnPoints.Num(); i++)
        {
            if (SpawnPoints[i]->Weight > BestWeight)
            {
                BestSpawnPoints.Empty();
                BestSpawnPoints.Add(SpawnPoints[i]);
                BestWeight = SpawnPoints[i]->Weight;
            }
            else if (SpawnPoints[i]->Weight == BestWeight)
            {
                BestSpawnPoints.Add(SpawnPoints[i]);
            }
        }

        if (BestSpawnPoints.Num() == 0)
        {
            return nullptr;
        }

        // Check if the array is not empty before selecting a random element
        int32 RandomIndex = FMath::RandRange(0, BestSpawnPoints.Num() - 1);
        BestSpawnPoint = BestSpawnPoints[RandomIndex];

        if (BestSpawnPoint->LastSightTraceTime == GetWorld()->GetTimeSeconds())
        {
            return BestSpawnPoint;
        }

        if (!LastMinuteSightTraces(BestSpawnPoint, Playa))
        {
            return BestSpawnPoint;
        }

        float Penalty = 100000;
        BestSpawnPoint->Weight -= Penalty; // Apply the penalty to the selected spawn point
        BestSpawnPoint->LastSightTraceTime = GetWorld()->GetTimeSeconds();
    }

    return nullptr;
}

bool USpawnComponent::LastMinuteSightTraces(FSpawnpointInfo* spawnpoint, APlayerCharacter* Player)
{
    if (spawnpoint->NearbyPlayers.Num() == 0)
    {
        return false;
    }

    APlayerCharacter* closest = nullptr;
    float closestdistsq = 0.0f;
    APlayerCharacter* secondclosest = nullptr;
    float secondclosestdistsq = 0.0f;
    int lodebug = -1;
    for (auto nearbyplayer : spawnpoint->NearbyPlayers)
    {
        lodebug++;
        if (lodebug == Player->GetTeamID())
        {
            continue;
        }
        if (nearbyplayer.IsEmpty()) { continue; }

        for (int32 i = 0; i < nearbyplayer.Num(); i++)
        {
            APlayerCharacter* player = nearbyplayer[i];
            if (!player)
            {
                continue;
            }

            float distsq = FVector::DistSquared(spawnpoint->SpawnTransform.GetLocation(), player->GetActorLocation());
            if (!closest || distsq < closestdistsq)
            {
                secondclosest = closest;
                secondclosestdistsq = closestdistsq;
                closest = player;
                closestdistsq = distsq;
                continue;
            }
            if (!secondclosest || distsq < secondclosestdistsq)
            {
                secondclosest = player;
                secondclosestdistsq = distsq;
            }
        }
    }
    if (closest)
    {
        if(LineTraceSpawn(closest->GetActorLocation() + FVector(0, 0, 50), spawnpoint->SightTracePoint))
        //if (GetWorld()->LineTraceSingleByChannel(OutHit, closest->GetActorLocation() + FVector(0, 0, 50), spawnpoint->SightTracePoint, ECollisionChannel::ECC_Visibility))
        {
            return true;
        }
    }

    if (secondclosest)
    {
        if (LineTraceSpawn(secondclosest->GetActorLocation() + FVector(0, 0, 50), spawnpoint->SightTracePoint))
        //if (GetWorld()->LineTraceSingleByChannel(OutHit, secondclosest->GetActorLocation() + FVector(0, 0, 50), spawnpoint->SightTracePoint, ECollisionChannel::ECC_Visibility))
        {
            return true;
        }
    }

    return false;
}
bool USpawnComponent::LineTraceSpawn(FVector Start, FVector End)
{
    FHitResult OutHit;
    bool trace = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility);
   /* DrawDebugLine(
        GetWorld(),
        Start,
        End,
        FColor::Black,
        false, -1.0f, 0,
        5.0f
    );*/

    return trace;
}
void APlayerCharacter::AvoidSameSpawn(TArray<FSpawnpointInfo*> SpawnPoints)
{
    if (LastSpawnPoint != FVector::ZeroVector) { return; }

    for (size_t i = 0; i < SpawnPoints.Num(); i++)
    {
        if (SpawnPoints[i]->SpawnTransform.GetLocation() == LastSpawnPoint)
        {
            SpawnPoints[i]->Weight -= 50000;
            break;
        }
    }
}

void APlayerCharacter::AvoidSpawnReuse(TArray<FSpawnpointInfo*> SpawnPoints)
{
    float Time = GetWorld()->GetTimeSeconds();
    const float MaxTime = 10000.0f;
    const float MaxDistSq = 1048576.0f;

    for (FSpawnpointInfo*& SpawnPoint : SpawnPoints)
    {

        if (SpawnPoint->LastSpawnedPlayer == this)
        {
            continue;
        }

        if (SpawnPoint->LastSpawnedPlayer->GetTeamID() == this->GetTeamID())
        {
            continue;
        }

        float TimePassed = Time - SpawnPoint->LastSpawnTime;
        if (TimePassed < MaxTime)
        {
            FVector SpawnedPlayerOrigin = SpawnPoint->LastSpawnedPlayer->GetActorLocation();
            FVector SpawnPointOrigin = SpawnPoint->SpawnTransform.GetLocation();
            float DistSq = FVector::DistSquared(SpawnedPlayerOrigin, SpawnPointOrigin);
            if (DistSq < MaxDistSq)
            {
                float Worsen = (5000.0f * (1.0f - (DistSq / MaxDistSq))) * (1.0f - (TimePassed / MaxTime));
                SpawnPoint->Weight -= Worsen;
            }
            else
            {
                SpawnPoint->LastSpawnedPlayer = nullptr;
            }
        }
        else
        {
            SpawnPoint->LastSpawnedPlayer = nullptr;
        }
    }
}
void APlayerCharacter::AvoidVisibleEnemies(TArray<FSpawnpointInfo*> SpawnPoints)
{
    float LosPenalty = 100000;
    uint8 MinDistTeam = GetTeamID();

    for (int32 i = 0; i < SpawnPoints.Num(); i++)
    {
        if (!SpawnPoints[i]->EnemySights.IsEmpty())
        {
            continue;
        }

        float Penalty = LosPenalty * SpawnPoints[i]->EnemySights[MinDistTeam];
        SpawnPoints[i]->Weight -= Penalty;
    }

    float AvoidWeight = 2.f;
    if (AvoidWeight != 0.f)
    {
        float NearbyEnemyOuterRange = 800;
        float NearbyEnemyOuterRangeSq = NearbyEnemyOuterRange * NearbyEnemyOuterRange;
        float NearbyEnemyPenalty = 1500.f * AvoidWeight;
        float NearbyEnemyMinorPenalty = 800.f * AvoidWeight;


        for (int32 i = 0; i < SpawnPoints.Num(); i++)
        {
            float MinDist = SpawnPoints[i]->MinEnemyDist[MinDistTeam];
            if (MinDist < (NearbyEnemyOuterRange * 2))
            {
                float Penalty = NearbyEnemyMinorPenalty * (1.f - (MinDist / (NearbyEnemyOuterRange * 2)));
                if (MinDist < NearbyEnemyOuterRange)
                {
                    Penalty += NearbyEnemyPenalty * (1.f - (MinDist / NearbyEnemyOuterRange));
                }
                if (Penalty > 0.f)
                {
                    SpawnPoints[i]->Weight -= Penalty;
                }
            }
        }
    }
}