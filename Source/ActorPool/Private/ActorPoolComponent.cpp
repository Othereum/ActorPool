// Copyright (c) 2019, Seokjin Lee

#include "ActorPoolComponent.h"
#include "Engine/World.h"
#include "PoolActor.h"

APoolActor* UActorPoolComponent::SpawnActor(const FTransform& Transform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner, APawn* Instigator)
{
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = CollisionHandlingOverride;
	Param.Owner = Owner;
	Param.Instigator = Instigator;
	return SpawnActor(Transform, Param);
}

APoolActor* UActorPoolComponent::SpawnActor(const FTransform& Transform, const FActorSpawnParameters& Param)
{
	APoolActor* NewActor;
	if (AvailableActors.Num() > 0)
	{
		NewActor = AvailableActors.Pop(false);
		NewActor->SetActorTransform({ Transform.GetRotation(), Transform.GetLocation(), ActorClass->GetDefaultObject<APoolActor>()->GetActorScale() * Transform.GetScale3D() });
	}
	else
	{
		NewActor = static_cast<APoolActor*>(GetWorld()->SpawnActor(ActorClass, &Transform, Param));
	}
	if (NewActor)
	{
		NewActor->StartUsing(this);
	}
	return NewActor;
}

APoolActor* UActorPoolComponent::SpawnActor(const FVector& Location, const FRotator& Rotation, const FActorSpawnParameters& Param)
{
	return SpawnActor({ Rotation, Location, FVector::OneVector }, Param);
}

APoolActor* UActorPoolComponent::SpawnActor(const FActorSpawnParameters& Param)
{
	return SpawnActor(FTransform::Identity, Param);
}

void UActorPoolComponent::SetDefaultActorClass(const TSubclassOf<APoolActor>& Class)
{
	if (!ActorClass)
	{
		ActorClass = Class;
	}
}

void UActorPoolComponent::ReturnActor(APoolActor* Actor)
{
	AvailableActors.Add(Actor);
}

const FActorSpawnParameters UActorPoolComponent::DefaultSpawnParameters;
