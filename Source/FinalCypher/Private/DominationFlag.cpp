// Fill out your copyright notice in the Description page of Project Settings.
#include "DominationFlag.h"
#include "Components/SpotLightComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "PlayerCharacter.h"







// Sets default values
ADominationFlag::ADominationFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = DefaultSceneRoot;
	Cylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flag Mesh"));
	SpotLight = CreateDefaultSubobject< USpotLightComponent>(TEXT("Light Flag"));
	Sphere1 = CreateDefaultSubobject< USphereComponent>(TEXT("Sphere Collision"));
	Cylinder->SetupAttachment(DefaultSceneRoot);
	Sphere1->SetupAttachment(Cylinder);
	SpotLight->SetupAttachment(Cylinder);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Flag Widget"));
	WidgetComponent->SetupAttachment(DefaultSceneRoot);



}

// Called when the game starts or when spawned
void ADominationFlag::BeginPlay()
{
	Super::BeginPlay();
	FlagMaterial = Cylinder.Get()->CreateAndSetMaterialInstanceDynamic(0);

}



