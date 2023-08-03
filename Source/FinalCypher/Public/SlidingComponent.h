#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlidingComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FINALCYPHER_API USlidingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USlidingComponent();

protected:
	virtual void BeginPlay() override;

public:


	// Function to initiate the sliding action
	UFUNCTION(BlueprintCallable, Category = "Slide")
		void StartSliding();

private:
	// Sliding settings
	UPROPERTY(EditAnywhere)
		float SlideAcceleration = 1500.0f;

	UPROPERTY(EditAnywhere)
		float MaxSlideSpeed = 1000.0f;

	UPROPERTY(EditAnywhere)
		float SlideFriction = 10.0f;

	// Maximum duration of sliding in seconds
	UPROPERTY(EditAnywhere)
		float MaxSlideDuration = 2.0f;

	// Function to check if the character can slide
	UFUNCTION(BlueprintCallable, Category = "Slide")
		bool CanSlide() const;

	// Function to perform the sliding movement
	void Slide();

	// Function to stop sliding
	void StopSliding();

	// Sliding state variables
	bool bIsSliding = false;
	float SlideSpeed = 0.0f;
	float CurrentSlideDuration = 0.0f;
	FTimerHandle SlideTimerHandle;

};
