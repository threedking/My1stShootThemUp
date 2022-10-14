// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADestructibleActor.generated.h"

class UDestructibleComponent;

UCLASS()
class MY1STSHOOTTHEMUP_API AADestructibleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AADestructibleActor();

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Freeze")
    UDestructibleComponent* DestructibleComponent;

public:	

	virtual void Tick(float DeltaTime) override;
};
