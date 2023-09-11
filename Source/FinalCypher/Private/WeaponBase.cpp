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

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::UpdateMesh(USkeletalMesh* Skelmesh)
{
	WeapMesh->SetSkeletalMesh(Skelmesh);
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

