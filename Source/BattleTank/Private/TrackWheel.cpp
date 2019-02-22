// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackWheel.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ATrackWheel::ATrackWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	BodyToAxle = CreateDefaultSubobject<UPhysicsConstraintComponent>("Body to Axle");
	Axle = CreateDefaultSubobject<USphereComponent>("Axle");
	AxleToWheel = CreateDefaultSubobject<UPhysicsConstraintComponent>("Axle to Wheel");
	Wheel = CreateDefaultSubobject<UStaticMeshComponent>("Wheel");

	SetRootComponent(BodyToAxle);
	Axle->SetupAttachment(BodyToAxle);
	AxleToWheel->SetupAttachment(Axle);
	Wheel->SetupAttachment(AxleToWheel);

	BodyToAxle->SetLinearXLimit(LCM_Locked, 0.f);
	BodyToAxle->SetLinearYLimit(LCM_Locked, 0.f);
	BodyToAxle->SetLinearZLimit(LCM_Free, 0.f);
	BodyToAxle->SetAngularSwing1Limit(ACM_Locked, 45.f);
	BodyToAxle->SetAngularSwing2Limit(ACM_Locked, 45.f);
	BodyToAxle->SetAngularTwistLimit(ACM_Locked, 45.f);
	BodyToAxle->SetLinearPositionDrive(false, false, true);
	BodyToAxle->SetLinearVelocityDrive(false, false, true);

	Axle->SetSimulatePhysics(true);
	Axle->SetNotifyRigidBodyCollision(false);
	Axle->SetGenerateOverlapEvents(false);
	Axle->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Axle->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Axle->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Axle->SetCanEverAffectNavigation(false);
	Axle->bDrawOnlyIfSelected = true;

	AxleToWheel->SetLinearXLimit(LCM_Locked, 0.f);
	AxleToWheel->SetLinearYLimit(LCM_Locked, 0.f);
	AxleToWheel->SetLinearZLimit(LCM_Locked, 0.f);
	AxleToWheel->SetAngularSwing1Limit(ACM_Locked, 45.f);
	AxleToWheel->SetAngularSwing2Limit(ACM_Free, 45.f);
	AxleToWheel->SetAngularTwistLimit(ACM_Locked, 45.f);

	Wheel->SetSimulatePhysics(true);
	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->SetGenerateOverlapEvents(false);
	Wheel->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Wheel->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Wheel->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Wheel->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Wheel->SetCanEverAffectNavigation(false);
}


// Called every frame
void ATrackWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//...

	if (Body && GetWorld()->TickGroup == TG_PostPhysics)
	{	
		UpdateGrip();
		UpdateTorque();

		WheelThrottleThisFrame = 0.f;
		WheelForceThisFrame = 0.f;
	}
}

void ATrackWheel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	bGrip = true;
	GripHit = Hit;
}


void ATrackWheel::SetWheelForce(float Throttle, float Force)
{
	WheelThrottleThisFrame = FMath::Clamp(WheelThrottleThisFrame + Throttle, -1.f, 1.f);
	WheelForceThisFrame = Force;	
}


void ATrackWheel::Initialise(UPrimitiveComponent* BodyToSet)
{
	Body = BodyToSet;
	if (Body)	
	{
		BodyToAxle->SetConstrainedComponents(Body, NAME_None, Axle, NAME_None);
		AxleToWheel->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);

		Wheel->OnComponentHit.AddDynamic(this, &ATrackWheel::OnComponentHit); 
	}
}

float ATrackWheel::GetGripRadius() const
{
	return GripRadius;
}

void ATrackWheel::UpdateGrip()
{
	if (!bGrip) return;

	FHitResult TraceHit;
	auto QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Body->GetOwner());

	auto TraceRange = FVector2D(GripHit.Distance, GripHit.Distance + GripRadius);
	float HitMinRadius = GripHit.Distance;
	float HitMaxRadius = HitMinRadius + GripRadius;
	FVector StartLocation = Axle->GetComponentLocation();
	FVector EndLocation = StartLocation - GripHit.ImpactNormal * TraceRange.Y;

	if (GetWorld()->LineTraceSingleByChannel(TraceHit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldStatic, QueryParams))
	{
		float SpeedRate = Axle->GetComponentVelocity().Size();		
		float HitRate = FMath::GetRangePct(TraceRange, TraceHit.Distance);
		float InclinationRate = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(-1.f, 1.f), Axle->GetUpVector().Z);

		FVector GripImpulse = GripHit.ImpactNormal * GripForce * SpeedRate * HitRate * InclinationRate * -1;

		Axle->AddImpulse(GripImpulse, NAME_None, false);
	}
	else {
		bGrip = false;
	}
}

void ATrackWheel::UpdateTorque()
{
	float WheelTorqueThisFrame = WheelThrottleThisFrame * WheelForceThisFrame * TorqueRate;
	Wheel->AddTorqueInDegrees(Axle->GetRightVector() * WheelTorqueThisFrame, NAME_None, false);
}

