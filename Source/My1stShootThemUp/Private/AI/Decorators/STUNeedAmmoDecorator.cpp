// My 1st Unreal Shoot Them Up Game

#include "AI/Decorators/STUNeedAmmoDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"
#include "STUCoreTypes.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator() {
    this->NodeName = "Need Ammo";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
        return false;

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
    if (!WeaponComponent)
        return false;

    FAmmoData AmmoData; //Декоратор слишком много знает об оружии?
    if (WeaponComponent->GetChosenAmmoData(this->WeaponClass, AmmoData))
    {
        return !AmmoData.Infinite && AmmoData.Clips <= 0;
    }    

    return false;
}