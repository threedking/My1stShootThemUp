// My 1st Unreal Shoot Them Up Game

#include "Player/STUPlayerController.h"
#include "Components/STURespawnComponent.h"

ASTUPlayerController::ASTUPlayerController() 
{
	this->RespawnComponent = this->CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}

void ASTUPlayerController::OnPossess(APawn* InPawn) 
{
    Super::OnPossess(InPawn);

	this->OnNewPawn.Broadcast(InPawn);
}