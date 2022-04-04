// My 1st Unreal Shoot Them Up Game


#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"


ASTUGameModeBase::ASTUGameModeBase() 
{
    this->DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    this->PlayerControllerClass = ASTUPlayerController::StaticClass();
    this->HUDClass = ASTUGameHUD::StaticClass();
}