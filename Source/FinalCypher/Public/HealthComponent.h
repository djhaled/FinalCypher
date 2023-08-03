// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChanged, float, OldHealth, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageReceived, float, DamageReceived);

UENUM(BlueprintType)
enum class EAresRegionalDamage : uint8 {
	RegionalDamage_Normal,
	RegionalDamage_Headshot,
	RegionalDamage_Legshot,
	RegionalDamage_RegionCount,
	RegionalDamage_Invalid_Radial,
	RegionalDamage_Invalid,
	RegionalDamage_CountPlusOne,
	EAresRegionalDamage_MAX,
};
UENUM(BlueprintType)
enum class EAresDirectionalDamage : uint8 {
	DirectionalDamage_Front,
	DirectionalDamage_Left,
	DirectionalDamage_Right,
	DirectionalDamage_Rear,
	DirectionalDamage_Count,
	DirectionalDamage_Invalid,
	EAresDirectionalDamage_MAX,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALCYPHER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Health") // Replicate Health variable
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float MaximumHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float DamageScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool bCanBeDestroyed;
	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetHealth();
	UPROPERTY(BlueprintAssignable)
		FHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
		FOnDamageReceived OnDamageReceived;
	UPROPERTY(BlueprintAssignable)
		FOnDeath OnDeath;
	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetMaximumHealth();
	UFUNCTION(BlueprintCallable, Category = "Health")
		float GetNormalizedHealth();
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetHealth(float NewHealth);
	UFUNCTION(BlueprintCallable, Category = "Health")
		bool CheckDeath(float Damage, EAresRegionalDamage RegionDMG, EAresDirectionalDamage DirectionDMG);
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetMaximumHealth(float NewMaximumHealth);
	UFUNCTION(BlueprintCallable, Category = "Health")
		float ApplyDamage(AController* EventInstigator, AActor* DamageCauser, EAresRegionalDamage RegionalDamage, EAresDirectionalDamage Direction, float Damage);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Character") // Replicate bAlive variable
		bool bAlive = true;

		
};
