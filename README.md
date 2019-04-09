# ActorPoolComponent

EN: This is the actor component that designed to apply [object pool patterns](https://en.wikipedia.org/wiki/Object_pool_pattern) easier to the actors of the Unreal Engine 4. You can also use it only with Blueprint. It's easy to get performance benefits. Please note that this includes an example C++ code and blueprints to help you understand how to use it.

KO: [객체 풀 패턴](https://en.wikipedia.org/wiki/Object_pool_pattern)을 언리얼 엔진 4의 액터에 쉽게 적용할 수 있게 하기 위해 제작된 액터 컴포넌트 입니다. 블루프린트만으로도 사용하실 수 있습니다. 성능상의 이득을 손쉽게 얻으실 수 있습니다. 사용법의 이해를 돕기 위한 C++ 코드 및 블루프린트 예제가 포함되어 있으니 참고하시기 바랍니다.

## Details

EN: There are two classes, ActorPoolComponent and PoolActor. Here's the scenario:

1. Add a ActorPoolComponent to the actor which you want to spawn the actor.
1. Lets the actor that you want to spawn inherit the PoolActor class.
1. Set the ActorClass property of the ActorPoolComponent to the class of the actor to be spawned.
1. Now, call the SpawnActor in the ActorPoolComponent. This is exactly the same usage as the original SpawnActor.
1. Call ReturnToPool instead of Destroy when you are done using the actor.

KO: 두 개의 클래스, 액터 풀 컴포넌트와 풀 액터가 있습니다. 사용 시나리오는 이렇습니다:

1. 액터를 스폰시킬 액터에 풀 컴포넌트를 추가합니다.
1. 스폰할 액터가 풀 액터 클래스를 상속하도록 합니다.
1. 풀 컴포넌트의 ActorClass 프로퍼티를 스폰할 액터의 클래스로 지정합니다.
1. 이제 풀 컴포넌트의 SpawnActor를 호출합니다. 이는 원래 사용하던 SpawnActor와 사용법이 완전히 동일합니다.
1. 사용이 끝난 액터는 Destory 대신에 ReturnToPool을 호출합니다.
