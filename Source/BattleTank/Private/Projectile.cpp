// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	SetRootComponent(CollisionMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->SetupAttachment(CollisionMesh);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->SetupAttachment(ExplosionForce);
	LaunchBlast->bAutoActivate = false;

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->SetupAttachment(ExplosionForce);
	ImpactBlast->bAutoActivate = false;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	LaunchBlast->OnSystemFinished.AddDynamic(this, &AProjectile::OnSystemFinished);
	ImpactBlast->OnSystemFinished.AddDynamic(this, &AProjectile::OnSystemFinished);

}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetInstigator() != nullptr)
	{
		GetInstigator()->ForEachComponent<UPrimitiveComponent>(true, [&](UPrimitiveComponent* InPrimComp)
			{
				if (InPrimComp->IsRegistered() && InPrimComp->IsCollisionEnabled())
				{
					CollisionMesh->IgnoreComponentWhenMoving(InPrimComp, false);
					InPrimComp->IgnoreComponentWhenMoving(CollisionMesh, false);
				}
			});
	}

	Super::EndPlay(EndPlayReason);
}

float AProjectile::GetProjectileAmmo() const
{
	return ProjectileAmmo;
}

float AProjectile::GetProjectileDamage() const
{
	return ProjectileDamage;
}

void AProjectile::Launch(float Speed)
{
	UGameplayStatics::ApplyDamage(GetInstigator(), ProjectileAmmo, GetInstigatorController(), GetInstigator(), ProjectileAmmoType);
	
	if (GetInstigator() != nullptr) 
	{
		GetInstigator()->ForEachComponent<UPrimitiveComponent>(true, [&](UPrimitiveComponent* InPrimComp)
			{
				if (InPrimComp->IsRegistered() && InPrimComp->IsCollisionEnabled())
				{
					CollisionMesh->IgnoreComponentWhenMoving(InPrimComp, true);
					InPrimComp->IgnoreComponentWhenMoving(CollisionMesh, true);
				}
			});
	}
	
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
	LaunchBlast->Activate();
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	bool bSelfCollision = false;
	
	if (GetInstigator() != nullptr)
	{
		GetInstigator()->ForEachComponent<UPrimitiveComponent>(true, [&](UPrimitiveComponent* InPrimComp)
			{
				if (InPrimComp->IsRegistered() && InPrimComp->IsCollisionEnabled())
				{
					if (OtherComponent == InPrimComp)
					{
						bSelfCollision = true;
					}
				}
			});
	}

	if (bSelfCollision) return;

	SetActorEnableCollision(false);
	CollisionMesh->SetVisibility(false);

	ProjectileMovement->SetVelocityInLocalSpace(FVector(0));
	ProjectileMovement->Deactivate();

	ExplosionForce->FireImpulse();

	UGameplayStatics::ApplyRadialDamage(this, ProjectileDamage, ExplosionForce->GetComponentLocation(), ExplosionForce->Radius, ProjectileDamageType, TArray<AActor*>(), GetInstigator(), GetInstigatorController());

	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	
	if (DestroyDelay > 0.f)
	{
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectile::OnTimerExpired, DestroyDelay, false);
	}
}

void AProjectile::OnSystemFinished(UParticleSystemComponent* PSystem)
{
	if (bSystemFinished)
	{
		Destroy();
	}
	else
	{
		bSystemFinished = true;
	}
}

void AProjectile::OnTimerExpired()
{
	Destroy();
}
