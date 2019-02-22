// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UTankMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// Sets the references for the turret and barrel components
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UStaticMeshComponent* MassToSet, UStaticMeshComponent* RightTrackToSet, UStaticMeshComponent* LeftTrackToSet, TArray<UStaticMeshComponent*> RightWheelsToSet, TArray<UStaticMeshComponent*> LeftWheelsToSet);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveForward(float Throw);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveRight(float Throw);

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDrivingForce = 10000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FVector RightForceOffset = FVector(0);

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FVector LeftForceOffset = FVector(0);

	/** path following: request new velocity */
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

private:
	UFUNCTION()
	void OnRightHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnLeftHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetThrottle(float Throttle, float& ForceThisFrame);

	UStaticMeshComponent* Mass;
	UStaticMeshComponent* RightTrack;
	UStaticMeshComponent* LeftTrack;
	TArray<UStaticMeshComponent*> RightWheels;
	TArray<UStaticMeshComponent*> LeftWheels;

	float RightForceThisFrame = 0.f;
	float LeftForceThisFrame = 0.f;
};
