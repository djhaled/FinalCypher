// Fill out your copyright notice in the Description page of Project Settings.
#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UHealthComponent::GetHealth()
{
	return Health;
}

float UHealthComponent::GetMaximumHealth()
{
	return MaximumHealth;
}

float UHealthComponent::GetNormalizedHealth()
{
	return Health / MaximumHealth;
}

void UHealthComponent::SetHealth(float NewHealth)
{
	Health = NewHealth;
	OnHealthChanged.Broadcast(Health, MaximumHealth);
}
void UHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the Health and bAlive variables
	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, bAlive);
}
bool UHealthComponent::CheckDeath(float Damage, EAresRegionalDamage RegionDMG, EAresDirectionalDamage DirectionDMG)
{
	if (Health - Damage <= 0.f)
	{
		// Die
		bAlive = false;
		OnDeath.Broadcast();
		return true;
	}
	return false;
}

void UHealthComponent::SetMaximumHealth(float NewMaximumHealth)
{
	MaximumHealth = NewMaximumHealth;
}



float UHealthComponent::ApplyDamage(AController* EventInstigator, AActor* DamageCauser, EAresRegionalDamage RegionalDamage, EAresDirectionalDamage Direction, float Damage)
{
	if (!bAlive){return 0.f;}

	OnDamageReceived.Broadcast(Damage);

	if (!CheckDeath(Damage, RegionalDamage, Direction))
	{
		SetHealth(Health - Damage);
		return Damage;
	}
	SetHealth(Health - Damage);
	return Damage;


}

