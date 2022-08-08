// My 1st Unreal Shoot Them Up Game


#include "AI/STUAIController.h"
#include "AI/STUAIBaseCharacter.h"
#include "Components/STUAIPerceptionComponent.h"
#include "Components/STURespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTUAIController::ASTUAIController()
{
    this->STUAIPerceptionComponent = this->CreateDefaultSubobject<USTUAIPerceptionComponent>("STUPerceptionComponent");
    this->SetPerceptionComponent(*this->STUAIPerceptionComponent);

    this->RespawnComponent = this->CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");


    this->bWantsPlayerState = true;
}

void ASTUAIController::OnPossess(APawn* InPawn) 
{
    Super::OnPossess(InPawn);

    const auto STUCharacter = Cast<ASTUAIBaseCharacter>(InPawn);
    if (STUCharacter)
    {
        this->RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
    }

}

void ASTUAIController::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    //const auto AimActor = this->STUAIPerceptionComponent->GetClosestEnemy();
    const auto AimActor = this->GetFocusOnActor();
    this->SetFocus(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const 
{
    if (!this->GetBlackboardComponent())
    {
        return nullptr;
    }
    else {
        return Cast<AActor>(this->GetBlackboardComponent()->GetValueAsObject(this->FocusOnKeyName));
    }
        
}