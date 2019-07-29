// Copyright 2019 Seokjin Lee. All Rights Reserved.

#include "PoolActor.h"
#include "ActorPool.h"

void APoolActor::Release(const bool bForce)
{
	if (!bActivated && !bForce) return;
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	bActivated = false;
	Pool->Release(this);
	OnReleased();
}

void APoolActor::Activate(const bool bForce)
{
	if (bActivated && !bForce) return;
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	bActivated = true;
	OnActivated();
}
