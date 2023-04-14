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
		TankAimingComp = InPawn->FindComponentByClass<UTankAimingComponent>();
		TankHealthComp = InPawn->FindComponentByClass<UTankHealthComponent>();

		OnPossessedTank(TankAimingComp, TankHealthComp);

		if (TankHealthComp)
		{
			TankHealthComp->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
		}
	}
}

void ATankPlayerController::OnPossessedTankDeath()
{
	StartSpectatingOnly();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (TankAimingComp)
	{
		int32 ViewportSizeX, ViewportSizeY;
		GetViewportSize(ViewportSizeX, ViewportSizeY);

		auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairLocation.X, ViewportSizeY * CrosshairLocation.Y);
		FVector CameraLocation;
		FVector AimDirection;

		if (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraLocation, AimDirection))
		{
			FVector AimLocation;
			if (GetLookTargetLocation(AimDirection, AimLocation))
			{
				TankAimingComp->SetAimingOutOfBounds(false);
				TankAimingComp->AimAt(AimLocation);
			}
			else {
				TankAimingComp->SetAimingOutOfBounds(true);
				TankAimingComp->AimAt(AimDirection);
			}
		}
		else {
			TankAimingComp->SetAimingOutOfBounds(true);
		}
	}
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
