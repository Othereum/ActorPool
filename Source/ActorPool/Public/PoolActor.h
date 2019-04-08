// Copyright (c) 2019, Seokjin Lee

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

	UFUNCTION(BlueprintCallable, Category = PoolActor)
	void ReturnToPool();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = PoolActor, DisplayName = BeginUse)
	void BP_BeginReuse();
	virtual void BeginReuse() {};

private:
	friend class UActorPoolComponent;
	void StartUsing(UActorPoolComponent* InPool);

	UActorPoolComponent* Pool;
};
