// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankTurret.h"
#include "TankBarrel.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::Initialise(UTankTurret* TurretToSet, UTankBarrel* BarrelToSet)
{
	Turret = TurretToSet;
	Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (Barrel)
	{
		

		FVector OutLaunchVelocity(0);
		FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
		if (UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, StartLocation, HitLocation, LaunchSpeed, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace))
		{
			auto AimDirection = OutLaunchVelocity.GetSafeNormal();
			auto BarrelRotator = Barrel->GetForwardVector().Rotation();
			auto AimRotator = AimDirection.Rotation();
			auto DeltaRotator = AimRotator - BarrelRotator;
			
			//UE_LOG(LogTemp, Warning, TEXT("%s: DeltaRotator: %s"), *GetOwner()->GetName(), *DeltaRotator.ToString())

			Turret->Rotate(DeltaRotator.Yaw);
			Barrel->Elevate(DeltaRotator.Pitch);
		}
		else
		{

		}
	}
}
