// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Tank.h"

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

	auto TankMovement = GetPawn()->FindComponentByClass<UTankMovementComponent>();
	auto TankAiming = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	if (PlayerTank) {
		if (TankMovement)
		{
			MoveToActor(PlayerTank, AcceptanceRadius);
		}
		if (TankAiming)
		{
			TankAiming->AimAt(PlayerTank->GetActorLocation());
			TankAiming->Fire();
		}
	}
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (ensure(PossessedTank))
		{
			PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
		}
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	GetPawn()->DetachFromControllerPendingDestroy();
}

