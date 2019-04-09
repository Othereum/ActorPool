// Copyright (c) 2019, Seokjin Lee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoolActor.generated.h"

UCLASS()
class ACTORPOOL_API APoolActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APoolActor() = default;

	// Returns this actor to the pool. You should use this function instead of the Destroy.
	UFUNCTION(BlueprintCallable, Category = PoolActor)
	void ReturnToPool();

protected:
	// Called when reused. It will not be called at first use. See BeginPlay in this case.
	UFUNCTION(BlueprintImplementableEvent, Category = PoolActor, DisplayName = BeginReuse)
	void BP_BeginReuse();
	virtual void BeginReuse() {};

private:
	friend class UActorPoolComponent;
	void StartUsing(UActorPoolComponent* InPool);
	bool HandleCollision();

	UActorPoolComponent* Pool;
};
