// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"




// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeapMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeapMesh"));
	RootComponent = WeapMesh;
}

void AWeaponBase::PostLoad()
{
	Super::PostLoad();

	// Check if we have an old "Weapon" SkeletalMeshComponent and redirect it to "WeapMesh"
	if (USkeletalMeshComponent* OldWeaponMesh = FindComponentByClass<USkeletalMeshComponent>())
	{
		if (OldWeaponMesh->GetName() == "WeapMesh") {return;}
		USkeletalMeshComponent* NewWeapMesh = WeapMesh;
		if (NewWeapMesh)
		{
			NewWeapMesh->SetSkeletalMesh(OldWeaponMesh->SkeletalMesh);
			TArray<UMaterialInterface*> MatInts = OldWeaponMesh->GetMaterials();
			for (size_t i = 0; i < MatInts.Num(); i++)
			{
				NewWeapMesh->SetMaterial(i,MatInts[i]);
			}
			
			//NewWeapMesh = OldWeaponMesh;
			// Add any other property redirections or configurations here
			// For example, if you have additional socket attachments or other settings,
			// copy them over as well.
		}

		OldWeaponMesh->DestroyComponent(); // Optionally destroy the old component if needed
	}
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

USkeletalMesh* AWeaponBase::GetWeaponMesh()
{
	return WeapMesh->SkeletalMesh.Get();
}

