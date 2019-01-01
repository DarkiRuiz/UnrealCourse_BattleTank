// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpringWheel.generated.h"

class USphereComponent;
class UPhysicsConstraintComponent;

UCLASS()
class BATTLETANK_API ASpringWheel : public AActor
{
	GENERATED_BODY()
	
private:
	// Sets default values for this actor's properties
	ASpringWheel();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void SetupConstraints();
	void ApplyForce();

public:	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USphereComponent* Axle = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USphereComponent* Wheel = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UPhysicsConstraintComponent* MassAxleConstraint = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UPhysicsConstraintComponent* AxleWheelConstraint = nullptr;

	void AddDrivingForce(float ForceMagnitude);

	float TotalForceMagnitudeThisFrame = 0.f;

};
