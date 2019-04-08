// Copyright (c) 2019, Seokjin Lee

#include "PoolActor.h"
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
		SetActorEnableCollision(DefaultActor->GetActorEnableCollision());
		SetActorHiddenInGame(DefaultActor->bHidden);
		BeginReuse();
		BP_BeginReuse();
	}
	else
	{
		Pool = InPool;
	}
}
