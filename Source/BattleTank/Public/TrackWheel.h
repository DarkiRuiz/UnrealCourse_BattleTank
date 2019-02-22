// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackWheel.generated.h"

class UPhysicsConstraintComponent;
class USphereComponent;

UCLASS()
class BATTLETANK_API ATrackWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrackWheel();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UPrimitiveComponent* BodyToSet);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	float GetGripRadius() const;

	void SetWheelForce(float Throttle, float Force);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPrimitiveComponent* Body = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* BodyToAxle = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* Axle = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* AxleToWheel = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Wheel = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float GripRadius = 500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float GripForce = 0.75;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TorqueRate = 1000.f;

	float WheelThrottleThisFrame = 0.f;
	float WheelForceThisFrame = 0.f;
	bool bGrip = false;
	FHitResult GripHit;	

	void UpdateGrip();
	void UpdateTorque();

};
