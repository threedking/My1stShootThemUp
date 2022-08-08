// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGameInfoWidget.generated.h"

class ASTUGameModeBase;
class ASTUPlayerState;

UCLASS()
class MY1STSHOOTTHEMUP_API USTUGameInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetKillsNum() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetCurrentRound() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetRoundsNum() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetRoundSecondsRemaining() const;

private:
    ASTUGameModeBase* GetSTUGameMode() const;
    ASTUPlayerState* GetSTUPlayerState() const;	
};
