// My 1st Unreal Shoot Them Up Game

#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const 
{
    TArray<AActor*> PerceptionActors;
    this->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceptionActors);

    if (PerceptionActors.Num() == 0)
        return nullptr;

    const auto Controller = Cast<AAIController>(this->GetOwner());
    if (!Controller)
        return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
        return nullptr;

    float BestDistance{MAX_FLT};
    AActor* BestPawn{nullptr};

    for (const auto PerceptionActor : PerceptionActors)
    {
        const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PerceptionActor);
        
        const auto PerceptionPawn = Cast<APawn>(PerceptionActor);
        const bool AreEnemies = PerceptionPawn && STUUtils::AreEnemies(Controller, PerceptionPawn->Controller);
        
        if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
        {
            const auto CurrentDistance = (PerceptionActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PerceptionActor;
            }
        }
    }

    return BestPawn;
}