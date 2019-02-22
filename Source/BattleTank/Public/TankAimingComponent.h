// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Projectile.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8
{
	Locked,
	Aiming,
	Reloading,
	OutOfAmmo
};

class UTankTurret;
class UTankBarrel;
class AProjectile;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	// Sets default values for this component's properties
	UTankAimingComponent();

	UTankTurret* Turret = nullptr;
	UTankBarrel* Barrel = nullptr;
	double LastFireTime = 0.0;
	FVector AimDirection;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	bool IsBarrelMoving();

private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	FName ProjectileSocket = "Projectile";
	
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 10000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float AimTolerance = 0.02f;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Firing")
	int32 MaxAmmo = 10;

	UPROPERTY(BlueprintReadOnly, Category = "Firing")
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadOnly, Category = "Firing")
	int32 KillCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBP = AProjectile::StaticClass();

	// Sets the references for the turret and barrel components
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankTurret* TurretToSet, UTankBarrel* BarrelToSet);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetRoundsLeft() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetKillCount() const;

	// Fires projectile
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();
	
	// Calculates crosshair projection on the world and aims towards it
	void AimAt(FVector HitLocation);

	void EnemyKilled(AActor* Enemy);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState = EFiringState::Reloading;

};
