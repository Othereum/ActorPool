// Copyright (c) 2019, Seokjin Lee

#include "PoolActor.h"
#include "Engine/World.h"
#include "ActorPoolComponent.h"

void APoolActor::ReturnToPool()
{
	if (Pool)
	{
		SetActorTickEnabled(false);
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
		Pool->ReturnActor(this);
	}
	else
	{
		Destroy();
	}
}

void APoolActor::StartUsing(UActorPoolComponent* InPool)
{
	if (Pool)
	{
		APoolActor* const DefaultActor = GetClass()->GetDefaultObject<APoolActor>();
		SetActorTickEnabled(DefaultActor->IsActorTickEnabled());
		SetActorHiddenInGame(DefaultActor->bHidden);
		BeginReuse();
		BP_BeginReuse();
	}
	else
	{
		Pool = InPool;
	}
}

bool APoolActor::HandleCollision()
{
	SetActorEnableCollision(GetClass()->GetDefaultObject<AActor>()->GetActorEnableCollision());

	switch (SpawnCollisionHandlingMethod)
	{
	case ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn:
	{
		FVector AdjustedLocation = GetActorLocation();
		FRotator AdjustedRotation = GetActorRotation();
		if (GetWorld()->FindTeleportSpot(this, AdjustedLocation, AdjustedRotation))
		{
			SetActorLocationAndRotation(AdjustedLocation, AdjustedRotation, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
	break;
	case ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding:
	{
		FVector AdjustedLocation = GetActorLocation();
		FRotator AdjustedRotation = GetActorRotation();
		if (GetWorld()->FindTeleportSpot(this, AdjustedLocation, AdjustedRotation))
		{
			SetActorLocationAndRotation(AdjustedLocation, AdjustedRotation, false, nullptr, ETeleportType::TeleportPhysics);
		}
		else
		{
			SetActorEnableCollision(false);
			return false;
		}
	}
	break;
	case ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding:
		if (GetWorld()->EncroachingBlockingGeometry(this, GetActorLocation(), GetActorRotation()))
		{
			SetActorEnableCollision(false);
			return false;
		}
		break;
	case ESpawnActorCollisionHandlingMethod::Undefined:
	case ESpawnActorCollisionHandlingMethod::AlwaysSpawn:
	default:
		break;
	}
	return true;
}
