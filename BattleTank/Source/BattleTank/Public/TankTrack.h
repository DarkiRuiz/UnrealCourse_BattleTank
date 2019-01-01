// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

class ASpringWheel;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

private:
	UTankTrack();
	
	TArray<ASpringWheel*> GetWheels() const;
	
	void DriveTrack(float CurrentThrottle);


public:

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float TrackMaxDrivingForce = 40000000;

	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

};
