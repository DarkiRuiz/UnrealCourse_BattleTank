// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "OccludingWidget.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UOccludingWidget : public UWidgetComponent
{
	GENERATED_BODY()

private:
	UOccludingWidget();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void TestForOcclusion();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ProximityRadius = 1000.f;

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void ActivateWidgetOcclusion();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void DeactivateWidgetOcclusion(bool bVisibility = false);

};
