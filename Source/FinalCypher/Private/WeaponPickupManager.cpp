// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickupManager.h"
#include "WeaponBase.h"
#include "WeaponPickup.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UWeaponPickupManager::UWeaponPickupManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponPickupManager::BeginPlay()
{
	Super::BeginPlay();
	/*InitDataToStruct();
	InitializeEmptyPickups();
	SpawnInitialPickups();*/

	// ...
	
}

void UWeaponPickupManager::SpawnInitialPickups()
{
	for (size_t i = 0; i < WeaponPickups.Num(); i++)
	{
		FPickupData AvailablePickup = GetAvailableWeapon();
		SpawnPickup(GetAvailableWeapon());
	}
}

void UWeaponPickupManager::SpawnPickup(FPickupData PickupData)
{
	// If the actor does not exist, spawn a new WeaponPickup actor
	if (PickupData.Pickup)
	{
		FTransform SpawnTransform; // You can set the transform for the spawn location
		
		// Check if the spawning was successful
		if (AWeaponPickup* NewPickup = GetAvailablePickup())
		{
			// Set the PickupData for the new actor
			NewPickup->UpdatePik_TEST(PickupData);
		}
	} 
}


FPickupData UWeaponPickupManager::GetAvailableWeapon()
{
	TArray<FPickupData> AvailableWeights;
	for (const FPickupData Weapon : AvailableWeapons)
	{
		bool bAlreadySpawned = false;
		for (AWeaponPickup* Pickup : WeaponPickups)
		{
			if (Pickup == Weapon.CurrentPickup)
			{
				bAlreadySpawned = true;
				break;
			}
		}

		if (!bAlreadySpawned)
		{
			AvailableWeights.Add(Weapon);
		}
	}

	// Calculate the total weight of all available weapons
	float TotalWeight = 0.0f;
	for (const FPickupData& WeightedWeapon : AvailableWeights)
	{
		TotalWeight += WeightedWeapon.Weight;
	}

	// Choose a random weight value within the total weight range
	float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);

	// Select the weapon based on the random weight
	FPickupData ChosenWeapon;
	for (const FPickupData& WeightedWeapon : AvailableWeights)
	{
		if (RandomWeight <= WeightedWeapon.Weight)
		{
			ChosenWeapon = WeightedWeapon;
			break;
		}
		RandomWeight -= WeightedWeapon.Weight;
	}

	return ChosenWeapon;
}

AWeaponPickup* UWeaponPickupManager::GetAvailablePickup()
{
	for (AWeaponPickup* Pickup : WeaponPickups)
	{
		if (Pickup->WeaponPickupActor == nullptr)
		{
			return Pickup;
		}
	}
	return NULL;
}


void UWeaponPickupManager::InitDataToStruct()
{
	UDataTable* DataWeapons = WeaponsDataTable.Get();
	TArray<FSInformation> WeaponDataRows;
	FString ContextString;
	auto RowNames = DataWeapons->GetRowNames();
	for ( auto& name : RowNames )
	{
		FPickupData PickData =  FPickupData();
		FSInventorySlot* row = DataWeapons->FindRow<FSInventorySlot>(name, ContextString);
		if ( row )
		{
			PickData.Weight = row->Weight;
			PickData.Pickup = row->Item;
			PickData.PickupMesh = row->SkelMesh;
			
			AvailableWeapons.Add(PickData);
		}
	}

}

void UWeaponPickupManager::InitializeEmptyPickups()
{
	TArray<AActor*> ActorsPickups;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeaponPickup::StaticClass(), ActorsPickups);
	for (auto pkAc : ActorsPickups)
	{
		AWeaponPickup* WePickup = Cast<AWeaponPickup>(pkAc);
		WeaponPickups.Add(WePickup);
	}
}


// Called every frame


