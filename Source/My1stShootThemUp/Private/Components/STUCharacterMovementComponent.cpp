// My 1st Unreal Shoot Them Up Game


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(this->GetPawnOwner());
    return Player && Player->GetRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}