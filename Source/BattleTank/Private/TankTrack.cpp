// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "TrackWheel.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values
ATankTrack::ATankTrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Track = CreateDefaultSubobject<UStaticMeshComponent>("Track");
	Tire = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Tire");
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

	SetRootComponent(Track);
	Tire->SetupAttachment(Track);
	Spline->SetupAttachment(Tire);

}

ETrackSide ATankTrack::GetTrackSide() const
{
	return TrackSide;
}

float ATankTrack::GetMaxDrivingForce() const
{
	return MaxDrivingForce;
}

float ATankTrack::GetTurnFactor() const
{
	return TurnFactor;
}

TArray<ATrackWheel*> ATankTrack::GetWheels() const
{
	return TrackWheels;
}

void ATankTrack::SetThrottle(float Throttle)
{
	float MaxForcePerWheel = MaxDrivingForce / TrackWheels.Num();
	for (auto Wheel : TrackWheels)
	{
		Wheel->SetWheelForce(Throttle, MaxForcePerWheel);
	}
}

// Called when the game starts or when spawned
void ATankTrack::BeginPlay()
{
	Super::BeginPlay();
	
	//...
	GenerateTire();
	TrackWheels = FindAttachedWheels();
}


TArray<ATrackWheel*> ATankTrack::FindAttachedWheels()
{
	TArray<ATrackWheel*> Wheels;
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (auto AttachedActor : AttachedActors)
	{
		auto Wheel = Cast<ATrackWheel>(AttachedActor);
		if (Wheel)
		{
			Wheels.Add(Wheel);
			auto Parent = Cast<UChildActorComponent>(GetRootComponent()->GetAttachParent());
			if (Parent)
			{
				auto Body = Cast<UPrimitiveComponent>(Parent->GetAttachParent());
				if (Body)
				{
					Wheel->Initialise(Body);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Track isn't attached through ChildActorComponent."))
			}

		}
	}
	return Wheels;
}

void ATankTrack::GenerateTire()
{
	if (Spline->GetNumberOfSplinePoints() > 1)

	{
		Spline->Duration = SegmentNumber;
		Tire->ClearInstances();
		for (int32 Time = 0; Time < SegmentNumber; Time++)
		{
			Tire->AddInstance(Spline->GetTransformAtTime(Time, ESplineCoordinateSpace::Local, true, true));
		}
	}
}

void ATankTrack::AnimateTire(float Delta, float Rate, UTimelineComponent* Timeline, UPrimitiveComponent* RotationalComponent)
{
	if (!RotationalComponent || !Timeline) return;
	auto RotationalVelocity = RotationalComponent->GetPhysicsAngularVelocityInDegrees();
	auto LocalVelocity = GetActorTransform().InverseTransformPosition(RotationalVelocity + GetActorLocation());
	Timeline->SetPlayRate(LocalVelocity.Y * Rate);
	for (int32 Time = 0; Time < SegmentNumber; Time++)
	{
		auto InstanceTime = FMath::Fmod(Time + (SegmentNumber * Delta), SegmentNumber);
		Tire->UpdateInstanceTransform(Time, Spline->GetTransformAtTime(InstanceTime, ESplineCoordinateSpace::Local, true, true), false, false, true);
	}
	Tire->MarkRenderStateDirty();
}

#if WITH_EDITOR
void ATankTrack::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	GenerateTire();
}

void ATankTrack::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	GenerateTire();
}
#endif
