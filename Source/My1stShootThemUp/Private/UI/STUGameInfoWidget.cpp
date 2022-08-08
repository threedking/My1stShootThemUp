// My 1st Unreal Shoot Them Up Game

#include "UI/STUGameInfoWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameInfoWidget, All, All);

int32 USTUGameInfoWidget::GetKillsNum() const
{
    const auto PlayerState = this->GetSTUPlayerState();
    
    return PlayerState ? PlayerState->GetKillsNum() : 0;
}
int32 USTUGameInfoWidget::GetCurrentRound() const 
{
    const auto GameMode = this->GetSTUGameMode();
    return GameMode ? GameMode->GetCurrentRoundNum() : 0;
}
int32 USTUGameInfoWidget::GetRoundsNum() const 
{
    const auto GameMode = this->GetSTUGameMode();
    return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}
int32 USTUGameInfoWidget::GetRoundSecondsRemaining() const 
{
    const auto GameMode = this->GetSTUGameMode();
    return GameMode ? GameMode->GetRoundSecondsRemaining() : 0;
}

ASTUGameModeBase* USTUGameInfoWidget::GetSTUGameMode() const
{
    return this->GetWorld() ? Cast<ASTUGameModeBase>(this->GetWorld()->GetAuthGameMode()) : nullptr;
}
ASTUPlayerState* USTUGameInfoWidget::GetSTUPlayerState() const 
{
    return this->GetOwningPlayer() ? Cast<ASTUPlayerState>(this->GetOwningPlayer()->PlayerState) : nullptr;
}
