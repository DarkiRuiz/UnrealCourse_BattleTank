// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

UENUM()
enum class ETrackSide : uint8
{
	None,
	Right,
	Left
};

class ATrackWheel;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public: 
	UFUNCTION(BlueprintCallable, Category = "Setup")
	ETrackSide GetTrackSide() const;
	UFUNCTION(BlueprintCallable, Category = "Setup")
	float GetMaxDrivingForce() const;
	UFUNCTION(BlueprintCallable, Category = "Setup")
	float GetTurnFactor() const;
	UFUNCTION(BlueprintCallable, Category = "Setup")
	TArray<ATrackWheel*> GetWheels() const;

	void SetThrottle(float Throttle);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	ETrackSide TrackSide = ETrackSide::None;	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDrivingForce = 10000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TurnFactor = 0.75;

	TArray<ATrackWheel*> FindAttachedWheels();
	TArray<ATrackWheel*> Wheels;
};
