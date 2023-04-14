// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankTrack.generated.h"

UENUM()
enum class ETrackSide : uint8
{
	None,
	Right,
	Left
};

class UInstancedStaticMeshComponent;
class USplineComponent;
class UTimelineComponent;
class ATrackWheel;

UCLASS()
class BATTLETANK_API ATankTrack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankTrack();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	ETrackSide GetTrackSide() const;
	UFUNCTION(BlueprintCallable, Category = "Setup")
	float GetMaxDrivingForce() const;
	UFUNCTION(BlueprintCallable, Category = "Setup")
	float GetTurnFactor() const;
	UFUNCTION(BlueprintCallable, Category = "Setup")
	TArray<ATrackWheel*> GetWheels() const;

	void SetThrottle(float Throttle);

protected:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Track = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UInstancedStaticMeshComponent* Tire = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USplineComponent* Spline = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	int32 SegmentNumber = 40;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void AnimateTire(float Delta, float Rate, UTimelineComponent* Timeline, UPrimitiveComponent* RotationalComponent);


private:

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	ETrackSide TrackSide = ETrackSide::None;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDrivingForce = 10000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TurnFactor = 0.75;

	TArray<ATrackWheel*> FindAttachedWheels();
	TArray<ATrackWheel*> TrackWheels;
	void GenerateTire();
};
