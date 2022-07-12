// My 1st Unreal Shoot Them Up Game

#include "AI/STUAIBaseCharacter.h"             
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"

ASTUAIBaseCharacter::ASTUAIBaseCharacter(const FObjectInitializer& ObjInit) 
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    this->AutoPossessAI = EAutoPossessAI::Disabled;
    this->AIControllerClass = ASTUAIController::StaticClass();

    if (this->GetCharacterMovement())
    {
        this->bUseControllerRotationYaw = false;
        this->GetCharacterMovement()->bUseControllerDesiredRotation = true;
        this->GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}


void ASTUAIBaseCharacter::OnDeath()
{
    Super::OnDeath();

    const auto STUController = Cast<AAIController>(this->Controller);
    if (STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
}