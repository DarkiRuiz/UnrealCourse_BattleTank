// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ChaosTankPawn.generated.h"


/**
 * 
 */
UCLASS()
class BATTLETANK_API AChaosTankPawn : public AWheeledVehiclePawn
{
	GENERATED_UCLASS_BODY()

	/** vehicle simulation component */
	UPROPERTY(Category = Vehicle, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UChaosTankMovementComponent> TankMovementComponent;

public:

	/** Util to get the wheeled vehicle movement component */
	class UChaosTankMovementComponent* GetTankMovementComponent() const;

	/** Returns VehicleMovement subobject **/
	class UChaosTankMovementComponent* GetTankMovement() const { return TankMovementComponent; }
};
