// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "TankHealthComponent.h"

// Sets default values
ATankAIController::ATankAIController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPawn())
	{
		AimTowardsPlayer();
	}
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		TankAiming = InPawn->FindComponentByClass<UTankAimingComponent>();
		TankMovement = InPawn->FindComponentByClass<UTankMovementComponent>();
		TankHealth = InPawn->FindComponentByClass<UTankHealthComponent>();
		if (TankHealth)
		{
			TankHealth->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
		}
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (GetPawn())
	{
		GetPawn()->DetachFromControllerPendingDestroy();
	}
}

void ATankAIController::AimTowardsPlayer()
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn)
	{
		if (TankMovement)
		{
			MoveToActor(PlayerPawn, AcceptanceRadius);
		}
		if (TankAiming)
		{	
			FVector HitLocation = PlayerPawn->GetActorLocation();
			TankAiming->AimAt(HitLocation);
			if (GetLineOfSightToLocation(HitLocation))
			{
				TankAiming->Fire();
			}
		}
	}
}

bool ATankAIController::GetLineOfSightToLocation(FVector HitLocation)
{
	FHitResult HitResult;
	auto StartLocation = GetPawn()->GetActorLocation();
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, HitLocation, ECollisionChannel::ECC_Visibility))
	{
		auto EndLocation = HitResult.Location;
		if (EndLocation.Equals(HitLocation, LineTraceTolerance))
		return true;
	}
	return false;
}









