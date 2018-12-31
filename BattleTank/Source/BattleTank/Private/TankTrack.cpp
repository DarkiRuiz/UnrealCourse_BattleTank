// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

// Sets default values for this component's properties
UTankTrack::UTankTrack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();

	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0.f;
}

void UTankTrack::ApplySidewaysForce()
{
	auto SlipSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	auto AccelCorrection = -SlipSpeed / GetWorld()->GetDeltaSeconds() * GetRightVector();
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto ForceCorrection = TankRoot->GetMass() * AccelCorrection / 2;
	auto ForceLocation = GetSocketLocation(FName("TrackForce"));

	TankRoot->AddForceAtLocation(ForceCorrection, ForceLocation);
}

void UTankTrack::DriveTrack()
{
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	auto ForceLocation = GetSocketLocation(FName("TrackForce"));

	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
	
}

void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp(CurrentThrottle + Throttle, -1.f, 1.f);
}

