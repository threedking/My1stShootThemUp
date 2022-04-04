// My 1st Unreal Shoot Them Up Game

#include "Pickups/STUHelthPickup.h"
#include "Components/STUHealthComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHelthPickup, All, All);

bool ASTUHelthPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead())
        return false;

    return HealthComponent->TryToAddHealth(this->HelthAmount);
}