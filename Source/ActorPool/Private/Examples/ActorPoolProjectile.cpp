// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ActorPoolProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

void AActorPoolProjectile::OnActivated()
{
	ProjectileMovement->SetUpdatedComponent(RootComponent);
	const UProjectileMovementComponent* const DefaultMovement = GetClass()->GetDefaultObject<AActorPoolProjectile>()->ProjectileMovement;
	ProjectileMovement->SetVelocityInLocalSpace(DefaultMovement->InitialSpeed > 0.f ? DefaultMovement->Velocity.GetSafeNormal() * DefaultMovement->InitialSpeed : DefaultMovement->Velocity);
}

AActorPoolProjectile::AActorPoolProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AActorPoolProjectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

void AActorPoolProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	Release();
}
