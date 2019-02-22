// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/NavigationObjectBase.h"
#include "AutoSpawner.generated.h"

UCLASS()
class BATTLETANK_API AAutoSpawner : public ANavigationObjectBase
{
	GENERATED_BODY()

public:
	AAutoSpawner(const FObjectInitializer& ObjectInitializer);

	/** Arrow component to indicate forward direction of start */
#if WITH_EDITORONLY_DATA
private:
	UPROPERTY()
	class UArrowComponent* ArrowComponent;
public:
#endif

#if WITH_EDITORONLY_DATA
	/** Returns ArrowComponent subobject **/
	class UArrowComponent* GetArrowComponent() const;
#endif


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnDeath();

	UPROPERTY(EditInstanceOnly, Category = "Setup")
	TSubclassOf<AActor> SpawnClass;

	AActor* SpawnedActor = nullptr;
	
	// Delay from Begin Play to spawn an actor for the first time.
	UPROPERTY(EditInstanceOnly, Category = "Setup")
	float BeginDelay = 0.f;

	// Time to wait before spawning a new actor.
	UPROPERTY(EditInstanceOnly, Category = "Setup")
	float RespawnDelay = 5.f;
	
	// Time to wait before retrying to spawn a new actor after a failed attempt (for example if something is overlapping the spawner).
	UPROPERTY(EditInstanceOnly, Category = "Setup")
	float RetryDelay = 1.f;

	// Respawn actor after death? (requires TankHealthComponent)
	UPROPERTY(EditInstanceOnly, Category = "Setup")
	bool bRespawnOnDeath = true;

	UPROPERTY(EditInstanceOnly, Category = "Setup")
	ESpawnActorCollisionHandlingMethod CollisionMethod = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	FTimerHandle Timer;
	void SetSpawnDelay(float Delay);
	void SpawnNewActor();
};
