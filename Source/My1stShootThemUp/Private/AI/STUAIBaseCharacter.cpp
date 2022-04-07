// My 1st Unreal Shoot Them Up Game

#include "AI/STUAIBaseCharacter.h"             
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTUAIBaseCharacter::ASTUAIBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    this->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    this->AIControllerClass = ASTUAIController::StaticClass();

    if (this->GetCharacterMovement())
    {
        this->bUseControllerRotationYaw = false;
        this->GetCharacterMovement()->bUseControllerDesiredRotation = true;
        this->GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}