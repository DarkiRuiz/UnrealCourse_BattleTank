// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent_Test.generated.h"

class ATankTrack_Test;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent_Test : public UNavMovementComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	ATankTrack_Test* GetRightTrack() const;
	UFUNCTION(BlueprintCallable, Category = "Setup")
	ATankTrack_Test* GetLeftTrack() const;
	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveForward(float Throw);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveRight(float Throw);

	/** path following: request new velocity */
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

private:
	ATankTrack_Test* RightTrack;
	ATankTrack_Test* LeftTrack;
	
	void Initialise();
};
