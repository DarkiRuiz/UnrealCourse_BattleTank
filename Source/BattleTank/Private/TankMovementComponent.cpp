// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"

// Sets default values for this component's properties
UTankMovementComponent::UTankMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	bAutoActivate = false;
}

void UTankMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		RightForceThisFrame = 0.f;
		LeftForceThisFrame = 0.f;
	}
}

void UTankMovementComponent::Initialise(UStaticMeshComponent* MassToSet, UStaticMeshComponent* RightTrackToSet, UStaticMeshComponent* LeftTrackToSet, TArray<UStaticMeshComponent*> RightWheelsToSet, TArray<UStaticMeshComponent*> LeftWheelsToSet)
{
	Mass = MassToSet;

	RightTrack = RightTrackToSet;
	LeftTrack = LeftTrackToSet;

	RightWheels = RightWheelsToSet;
	LeftWheels = LeftWheelsToSet;

	if (RightTrack)
	{
		for (auto RightWheel : RightWheels)
		{
			RightWheel->OnComponentHit.AddDynamic(this, &UTankMovementComponent::OnRightHit);
		}
	}

	if (LeftTrack)
	{
		for (auto LeftWheel : LeftWheels)
		{
			LeftWheel->OnComponentHit.AddDynamic(this, &UTankMovementComponent::OnLeftHit);
		}
	}

	Activate();
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	SetThrottle(Throw, RightForceThisFrame);
	SetThrottle(Throw, LeftForceThisFrame);
}

void UTankMovementComponent::IntendMoveRight(float Throw)
{
	SetThrottle(-Throw, RightForceThisFrame);
	SetThrottle(Throw, LeftForceThisFrame);
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

void UTankMovementComponent::OnRightHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto ForceMagnitude = RightForceThisFrame * MaxDrivingForce * Mass->GetMass() / RightWheels.Num();
	auto ForceToAdd = Mass->GetForwardVector() * ForceMagnitude;
	auto ForceLocation = HitComponent->GetComponentLocation() + RightForceOffset;
	
	HitComponent->AddForceAtLocation(ForceToAdd, ForceLocation);
}

void UTankMovementComponent::OnLeftHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto ForceMagnitude = LeftForceThisFrame * MaxDrivingForce * Mass->GetMass() / LeftWheels.Num();
	auto ForceToAdd = Mass->GetForwardVector() * ForceMagnitude;
	auto ForceLocation = HitComponent->GetComponentLocation() + LeftForceOffset;

	HitComponent->AddForceAtLocation(ForceToAdd, ForceLocation);
}

void UTankMovementComponent::SetThrottle(float Throttle, float& ForceThisFrame)
{
	ForceThisFrame += FMath::Clamp(Throttle, -1.f, 1.f);
}
