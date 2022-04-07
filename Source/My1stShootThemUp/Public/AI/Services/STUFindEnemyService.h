// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUFindEnemyService.generated.h"

/**
 * 
 */
UCLASS()
class MY1STSHOOTTHEMUP_API USTUFindEnemyService : public UBTService
{
	GENERATED_BODY()
	
public:
    USTUFindEnemyService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
