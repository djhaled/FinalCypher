#include "MantleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UMantleComponent::UMantleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMantleComponent::BeginPlay()
{
	Super::BeginPlay();

	// Ensure the character movement component is valid
	UCharacterMovementComponent* CharacterMovement = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	if (!CharacterMovement)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Movement Component not found in owner!"));
	}
}

void UMantleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check for valid mantle surfaces and initiate the mantle action
	if (CanMantle())
	{
		StartMantle();
	}
}

void UMantleComponent::StartMantle()
{
	// Implement your mantle logic here
	UE_LOG(LogTemp, Warning, TEXT("Mantle initiated!"));
}

bool UMantleComponent::CanMantle() const
{
	// Implement your collision detection logic here
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = StartLocation + (GetOwner()->GetActorUpVector() * MantleCheckHeight);

	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(MantleCheckRadius, MantleCheckHeight * 0.5f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, CollisionShape, QueryParams);
	if (bHit)
	{
		for (const FHitResult& HitResult : HitResults)
		{
			// Check if the hit result is a valid mantle surface
			if (HitResult.GetActor())
			{
				// Implement your custom logic to determine if the surface can be mantled
				return true;
			}
		}
	}

	return false;
}