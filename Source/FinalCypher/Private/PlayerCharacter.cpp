// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "HealthComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

uint8 APlayerCharacter::GetTeamID() const
{
	return TeamID;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void APlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the TeamID property to any relevant client
	DOREPLIFETIME(APlayerCharacter, TeamID);
}
uint8 APlayerCharacter::SetTeamID(uint8 tID)
{
	if (HasAuthority()) // Check if this is the server
	{
		// Set the TeamID and replicate it to all clients
		TeamID = tID;
	}
	return TeamID;
}

bool APlayerCharacter::IsTeammate(APlayerCharacter* ComparedPlayer)
{
	return ComparedPlayer->GetTeamID() == GetTeamID();
}

