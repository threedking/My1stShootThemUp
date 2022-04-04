// My 1st Unreal Shoot Them Up Game


#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"


float USTUPlayerHUDWidget::GetHealthPercent() const 
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(this->GetOwningPlayerPawn());

    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}


bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const 
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(this->GetOwningPlayerPawn());
    if (!WeaponComponent)   return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(this->GetOwningPlayerPawn());
    if (!WeaponComponent)   return false;

    return WeaponComponent->GetAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const {
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(this->GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead(); // контроллер захватит spectator pawn, поэтому IsDead никогда не вернет true
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const {
    const auto Controller = this->GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool USTUPlayerHUDWidget::Initialize() 
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(this->GetOwningPlayerPawn());

    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }    

    return Super::Initialize();
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        OnTakeDamage();
    }
}