// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;

UCLASS()
class MY1STSHOOTTHEMUP_API ASTUAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    ASTUAIController();

protected:

    UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Components") 
    USTUAIPerceptionComponent* STUAIPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnKeyName = "EnemyActor";

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;

private:
    AActor* GetFocusOnActor() const;
};
