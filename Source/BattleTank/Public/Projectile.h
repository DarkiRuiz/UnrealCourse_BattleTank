// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankAmmo_Base.h"
#include "TankDamage_Base.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class URadialForceComponent;
class UParticleSystemComponent;

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* CollisionMesh = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	URadialForceComponent* ExplosionForce = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* LaunchBlast = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* ImpactBlast = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	float GetProjectileAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	float GetProjectileDamage() const;

	void Launch(float Speed);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<UTankAmmo_Base> ProjectileAmmoType = UTankAmmo_Base::StaticClass();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<UTankDamage_Base> ProjectileDamageType = UTankDamage_Base::StaticClass();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	float ProjectileAmmo = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	float ProjectileDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Setup")
	float DestroyDelay = 5.f;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnSystemFinished(UParticleSystemComponent* PSystem);

	bool bSystemFinished = false;

	void OnTimerExpired();
};
