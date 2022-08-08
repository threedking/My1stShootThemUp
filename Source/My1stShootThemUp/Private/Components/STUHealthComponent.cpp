// My 1st Unreal Shoot Them Up Game


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Dev/STUFireDamageType.h"
#include "Dev/STUIceDamageType.h"
#include "TimerManager.h"
#include "Camera/CameraShake.h"
#include "STUGameModeBase.h"


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)



USTUHealthComponent::USTUHealthComponent()
{
	// Set this component to be initialized when the game starts, 
	// and to be ticked every frame.  
	// You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(this->MaxHealth > 0);

    this->SetHealth(this->MaxHealth);

    AActor* ComponentOwner = this->GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandle);
        
    }

}

float USTUHealthComponent::GetHealth() const
{
    return this->Health;
}

bool USTUHealthComponent::IsDead() const
{
    return FMath::IsNearlyZero(this->Health);
}


void USTUHealthComponent::OnTakeAnyDamageHandle(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || this->IsDead())
    {
        return;
    }


    this->SetHealth(this->Health - Damage);
    

    if (this->AutoHealIsOn && this->Health < this->MaxHealth)
    {
        AActor* ComponentOwner = this->GetOwner();

        if (ComponentOwner->GetWorldTimerManager().TimerExists(this->HealTimer))
        {
            ComponentOwner->GetWorldTimerManager().ClearTimer(this->HealTimer);
        }
        ComponentOwner->GetWorldTimerManager().SetTimer(
            this->HealTimer, this, &USTUHealthComponent::OnHealTimerFired, this->DelayBeforeHeal, true);
        this->IsHealing = false;

    }
    
    if (this->IsDead())
    {
        AActor* ComponentOwner = this->GetOwner();
        ComponentOwner->GetWorldTimerManager().ClearTimer(this->HealTimer);

        this->Killed(InstigatedBy);
        this->OnDeath.Broadcast();
    }

    UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

    if (DamageType)
    {
        if (DamageType->IsA<USTUFireDamageType>())
        {
            UE_LOG(LogHealthComponent, Display, TEXT("Damage is FIRE It's HOOOOOOOT"));
      
        }else if (DamageType->IsA<USTUIceDamageType>())
        {
            UE_LOG(LogHealthComponent, Display, TEXT("Damage is Ice It's Coooooooooold"));
        }
    }

    this->PlayCameraShake();
}

/*
// Called every frame
void USTUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
*/

void USTUHealthComponent::OnHealTimerFired() {

    if (this->Health < this->MaxHealth)
    {
        if (this->IsHealing)
        {
            this->SetHealth(this->Health + this->HealModifier);
        }
        else
        {
            AActor* ComponentOwner = this->GetOwner();
            ComponentOwner->GetWorldTimerManager().ClearTimer(this->HealTimer);
            ComponentOwner->GetWorldTimerManager().SetTimer(
                this->HealTimer, this, &USTUHealthComponent::OnHealTimerFired, this->HealEverySecTime, true);
            this->IsHealing = true;
        }
    }

    if (this->Health >= this->MaxHealth)
    {
        AActor* ComponentOwner = this->GetOwner();
        ComponentOwner->GetWorldTimerManager().ClearTimer(this->HealTimer);
        this->IsHealing = false;
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, this->MaxHealth);
    const auto HealthDelta = NextHealth - this->Health;

    this->Health = NextHealth;
    this->OnHealthChanged.Broadcast(this->Health, HealthDelta);
}

float USTUHealthComponent::GetHealthPercent() const 
{
    return this->Health / this->MaxHealth;
}

bool USTUHealthComponent::IsHealthFull()
{
    return FMath::IsNearlyEqual(this->Health, this->MaxHealth);
}

bool USTUHealthComponent::TryToAddHealth(int32 HelthAmount) {
    if (this->IsDead() || this->IsHealthFull())
        return false;

    this->SetHealth(this->Health + HelthAmount);
    return true;
}

void USTUHealthComponent::PlayCameraShake() 
{
    if (this->IsDead())
        return;

    const auto Player = Cast<APawn>(this->GetOwner());
    if (!Player)
        return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager)
        return;

    Controller->PlayerCameraManager->StartCameraShake(this->CameraShake);
}

void USTUHealthComponent::Killed(AController* KillerController)
{
    if(!GetWorld()) return;

    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if(!GameMode) return;

    const auto Player = Cast<APawn>(this->GetOwner());
    const auto VictimController = Player ? Player->Controller : nullptr;

    GameMode->Killed(KillerController, VictimController);
}