#include "WeaponPickup.h"
#include "WeaponPickupManager.h"
#include "Net/UnrealNetwork.h"

AWeaponPickup::AWeaponPickup()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = DefaultSceneRoot;

	Pickup = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup"));
	Pickup->SetupAttachment(DefaultSceneRoot);

	WeaponM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WMesh"));
	WeaponM->SetupAttachment(Pickup);

	bReplicates = true;
}

void AWeaponPickup::UpdatePickup(FPickupData NewWeaponPickupData)
{
	if (HasAuthority())
	{
		// Call the multicast function to update clients
		Multicast_UpdatePickup(NewWeaponPickupData);
	}
	else
	{
		// If not on the server, call the server function to update the weapon pickup data
		Server_UpdatePickup(NewWeaponPickupData);
	}
}

void AWeaponPickup::Server_UpdatePickup_Implementation(FPickupData NewWeaponPickupData)
{
	// This function is executed on the server when called from a client
	UpdatePickup(NewWeaponPickupData);
}



void AWeaponPickup::Multicast_UpdatePickup_Implementation(FPickupData NewWeaponPickupData)
{
	// This function is executed on all clients and the server when called from the server
	
	WeaponM->SetSkeletalMesh(NewWeaponPickupData.PickupMesh);
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMesh Set on Server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMesh Set on Client"));
	}
}



