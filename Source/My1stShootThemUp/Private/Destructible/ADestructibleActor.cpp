// My 1st Unreal Shoot Them Up Game

#include "Destructible/ADestructibleActor.h"
#include "DestructibleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(DestructibleActorLog, All, All);

AADestructibleActor::AADestructibleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");


	DestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>("Destructible Component");
    DestructibleComponent->SetupAttachment(RootComponent);
    
    //DestructibleComponent->OnComponentDestroyed.AddDynamic(this, &AADestructibleActor::Destroy);
}

void AADestructibleActor::BeginPlay()
{
	Super::BeginPlay();
}


void AADestructibleActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	if (DestructibleComponent)
    {
        UE_LOG(DestructibleActorLog, Display, TEXT("DestructibleComponent exist"));
    }
    else
    {
        UE_LOG(DestructibleActorLog, Display, TEXT("DestructibleComponent NOT exist"));
    }
}