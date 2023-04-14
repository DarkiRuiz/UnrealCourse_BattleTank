// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "ChaosTankMovementComponent.generated.h"




class BATTLETANK_API UChaosTankVehicleSimulation : public UChaosWheeledVehicleSimulation
{
public:
	UChaosTankVehicleSimulation()
		: UChaosWheeledVehicleSimulation()
	{

	}

	virtual ~UChaosTankVehicleSimulation()
	{
	}



	///** Process steering mechanism */
	
	virtual void ProcessSteering(const FControlInputs& ControlInputs) override;

};

/**
 * 
 */
UCLASS()
class BATTLETANK_API UChaosTankMovementComponent : public UChaosWheeledVehicleMovementComponent
{
	GENERATED_BODY()

public:


	/** */
	virtual TUniquePtr<Chaos::FSimpleWheeledVehicle> CreatePhysicsVehicle() override
	{
		// Make the Vehicle Simulation class that will be updated from the physics thread async callback
		VehicleSimulationPT = MakeUnique<UChaosTankVehicleSimulation>();

		return UChaosVehicleMovementComponent::CreatePhysicsVehicle();
	}


};

