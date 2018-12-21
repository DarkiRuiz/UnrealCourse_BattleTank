// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TankBarrel.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

public:	

	void SetBarrelReference(UTankBarrel* BarrelToSet);
	//TODO Set turret reference
	void AimAt(FVector HitLocation, float LaunchSpeed);
	void MoveBarrelTowards(FVector AimDirection);

private:
	UTankBarrel* Barrel = nullptr;

};
