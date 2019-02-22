// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankTurret.h"
#include "TankBarrel.h"
#include "TankAmmo_Base.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ProjectileBP->GetDefaultObject<AProjectile>()->GetProjectileAmmo() > GetRoundsLeft())
	{
		FiringState = EFiringState::OutOfAmmo;
	}

	else if ((GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
	LastFireTime = GetWorld()->GetTimeSeconds();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UTankAimingComponent::OnTakeAnyDamage);
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Turret)) { return false; }
	if (!ensure(Barrel)) { return false; }
	if (!ensure(ProjectileBP)) { return false; }
	return !Barrel->GetForwardVector().Equals(AimDirection, AimTolerance);
}

void UTankAimingComponent::OnTakeAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Cast<UTankAmmo_Base>(DamageType))
	{
		CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, MaxAmmo);
		int32 AmmoPoints = FPlatformMath::RoundToInt(Damage);
		CurrentAmmo = FMath::Clamp(CurrentAmmo - AmmoPoints, 0, MaxAmmo);
	}
}

void UTankAimingComponent::Initialise(UTankTurret* TurretToSet, UTankBarrel* BarrelToSet)
{
	Turret = TurretToSet;
	Barrel = BarrelToSet;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return CurrentAmmo;
}

int32 UTankAimingComponent::GetKillCount() const
{
	return KillCount;
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Turret)) { return; }
	if (!ensure(Barrel)) { return; }
	if (!ensure(ProjectileBP)) { return; }

	if (FiringState == EFiringState::Locked)
	{
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.Instigator = Cast<APawn>(GetOwner());
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBP, Barrel->GetSocketLocation(ProjectileSocket), Barrel->GetSocketRotation(ProjectileSocket), SpawnParameters);
		Projectile->Launch(LaunchSpeed);
		LastFireTime = GetWorld()->GetTimeSeconds();
	}
}

void UTankAimingComponent::EnemyKilled(AActor* Enemy)
{
	KillCount++;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Turret)) { return; }
	if (!ensure(Barrel)) { return; }
	if (!ensure(ProjectileBP)) { return; }

	FVector OutLaunchVelocity(0);
	FVector StartLocation = Barrel->GetSocketLocation(ProjectileSocket);
	if (UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, StartLocation, HitLocation, LaunchSpeed, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace))
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		auto BarrelRotator = Barrel->GetForwardVector().Rotation();
		auto AimRotator = AimDirection.Rotation();
		auto DeltaRotator = AimRotator - BarrelRotator;
		if (FMath::Abs(DeltaRotator.Yaw) < 180)
		{
			Turret->Rotate(DeltaRotator.Yaw);
		}
		else
		{
			Turret->Rotate(-DeltaRotator.Yaw);
		}
		Barrel->Elevate(DeltaRotator.Pitch);
	}
}
