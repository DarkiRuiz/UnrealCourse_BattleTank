// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent_Test.h"
#include "TankTrack_Test.h"


void UTankMovementComponent_Test::BeginPlay()
{
	Super::BeginPlay();

	//...

	Initialise();
}


void UTankMovementComponent_Test::IntendMoveForward(float Throw)
{
	if (RightTrack && LeftTrack)
	{
		RightTrack->SetThrottle(Throw);
		LeftTrack->SetThrottle(Throw);
	}
}


void UTankMovementComponent_Test::IntendMoveRight(float Throw)
{
	if (RightTrack && LeftTrack)
	{
		RightTrack->SetThrottle(-RightTrack->GetTurnFactor() * Throw);
		LeftTrack->SetThrottle(LeftTrack->GetTurnFactor() * Throw);
	}
}


ATankTrack_Test* UTankMovementComponent_Test::GetRightTrack() const
{
	return RightTrack;
}

ATankTrack_Test* UTankMovementComponent_Test::GetLeftTrack() const
{
	return LeftTrack;
}


void UTankMovementComponent_Test::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();
	auto AIForwardDirection = GetOwner()->GetActorForwardVector();

	auto ForwardThrow = FVector::DotProduct(AIForwardDirection, AIForwardIntention);
	IntendMoveForward(ForwardThrow);

	auto RightThrow = FVector::CrossProduct(AIForwardDirection, AIForwardIntention).Z;
	IntendMoveRight(RightThrow);
}


void UTankMovementComponent_Test::Initialise()
{
	TArray<USceneComponent*> Children;

	TArray<AActor*> AttachedActors;
	GetOwner()->GetAttachedActors(AttachedActors);

	for (auto AttachedActor : AttachedActors)
	{
		auto Track = Cast<ATankTrack_Test>(AttachedActor);
		if (Track)
		{
			if (Track->GetTrackSide() == ETrackSide_Test::Right)
			{
				RightTrack = Track;
			}
			else if (Track->GetTrackSide() == ETrackSide_Test::Left)
			{
				LeftTrack = Track;
			}
		}
	}
}
