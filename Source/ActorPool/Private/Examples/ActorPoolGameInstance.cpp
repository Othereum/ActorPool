// Copyright 2019 Seokjin Lee. All Rights Reserved.

#include "ActorPoolGameInstance.h"
#include "Engine/World.h"
#include "ActorPool.h"

UActorPoolGameInstance::UActorPoolGameInstance()
	:ActorPoolClass{AActorPool::StaticClass()}
{
}

AActorPool* UActorPoolGameInstance::GetActorPool()
{
	if (!ActorPool) ActorPool = static_cast<AActorPool*>(GetWorld()->SpawnActor(ActorPoolClass));
	return ActorPool;
}
