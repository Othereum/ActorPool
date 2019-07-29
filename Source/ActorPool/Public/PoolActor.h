// Copyright 2019 Seokjin Lee. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "PoolActor.generated.h"

UCLASS()
class APoolActor : public AActor
{
	GENERATED_BODY()
	
public:
	void Release(bool bForce = false);
	void Activate(bool bForce = false);
	void LifeSpanExpired() override { Release(); }
	void SetPool(class AActorPool* NewPool) { Pool = NewPool; }

protected:
	virtual void OnReleased() {}
	virtual void OnActivated() {}

private:
	AActorPool* Pool;
	uint8 bActivated : 1;
};
