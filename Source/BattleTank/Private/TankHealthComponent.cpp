// Fill out your copyright notice in the Description page of Project Settings.

#include "TankHealthComponent.h"
#include "TankAimingComponent.h"
#include "TankDamage_Base.h"

// Called when the game starts
void UTankHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	CurrentHealth = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UTankHealthComponent::OnTakeAnyDamage);
}

float UTankHealthComponent::GetHealthPercent() const
{
	return (float)CurrentHealth / (float)MaxHealth;
}

void UTankHealthComponent::KillActor(bool bDestroyActor)
{
	if (CurrentHealth > 0)
	{
		CurrentHealth = 0;
		OnDeath.Broadcast();
		if (bDestroyActor)
		{
			GetOwner()->Destroy();
		}
	}
}

// Called when owner receives damage. "Damage" can be negative, which produces a healing effect.
void UTankHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealth > 0)
	{
		if (Cast<UTankDamage_Base>(DamageType))
		{
			CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
			int32 DamagePoints = FPlatformMath::RoundToInt(Damage);
			CurrentHealth = FMath::Clamp(CurrentHealth - DamagePoints, 0, MaxHealth);
			if (CurrentHealth <= 0)
			{
				if (DamageCauser)
				{
					auto TankAiming = DamageCauser->FindComponentByClass<UTankAimingComponent>();
					if (TankAiming)
					{
						TankAiming->EnemyKilled(GetOwner());
					}
				}
				OnDeath.Broadcast();
			}
		}
	}
}
