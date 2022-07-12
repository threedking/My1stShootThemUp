// My 1st Unreal Shoot Them Up Game

#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "AIController.h"
#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase() 
{
    this->DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    this->PlayerControllerClass = ASTUPlayerController::StaticClass();
    this->HUDClass = ASTUGameHUD::StaticClass();
    this->PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay() 
{
    Super::StartPlay();

    this->SpawnBots();
    this->CreateTeamsInfo();

    this->CurrentRound = 1;
    this->StartRound();
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController) 
{
    if (InController && InController->IsA<AAIController>())
    {
        return this->AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::SpawnBots() 
{
    if(!this->GetWorld()) return;

    for(int32 i = 0; i < this->GameData.PlayersNum - 1; i++)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto STUAIControler = this->GetWorld()->SpawnActor<AAIController>(this->AIControllerClass, SpawnInfo);
        this->RestartPlayer(STUAIControler);
    }
}

void ASTUGameModeBase::StartRound() 
{
    this->RoundCountDown = this->GameData.RoundTime;
    this->GetWorldTimerManager().SetTimer(this->GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdata, 1.0f, true);

}
void ASTUGameModeBase::GameTimerUpdata()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i  Round: i%/%i"), this->RoundCountDown, this->CurrentRound, this->GameData.RoundsNum);

    //const auto TimerRate = this->GetWorldTimerManager().GetTimerRate(this->GameRoundTimerHandle);
    //this->RoundCountDown -= static_cast<int32>(TimerRate);

    if(--this->RoundCountDown == 0)
    {
        this->GetWorldTimerManager().ClearTimer(this->GameRoundTimerHandle);
        
        if(this->CurrentRound + 1 <= this->GameData.RoundsNum)
        {
            ++this->CurrentRound;
            this->ResetPlayers();
            this->StartRound();
        }
        else
        {
            UE_LOG(LogSTUGameModeBase, Display, TEXT("=======GAME OVER======="));
        }

    }
}


void ASTUGameModeBase::ResetPlayers()
{
    if(!this->GetWorld()) return;

    for(auto It = this->GetWorld()->GetControllerIterator(); It; ++It)
    {
        this->ResetOnePlayer(It->Get());
    }
}
void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if(Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    this->RestartPlayer(Controller);
    this->SetPlayerColor(Controller);
}


void ASTUGameModeBase::CreateTeamsInfo()
{
    if(!this->GetWorld()) return;

    int32 TeamID = 1;
    for(auto It = this->GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if(!Controller) continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!PlayerState) continue;

        PlayerState->SetTeamID(TeamID);
        FLinearColor TeamColor = this->DetemineColorByTeamID(TeamID);
        PlayerState->SetTeamColor(TeamColor);
        this->SetPlayerColor(Controller);

        TeamID = TeamID == 1 ? 2 : 1;
    }

}

FLinearColor ASTUGameModeBase::DetemineColorByTeamID(int32 TeamID) const
{
    if(TeamID - 1 < this->GameData.TeamColors.Num() && TeamID - 1 > -1)
    {
        return this->GameData.TeamColors[TeamID - 1];
    }
    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i"), TeamID);
    return this->GameData.DefaultColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if(!Character) return;
        
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if(!PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());

}

