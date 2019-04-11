// Copyright (c) 2019, Seokjin Lee. All rights reserved.

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
	if (!ActorClass) return nullptr;

	UWorld* const World = GetWorld();
	if (!World) return nullptr;

	const AActor* const Template = Param.Template ? Param.Template : ActorClass->GetDefaultObject<AActor>();

	const ESpawnActorCollisionHandlingMethod CollisionHandlingMethod = SelectCollisionHandlingMethod(Param, Template);
	if (CollisionHandlingMethod == ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding && EncroachingBlockingGeometry(Template, Transform))
	{
		return nullptr;
	}

	APoolActor* NewActor;
	if (AvailableActors.Num() > 0)
	{
		NewActor = AvailableActors.Last();
		NewActor->SetActorLocationAndRotation(Transform.GetLocation(), Transform.GetRotation());
		const USceneComponent* const TemplateRoot = Template->GetRootComponent();
		if (TemplateRoot)
		{
			NewActor->SetActorScale3D(TemplateRoot->RelativeScale3D * Transform.GetScale3D());
		}

		NewActor->SpawnCollisionHandlingMethod = CollisionHandlingMethod;
		const bool bCanSpawn = NewActor->HandleCollision();
		if (!bCanSpawn) return nullptr;

		AvailableActors.RemoveAt(AvailableActors.Num() - 1, 1, false);
	}
	else
	{
		NewActor = static_cast<APoolActor*>(World->SpawnActor(ActorClass, &Transform, Param));
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

ESpawnActorCollisionHandlingMethod UActorPoolComponent::SelectCollisionHandlingMethod(const FActorSpawnParameters& Param, const AActor* Template)
{
	ESpawnActorCollisionHandlingMethod CollisionHandlingOverride = Param.SpawnCollisionHandlingOverride;

	if (Param.bNoFail)
	{
		if (CollisionHandlingOverride == ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding)
		{
			CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		}
		else if (CollisionHandlingOverride == ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding)
		{
			CollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		}
	}

	return (CollisionHandlingOverride == ESpawnActorCollisionHandlingMethod::Undefined) ? Template->SpawnCollisionHandlingMethod : CollisionHandlingOverride;
}

bool UActorPoolComponent::EncroachingBlockingGeometry(const AActor* Template, const FTransform& UserTransform)
{
	USceneComponent* const TemplateRootComponent = Template->GetRootComponent();

	FTransform const FinalRootComponentTransform =
		TemplateRootComponent
		? FTransform{ TemplateRootComponent->RelativeRotation, TemplateRootComponent->RelativeLocation, TemplateRootComponent->RelativeScale3D } * UserTransform
		: UserTransform;

	FVector const FinalRootLocation = FinalRootComponentTransform.GetLocation();
	FRotator const FinalRootRotation = FinalRootComponentTransform.Rotator();

	return GetWorld()->EncroachingBlockingGeometry(Template, FinalRootLocation, FinalRootRotation);
}

const FActorSpawnParameters UActorPoolComponent::DefaultSpawnParameters;
