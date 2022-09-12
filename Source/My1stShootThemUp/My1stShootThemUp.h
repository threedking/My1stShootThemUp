// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGASAbilityInputID: uint8
{
	None,
	Confirm,
	Cancel, //This 3 allways need
	Dash
};