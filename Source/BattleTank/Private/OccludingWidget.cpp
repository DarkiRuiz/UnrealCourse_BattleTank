// Fill out your copyright notice in the Description page of Project Settings.

#include "OccludingWidget.h"

UOccludingWidget::UOccludingWidget()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	bAutoActivate = false;
	SetVisibility(false);
}

void UOccludingWidget::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TestForOcclusion();
}

void UOccludingWidget::ActivateWidgetOcclusion()
{
	Activate();
}

void UOccludingWidget::DeactivateWidgetOcclusion(bool bVisibility)
{
	Deactivate();
	SetVisibility(bVisibility);
}

void UOccludingWidget::TestForOcclusion()
{
	auto Player = GetWorld()->GetFirstPlayerController();
	if (!Player || !Player->GetPawn()) return;

	auto ParentLocation = GetOwner()->GetActorLocation();
	auto PlayerLocation = Player->GetPawn()->GetActorLocation();

	auto DistanceFromPlayer = (ParentLocation - PlayerLocation).Size();

	if (DistanceFromPlayer < ProximityRadius)
	{
		SetVisibility(true);
	}
	else
	{
		FHitResult HitResult;

		auto StartLocation = Player->PlayerCameraManager->GetCameraLocation();
		auto EndLocation = GetComponentLocation();
		auto QueryParams = FCollisionQueryParams::DefaultQueryParam;
		QueryParams.AddIgnoredActor(GetOwner());
		QueryParams.AddIgnoredActor(Player->GetPawn());

		bool bCanBeSeen = !GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, QueryParams);
		bool bCanParentBeSeen = !GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, ParentLocation, ECollisionChannel::ECC_Visibility, QueryParams);

		SetVisibility(bCanBeSeen || bCanParentBeSeen);
	}
}