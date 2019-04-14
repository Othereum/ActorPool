// Copyright (c) 2019, Seokjin Lee. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPoolComponent.generated.h"

struct FActorSpawnParameters;
class APoolActor;

UCLASS(meta = (BlueprintSpawnableComponent))
class SAUCEWICH_API UActorPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Returns the actor waiting to be used. If no actors, creates a new one.
	UFUNCTION(BlueprintCallable, Category = ActorPoolComponent)
	APoolActor* SpawnActor(const FTransform& Transform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner, APawn* Instigator, bool& bOutReused);
	APoolActor* SpawnActor(const FTransform& Transform, const FActorSpawnParameters& Param = DefaultSpawnParameters, bool* bOutReused = nullptr);
	APoolActor* SpawnActor(const FVector& Location, const FRotator& Rotation, const FActorSpawnParameters& Param = DefaultSpawnParameters, bool* bOutReused = nullptr);
	APoolActor* SpawnActor(const FActorSpawnParameters& Param = DefaultSpawnParameters, bool* bOutReused = nullptr);

	template<class T>
	T* SpawnActor(const FTransform& Transform, const FActorSpawnParameters& Param = DefaultSpawnParameters, bool* bOutReused = nullptr)
	{
		return CastChecked<T>(SpawnActor(Transform, Param, bOutReused), ECastCheckedType::NullAllowed);
	}

	template<class T>
	T* SpawnActor(const FVector& Location, const FRotator& Rotation, const FActorSpawnParameters& Param = DefaultSpawnParameters, bool* bOutReused = nullptr)
	{
		return CastChecked<T>(SpawnActor(Location, Rotation, Param, bOutReused), ECastCheckedType::NullAllowed);
	}

	template<class T>
	T* SpawnActor(const FActorSpawnParameters& Param = DefaultSpawnParameters, bool* bOutReused = nullptr)
	{
		return CastChecked<T>(SpawnActor(Param, bOutReused), ECastCheckedType::NullAllowed);
	}

	void SetDefaultActorClass(const TSubclassOf<APoolActor>& Class);

	UActorPoolComponent() = default;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APoolActor> ActorClass;

	friend class APoolActor;
	void ReturnActor(APoolActor* Actor);

	TArray<APoolActor*> AvailableActors;

	ESpawnActorCollisionHandlingMethod SelectCollisionHandlingMethod(const FActorSpawnParameters& Param, const AActor* Template);
	bool EncroachingBlockingGeometry(const AActor* Template, const FTransform& UserTransform);

	static const FActorSpawnParameters DefaultSpawnParameters;
};
