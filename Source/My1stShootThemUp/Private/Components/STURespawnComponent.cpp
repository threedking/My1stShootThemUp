// My 1st Unreal Shoot Them Up Game

#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
    if(!this->GetWorld()) return;

    const auto GameMode = Cast<ASTUGameModeBase>(this->GetWorld()->GetAuthGameMode());
    if (!GameMode || !GameMode->IsCanRespawn()) return;

	this->RespawnCountDown = RespawnTime;
    this->GetWorld()->GetTimerManager().SetTimer(this->RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f, true);
}

void USTURespawnComponent::RespawnTimerUpdate()
{
    if(--this->RespawnCountDown == 0)
    {
        if(!this->GetWorld()) return;

        this->GetWorld()->GetTimerManager().ClearTimer(this->RespawnTimerHandle);

        const auto GameMode = Cast<ASTUGameModeBase>(this->GetWorld()->GetAuthGameMode());
        if(!GameMode) return;

        GameMode->RespawnRequest(Cast<AController>(this->GetOwner()));
    }
}


int32 USTURespawnComponent::GetRespawnCountDown() const 
{
    return this->RespawnCountDown;
}
bool USTURespawnComponent::IsRespawnInProgress() const 
{
    return this->GetWorld() && this->GetWorld()->GetTimerManager().IsTimerActive(this->RespawnTimerHandle);
}