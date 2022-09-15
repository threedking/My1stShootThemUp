// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "STUCoreTypes.h"
#include "STUGameplayAbility.generated.h"

UCLASS()
class MY1STSHOOTTHEMUP_API USTUGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

    USTUGameplayAbility();

    //Abilities with this set will automatically activate when input is pressed
    UPROPERTY(EditAnywhere, Category = "Ability") //BlueprintReadWrite
    EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
};
