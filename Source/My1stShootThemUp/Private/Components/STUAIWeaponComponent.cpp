// My 1st Unreal Shoot Them Up Game

#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::StartFire() 
{
    if (!this->CanFire())
        return;

    if (this->CurrentWeapon->IsAmmoEmpty())
    {
        this->NextWeapon();
    }
    else
    {
        this->CurrentWeapon->StartFire();
    }
}

void USTUAIWeaponComponent::NextWeapon() 
{
    if (!this->CanEquip())
        return;

    int32 NextIndex = (this->CurrentWeaponIndex + 1) % this->Weapons.Num();
    while (NextIndex != this->CurrentWeaponIndex)
    {
        if (!this->Weapons[NextIndex]->IsAmmoEmpty())
        {
            break;
        }
        NextIndex = (NextIndex + 1) % this->Weapons.Num();
    }

    if (NextIndex != this->CurrentWeaponIndex)
    {
        this->CurrentWeaponIndex = NextIndex;
        this->EquipWeapon(this->CurrentWeaponIndex);
    }
}