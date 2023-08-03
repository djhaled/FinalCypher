// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DominationFlag.generated.h"

class USphereComponent;
class USpotLightComponent;
class APlayerCharacter;

UCLASS()
class FINALCYPHER_API ADominationFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
		TObjectPtr<USphereComponent> Sphere1;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
		TObjectPtr<USpotLightComponent> SpotLight;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
		TObjectPtr<UStaticMeshComponent> Cylinder;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
		TObjectPtr<USceneComponent> DefaultSceneRoot;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
		TObjectPtr<UMaterialInstanceDynamic> FlagMaterial;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UWidgetComponent* WidgetComponent;


	ADominationFlag();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
