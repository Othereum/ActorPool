// Copyright (c) 2019, Seokjin Lee

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPoolComponent.generated.h"

struct FActorSpawnParameters;
class APoolActor;

UCLASS(meta = (BlueprintSpawnableComponent))
class ACTORPOOL_API UActorPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = ActorPoolComponent)
	APoolActor* SpawnActor(const FTransform& Transform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner, APawn* Instigator);
	APoolActor* SpawnActor(const FTransform& Transform, const FActorSpawnParameters& Param = DefaultSpawnParameters);
	APoolActor* SpawnActor(const FVector& Location, const FRotator& Rotation, const FActorSpawnParameters& Param = DefaultSpawnParameters);
	APoolActor* SpawnActor(const FActorSpawnParameters& Param = DefaultSpawnParameters);

	template<class T>
	T* SpawnActor(const FTransform& Transform, const FActorSpawnParameters& Param = DefaultSpawnParameters)
	{
		return CastChecked<T>(SpawnActor(Transform, Param));
	}

	template<class T>
	T* SpawnActor(const FVector& Location, const FRotator& Rotation, const FActorSpawnParameters& Param = DefaultSpawnParameters)
	{
		return CastChecked<T>(SpawnActor(Location, Rotation, Param));
	}

	template<class T>
	T* SpawnActor(const FActorSpawnParameters& Param = DefaultSpawnParameters)
	{
		return CastChecked<T>(SpawnActor(Param));
	}

	void SetDefaultActorClass(const TSubclassOf<APoolActor>& Class);

	UActorPoolComponent() = default;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APoolActor> ActorClass;

	friend class APoolActor;
	void ReturnActor(APoolActor* Actor);

	TArray<APoolActor*> AvailableActors;

	static const FActorSpawnParameters DefaultSpawnParameters;
};
