// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class UTankAimingComponent;
class UTankMovementComponent;
class UTankHealthComponent;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnPossessedTankDeath();

private:
	// Sets default values
	ATankAIController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetPawn(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AcceptanceRadius = 3000;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LineTraceRange = 1000000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LineTraceTolerance = 1000.f;

	void AimTowardsPlayer();
	bool GetLineOfSightToLocation(FVector HitLocation);

	UTankAimingComponent* TankAiming = nullptr;
	UTankMovementComponent* TankMovement = nullptr;
	UTankHealthComponent* TankHealth = nullptr;
};
