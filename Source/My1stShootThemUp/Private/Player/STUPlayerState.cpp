// My 1st Unreal Shoot Them Up Game

#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All);

void ASTUPlayerState::SetTeamID(int32 ID)
{
    this->TeamID = ID;
}

int32 ASTUPlayerState::GetTeamID() const
{
    return this->TeamID;
}

void ASTUPlayerState::SetTeamColor(FLinearColor& Color)
{
    this->TeamColor = Color;
}

FLinearColor ASTUPlayerState::GetTeamColor() const
{
    return this->TeamColor;
}

void ASTUPlayerState::AddKill()
{
    ++this->KillsNum;
}
void ASTUPlayerState::AddDeath()
{
    ++this->DeathsNum;
}
int32 ASTUPlayerState::GetKillsNum()
{
    return this->KillsNum;
}
int32 ASTUPlayerState::GetDeathsNum()
{
    return this->DeathsNum;
}

void ASTUPlayerState::LogInfo()
{
    UE_LOG(LogSTUPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), this->TeamID, this->KillsNum, this->DeathsNum);
}