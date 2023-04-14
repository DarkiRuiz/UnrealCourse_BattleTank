// Fill out your copyright notice in the Description page of Project Settings.

#include "Vehicle/ChaosTankPawn.h"
#include "Vehicle/ChaosTankMovementComponent.h"

AChaosTankPawn::AChaosTankPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UChaosTankMovementComponent>(VehicleMovementComponentName))
{
	TankMovementComponent = Cast<UChaosTankMovementComponent>(GetVehicleMovementComponent());
}

class UChaosTankMovementComponent* AChaosTankPawn::GetTankMovementComponent() const
{
	return TankMovementComponent;
}
