// My 1st Unreal Shoot Them Up Game

#include "UI/STUSpectatorWidget.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(this->GetOwningPlayer());
    if(!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

    CountDownTime = RespawnComponent->GetRespawnCountDown();
    return true;
}