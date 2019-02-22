// Fill out your copyright notice in the Description page of Project Settings.

#include "TankWheel.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

void UTankWheel::BeginPlay()
{
	Super::BeginPlay();

	//...

	BeginSuspension();
}

void UTankWheel::BeginSuspension()
{
	auto Parent = Cast<UPrimitiveComponent>(GetAttachParent());
	if (Parent && GetOwner())
	{

		Suspension = NewObject<UPhysicsConstraintComponent>(GetOwner());
		Suspension->AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform, GetAttachSocketName());
		Suspension->RegisterComponent();
		Suspension->SetRelativeLocationAndRotation(RelativeLocation, RelativeRotation);
		Parent->SetSimulatePhysics(true);
		SetSimulatePhysics(true);
		Suspension->SetConstrainedComponents(Parent, NAME_None, this, NAME_None);
		Suspension->SetLinearXLimit(LCM_Locked, 0.f);
		Suspension->SetLinearYLimit(LCM_Locked, 0.f);
		Suspension->SetLinearZLimit(LCM_Locked, 0.f);
		Suspension->SetAngularSwing1Limit(ACM_Locked, 45.f);
		Suspension->SetAngularSwing2Limit(ACM_Free, 45.f);
		Suspension->SetAngularTwistLimit(ACM_Locked, 45.f);
	}
}
