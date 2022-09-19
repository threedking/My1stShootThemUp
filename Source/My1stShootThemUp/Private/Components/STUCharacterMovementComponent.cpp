// My 1st Unreal Shoot Them Up Game


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(this->GetPawnOwner());

    return Player && Player->IsStuned() ? 0.0f : Player->GetRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}

FRotator USTUCharacterMovementComponent::GetDeltaRotation(float DeltaTime) const 
{
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(this->GetPawnOwner());
    return Player && Player->IsStuned() ? FRotator::ZeroRotator : Super::GetDeltaRotation(DeltaTime);
}