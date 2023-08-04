// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"
#include "WeaponBase.h"
#include "WeaponPickupManager.h"


// Sets default values
AWeaponPickup::AWeaponPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the components
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = DefaultSceneRoot;

	Pickup = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup"));
	Pickup->SetupAttachment(DefaultSceneRoot);

	WeaponM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WMesh"));
	WeaponM->SetupAttachment(Pickup);
	
}

// Called when the game starts or when spawned
void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponPickup::UpdatePickup(FPickupData WeaponPickupData)
{
	auto tes2t = NewObject<AWeaponBase>(WeaponPickupData.Pickup);
	auto test = WeaponPickupData.Pickup.GetDefaultObject();
	WeaponM->SetSkeletalMesh(WeaponPickupData.Pickup.GetDefaultObject()->GetWeaponMesh());
	WeaponM->SetVisibility(true);
	Pickup->SetVisibility(true);
}

void AWeaponPickup::OnInteract(AActor* Interactor)
{
}

