// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankHealthDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	int32 MaxHealth = 100;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	int32 CurrentHealth;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FTankHealthDelegate OnDeath;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void KillActor(bool bDestroyActor = false);

private:
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
