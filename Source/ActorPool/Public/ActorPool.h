// Copyright 2019 Seokjin Lee. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPool.generated.h"

UCLASS()
class AActorPool final : public AActor
{
	GENERATED_BODY()
	
public:
	class APoolActor* Spawn(TSubclassOf<APoolActor> Class, const FTransform& Transform, const struct FActorSpawnParameters& SpawnParameters);

	template <class T>
	T* Spawn(const TSubclassOf<APoolActor> Class, const FTransform& Transform, const struct FActorSpawnParameters& SpawnParameters)
	{
		static_assert(TIsDerivedFrom<T, APoolActor>::IsDerived, "");
		return Cast<T>(Spawn(Class, Transform, SpawnParameters));
	}

	void Release(APoolActor* Actor);

private:
	void BeginPlay() override;

	// 여기에 클래스와 개수를 등록해두면 게임 시작시 미리 생성됩니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TMap<TSubclassOf<APoolActor>, uint8> Reserve;
	TMap<TSubclassOf<APoolActor>, TArray<TWeakObjectPtr<APoolActor>>> Pool;

#if !UE_BUILD_SHIPPING
	// 액터 풀에 등록된 액터가 가장 많았을 때의 개수를 기억했다가 게임이 끝날 때 로그를 출력합니다.
	// 실제 게임 플레이 테스트를 하고 출력된 로그에 나오는 만큼 액터를 예약하면 됩니다.
	// 주의: 액터가 삭제되는 경우는 고려되지 않습니다. 삭제되지 않도록 주의하십시오.
	TMap<TSubclassOf<APoolActor>, uint8> MaxUse;
	TMap<TSubclassOf<APoolActor>, uint8> CurUse;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	void UpdateMaxUse(TSubclassOf<APoolActor> Class, uint8 Cur);
#endif
};
