// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MY1STSHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    void SetTeamID(int32 ID);
    int32 GetTeamID() const;

    void SetTeamColor(FLinearColor& Color);
    FLinearColor GetTeamColor() const;

    void AddKill();
    void AddDeath();
    int32 GetKillsNum();
    int32 GetDeathsNum();

    void LogInfo();

private:
    int32 TeamID;
    FLinearColor TeamColor;

    int32 KillsNum{0};
    int32 DeathsNum{0};

};
