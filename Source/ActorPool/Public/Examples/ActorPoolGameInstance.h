// Copyright 2019 Seokjin Lee. All Rights Reserved.

#pragma once

#include "Engine/GameInstance.h"
#include "ActorPoolGameInstance.generated.h"

UCLASS()
class UActorPoolGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UActorPoolGameInstance();

	class AActorPool* GetActorPool();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TSubclassOf<AActorPool> ActorPoolClass;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	AActorPool* ActorPool;
};
