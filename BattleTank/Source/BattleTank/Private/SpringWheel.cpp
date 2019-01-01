// Fill out your copyright notice in the Description page of Project Settings.

#include "SpringWheel.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ASpringWheel::ASpringWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	MassAxleConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassAxleConstraint"));
	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));

	SetRootComponent(MassAxleConstraint);
	Axle->SetupAttachment(MassAxleConstraint);
	AxleWheelConstraint->SetupAttachment(Axle);
	Wheel->SetupAttachment(AxleWheelConstraint);
}

// Called when the game starts or when spawned
void ASpringWheel::BeginPlay()
{
	Super::BeginPlay();

	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASpringWheel::OnHit);

	SetupConstraints();

}

// Called every frame
void ASpringWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->TickGroup == TG_PostPhysics)
	{
		TotalForceMagnitudeThisFrame = 0.f;
	}
}

void ASpringWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ApplyForce();
}

void ASpringWheel::SetupConstraints()
{
	if (!GetAttachParentActor()) return;
	UPrimitiveComponent* Mass = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
	if (!Mass) return;
	MassAxleConstraint->SetConstrainedComponents(Mass, NAME_None, Axle, NAME_None);
	AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);
}


void ASpringWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}

void ASpringWheel::ApplyForce()
{

	Wheel->AddForce(Axle->GetForwardVector() * TotalForceMagnitudeThisFrame);
}