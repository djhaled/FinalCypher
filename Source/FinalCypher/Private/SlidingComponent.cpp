#include "SlidingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

USlidingComponent::USlidingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USlidingComponent::BeginPlay()
{
	Super::BeginPlay();

	// Ensure the character movement component is valid
	UCharacterMovementComponent* CharacterMovement = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	if (!CharacterMovement)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Movement Component not found in owner!"));
	}
}

void USlidingComponent::Slide()
{
	// Apply sliding acceleration
	SlideSpeed += SlideAcceleration;
	float SlideDuration = 3.0f;
	// Clamp sliding speed to the maximum slide speed
	SlideSpeed = FMath::Min(SlideSpeed, MaxSlideSpeed);

	// Apply friction to sliding speed
	SlideSpeed -= SlideFriction;

	if (SlideSpeed <= 0.0f)
	{
		// Stop sliding when speed reaches zero
		SlideSpeed = 0.0f;
		bIsSliding = false;
	}

	// Calculate the slide offset based on slide speed
	FVector SlideDirection = GetOwner()->GetActorForwardVector();
	FVector SlideOffset = SlideDirection * SlideSpeed;
	FVector NewPosition = GetOwner()->GetActorLocation() + SlideOffset;

	// Interpolate the actor's position towards the new position
	float Alpha = FMath::Clamp(GetWorld()->GetTimerManager().GetTimerElapsed(SlideTimerHandle) / SlideDuration, 0.0f, 1.0f);
	FVector InterpolatedPosition = FMath::Lerp(GetOwner()->GetActorLocation(), NewPosition, Alpha);

	GetOwner()->SetActorLocation(InterpolatedPosition);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *InterpolatedPosition.ToString()));
}

void USlidingComponent::StartSliding()
{
	if (!bIsSliding)
	{
		bIsSliding = true;
		SlideSpeed = 0.0f;

		// Implement any additional logic when sliding is initiated
		UE_LOG(LogTemp, Warning, TEXT("Sliding initiated!"));

		// Start the timer for slide duration
		float SlideDuration = 3.0f;
		GetWorld()->GetTimerManager().SetTimer(SlideTimerHandle, this, &USlidingComponent::Slide, SlideDuration, true);
	}
}

void USlidingComponent::StopSliding()
{
	if (bIsSliding)
	{
		bIsSliding = false;

		// Implement any additional logic when sliding is stopped
		UE_LOG(LogTemp, Warning, TEXT("Sliding stopped!"));

		// Clear the timer handle
		GetWorld()->GetTimerManager().ClearTimer(SlideTimerHandle);
	}
}

bool USlidingComponent::CanSlide() const
{
	// Ensure the character movement component is valid
	UCharacterMovementComponent* CharacterMovement = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	float DistanceToWall = 1000.0f;

	// Check if the character is running
	if (CharacterMovement->Velocity.IsZero())
	{
		return false;
	}

	// Check if the character is close enough to a wall
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = StartLocation + (GetOwner()->GetActorForwardVector() * DistanceToWall);

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(10.0f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, CollisionShape, QueryParams);
	if (bHit && !bIsSliding)
	{
		return false;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Can Slide")));

	return true;
}
