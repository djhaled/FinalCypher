// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerStart.h"

void AShooterPlayerStart::BeginPlay()
{
	SpawnData = new FSpawnpointInfo();
	SpawnData->SpawnTransform = GetActorTransform();
	SpawnData->NearbyPlayers.AddDefaulted(3);
	SpawnData->SightTracePoint = GetActorLocation() + FVector(0, 0, 1) * 50;
	SpawnData->EnemySights.AddDefaulted(3);
	SpawnData->MinEnemyDist.AddDefaulted(3);
	SpawnData->DistSum.AddDefaulted(3);
	SpawnData->EnemyDistSum.AddDefaulted(3);
	SpawnData->bIsSpawned = true;

}
