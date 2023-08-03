#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MantleComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FINALCYPHER_API UMantleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMantleComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Function to initiate the mantle action
	void StartMantle();

private:
	// Collision detection settings
	UPROPERTY(EditAnywhere)
		float MantleCheckDistance = 200.0f;

	UPROPERTY(EditAnywhere)
		float MantleCheckHeight = 100.0f;

	UPROPERTY(EditAnywhere)
		float MantleCheckRadius = 50.0f;

	// Function to check if the character can mantle
	bool CanMantle() const;
};