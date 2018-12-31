// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimingComponent;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* TankAiming);

private:
	// Sets default values
	ATankPlayerController();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetPawn(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly)
	FVector2D CrosshairLocation = FVector2D(0.5, 0.3);

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.f;

	void AimTowardsCrosshair();
	bool GetSightRayHitLocation(FVector& HitLocation) const;
	bool GetLookTargetLocation(FVector LookDirection, FVector& HitLocation) const;

	UFUNCTION()
	void OnPossessedTankDeath();

};
