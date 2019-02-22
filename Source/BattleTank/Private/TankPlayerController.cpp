// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "TankHealthComponent.h"

// Sets default values
ATankPlayerController::ATankPlayerController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPawn() && !IsPaused())
	{
		AimTowardsCrosshair();
	}
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		TankAiming = InPawn->FindComponentByClass<UTankAimingComponent>();
		TankHealth = InPawn->FindComponentByClass<UTankHealthComponent>();

		OnPossessedTank(TankAiming, TankHealth);

		if (TankHealth)
		{
			TankHealth->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
		}
	}
}

void ATankPlayerController::OnPossessedTankDeath()
{
	StartSpectatingOnly();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (TankAiming)
	{
		auto HitLocation = FVector(0);
		if (GetSightRayHitLocation(HitLocation))
		{
			TankAiming->AimAt(HitLocation);
		}
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairLocation.X, ViewportSizeY * CrosshairLocation.Y);
	FVector CameraLocation;
	FVector LookDirection;

	if (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraLocation, LookDirection))
	{
		if (GetLookTargetLocation(LookDirection, HitLocation))
		{
			return true;
		}
	}
	return false;
}

bool ATankPlayerController::GetLookTargetLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.AddIgnoredActor(GetPawn());

	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * LineTraceRange;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, QueryParams))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	return false;
}
