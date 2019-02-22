// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "TrackWheel.h"


void UTankTrack::BeginPlay()
{
	Super::BeginPlay();

	//...

	Wheels = FindAttachedWheels();

}


void UTankTrack::SetThrottle(float Throttle)
{
	float MaxForcePerWheel = MaxDrivingForce / Wheels.Num();
	for (auto Wheel : Wheels)
	{
		Wheel->SetWheelForce(Throttle, MaxForcePerWheel);
	}
}


ETrackSide UTankTrack::GetTrackSide() const
{
	return TrackSide;
}


float UTankTrack::GetMaxDrivingForce() const
{
	return MaxDrivingForce;
}


float UTankTrack::GetTurnFactor() const
{
	return TurnFactor;
}

TArray<ATrackWheel*> UTankTrack::GetWheels() const
{
	return Wheels;
}


TArray<ATrackWheel*> UTankTrack::FindAttachedWheels()
{
	TArray<ATrackWheel*> Wheels;	
	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	for (auto Component : Children)
	{
		auto Child = Cast<UChildActorComponent>(Component);
		if (Child)
		{
			auto Wheel = Cast<ATrackWheel>(Child->GetChildActor());
			if (Wheel) 
			{
				Wheels.Add(Wheel);
				auto Body = Cast<UPrimitiveComponent>(GetAttachParent());
				auto BodyActor = Cast<UChildActorComponent>(GetAttachParent());
				if (Body)
				{
					Wheel->Initialise(Body);
				}
				else if (BodyActor)
				{
					Wheel->Initialise(Cast<UPrimitiveComponent>(BodyActor->GetAttachParent()));
				}
			}
		}
	}
	return Wheels;
}
