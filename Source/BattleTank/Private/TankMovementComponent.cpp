// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"


void UTankMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//...

	Initialise();
}


void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (RightTrack && LeftTrack)
	{
		RightTrack->SetThrottle(Throw);
		LeftTrack->SetThrottle(Throw);
	}
}


void UTankMovementComponent::IntendMoveRight(float Throw)
{
	if (RightTrack && LeftTrack)
	{
		RightTrack->SetThrottle(-RightTrack->GetTurnFactor() * Throw);
		LeftTrack->SetThrottle(LeftTrack->GetTurnFactor() * Throw);
	}
}


ATankTrack* UTankMovementComponent::GetRightTrack() const
{
	return RightTrack;
}

ATankTrack* UTankMovementComponent::GetLeftTrack() const
{
	return LeftTrack;
}


void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();
	auto AIForwardDirection = GetOwner()->GetActorForwardVector();

	auto ForwardThrow = FVector::DotProduct(AIForwardDirection, AIForwardIntention);
	IntendMoveForward(ForwardThrow);

	auto RightThrow = FVector::CrossProduct(AIForwardDirection, AIForwardIntention).Z;
	IntendMoveRight(RightThrow);
}


void UTankMovementComponent::Initialise()
{
	TArray<USceneComponent*> Children;

	TArray<AActor*> AttachedActors;
	GetOwner()->GetAttachedActors(AttachedActors);

	for (auto AttachedActor : AttachedActors)
	{
		auto Track = Cast<ATankTrack>(AttachedActor);
		if (Track)
		{
			if (Track->GetTrackSide() == ETrackSide::Right)
			{
				RightTrack = Track;
			}
			else if (Track->GetTrackSide() == ETrackSide::Left)
			{
				LeftTrack = Track;
			}
		}
	}
}
