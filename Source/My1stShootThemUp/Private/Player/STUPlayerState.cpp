// My 1st Unreal Shoot Them Up Game

#include "Player/STUPlayerState.h"


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