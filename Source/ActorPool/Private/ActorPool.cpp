// Copyright 2019 Seokjin Lee. All Rights Reserved.

#include "ActorPool.h"
#include "Engine/World.h"
#include "PoolActor.h"

DEFINE_LOG_CATEGORY_STATIC(LogActorPool, Log, All)

APoolActor* AActorPool::Spawn(const TSubclassOf<APoolActor> Class, const FTransform& Transform, const FActorSpawnParameters& SpawnParameters)
{
	const auto SpawnImpl = [&]()->APoolActor*
	{
		if (!Class)
		{
			UE_LOG(LogActorPool, Error, TEXT(__FUNCTION__" : Failed to spawn actor. Class was nullptr."));
			return nullptr;
		}

		if (const auto PoolPtr = Pool.Find(Class))
		{
			while (PoolPtr->Num() > 0)
			{
				if (const auto Actor = PoolPtr->Pop().Get())
				{
					Actor->SetOwner(SpawnParameters.Owner);
					Actor->Instigator = SpawnParameters.Instigator;
					Actor->SetActorTransform(GetDefault<AActor>(Class)->GetRootComponent()->GetRelativeTransform() * Transform);
					Actor->Activate();
					return Actor;
				}
			}
		}

		if (const auto Actor = static_cast<APoolActor*>(GetWorld()->SpawnActor(Class, &Transform, SpawnParameters)))
		{
			UE_LOG(LogActorPool, Warning,
			       TEXT("%s에 대한 액터 풀이 비어있어 새 액터를 생성합니다. 해당 클래스를 Reserve에 등록하길 권장합니다. 현재까지 최대 사용량: %d개"),
			       *Class->GetName(), MaxUse.FindOrAdd(Class) + 1);
			Actor->SetPool(this);
			Actor->Activate();
			return Actor;
		}

		return nullptr;
	};

	const auto Actor = SpawnImpl();
#if !UE_BUILD_SHIPPING
	if (Actor) UpdateMaxUse(Class, ++CurUse.FindOrAdd(Class));
#endif
	return Actor;
}

void AActorPool::Release(APoolActor* Actor)
{
	const auto Class = Actor->GetClass();
	Pool.FindOrAdd(Class).Add(Actor);
#if !UE_BUILD_SHIPPING
	UpdateMaxUse(Class, --CurUse.FindOrAdd(Class));
#endif
}

void AActorPool::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Parameters;
	Parameters.Owner = this;
	Parameters.bNoFail = true;

	for (const auto& Pair : Reserve)
	{
		Pool.FindOrAdd(Pair.Key).Reserve(Pair.Value);

		for (auto i = 0; i < Pair.Value; ++i)
		{
			if (const auto Actor = static_cast<APoolActor*>(GetWorld()->SpawnActor(Pair.Key, &FTransform::Identity, Parameters)))
			{
				Actor->SetPool(this);
				Actor->Release(true);
			}
		}

#if !UE_BUILD_SHIPPING
		if (const auto P = CurUse.Find(Pair.Key)) *P = 0;
		if (const auto P = MaxUse.Find(Pair.Key)) *P = 0;
#endif
	}
}

#if !UE_BUILD_SHIPPING
void AActorPool::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (const auto& Pair : MaxUse)
	{
		UE_LOG(LogActorPool, Log, TEXT("%s 최대 동시 사용: %hhu개"), *Pair.Key->GetName(), Pair.Value);
	}
}

void AActorPool::UpdateMaxUse(const TSubclassOf<APoolActor> Class, const uint8 Cur)
{
	auto& Max = MaxUse.FindOrAdd(Class);
	if (Max < Cur) Max = Cur;
}
#endif