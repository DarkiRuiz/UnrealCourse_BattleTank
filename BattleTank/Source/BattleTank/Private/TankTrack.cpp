// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "SpawnPoint.h"
#include "SpringWheel.h"

// Sets default values for this component's properties
UTankTrack::UTankTrack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

TArray<ASpringWheel*> UTankTrack::GetWheels() const
{
	TArray<ASpringWheel*> ResultArray;
	
	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children)
	{
		auto SpawnPoint = Cast<USpawnPoint>(Child);
		if (!SpawnPoint) continue;
		auto SpringWheel = Cast<ASpringWheel>(SpawnPoint->GetSpawnedActor());
		if (!SpringWheel) continue;
		ResultArray.Add(SpringWheel);
	}

	return ResultArray;
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();
	for (ASpringWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}

void UTankTrack::SetThrottle(float Throttle)
{
	auto CurrentThrottle = FMath::Clamp(Throttle, -1.f, 1.f);
	DriveTrack(CurrentThrottle);
}

