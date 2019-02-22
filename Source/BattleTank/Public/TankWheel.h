// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankWheel.generated.h"

class UPhysicsConstraintComponent;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankWheel : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void BeginSuspension();
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPhysicsConstraintComponent* Suspension = nullptr;

};
