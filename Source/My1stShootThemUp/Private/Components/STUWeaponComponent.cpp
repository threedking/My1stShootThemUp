// My 1st Unreal Shoot Them Up Game

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"
#include "Player/STUBaseCharacter.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(this->WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);

    this->CurrentWeaponIndex = 0;
    this->InitAnamations();
    this->SpawnWeapons();
    this->EquipWeapon(this->CurrentWeaponIndex);

}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type ENdPlayReason) 
{
    this->CurrentWeapon = nullptr;
    for (auto Weapon : this->Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(ENdPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{

    if (!this->GetWorld())
    {
        return;
    }

    ACharacter* Character = Cast<ACharacter>(this->GetOwner());
    if (!Character)
    {
        return;
    }

    for (auto OneWeaponData : this->WeaponData)
    {

        auto Weapon = this->GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon)
        {
            continue;
        }
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(Character);
        this->Weapons.Add(Weapon);

        this->AttachWeaponToSocket(Weapon, Character->GetMesh(), this->WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName) 
{
    if (!Weapon || !SceneComponent) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex) 
{
    if (WeaponIndex < 0 || WeaponIndex >= this->Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Display, TEXT("Invalid weapon index"));
        return;
    }
    ACharacter* Character = Cast<ACharacter>(this->GetOwner());
    if (!Character)  return;

    if (this->CurrentWeapon)
    {
        this->CurrentWeapon->StopFire();
        this->AttachWeaponToSocket(this->CurrentWeapon, Character->GetMesh(), this->WeaponArmorySocketName);
    }

    this->CurrentWeapon = this->Weapons[WeaponIndex];
    //this->CurrentReloadAnimMontage = this->WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData =
        this->WeaponData.FindByPredicate([&](const FWeaponData& Data) 
            { return Data.WeaponClass == CurrentWeapon->GetClass(); });
    
    this->CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    this->AttachWeaponToSocket(this->CurrentWeapon, Character->GetMesh(), this->WeaponEqipSocketName);
    this->EquipAnimInProgress = true;
    this->PlayAnimMontage(this->EquipAnimMontage);
}

void USTUWeaponComponent::StartFire() 
{

    if (!this->CanFire())
    {
        return;
    }

    this->CurrentWeapon->StartFire();

}
void USTUWeaponComponent::StopFire()
{
    if(!CurrentWeapon) return;

    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon() 
{
    if (!this->CanEquip()) return;
    this->CurrentWeaponIndex = (this->CurrentWeaponIndex + 1) % this->Weapons.Num();
    this->EquipWeapon(this->CurrentWeaponIndex);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation) 
{
    ACharacter* Character = Cast<ACharacter>(this->GetOwner());
    if (!Character) return;
    
    Character->PlayAnimMontage(Animation);
}


void USTUWeaponComponent::InitAnamations() 
{
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(this->EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Eqip anim notify is forgotten to set"));
        checkNoEntry();
    }

    for (auto OneWeaponData : this->WeaponData)
    {
        auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify)
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
            checkNoEntry();
        }
        
        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
                
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(this->GetOwner());
    if (!Character)
        return;

    if (Character->GetMesh() == MeshComponent)
    {
        this->EquipAnimInProgress = false;
    }
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(this->GetOwner());
    if (!Character)
        return;

    if (Character->GetMesh() == MeshComponent)
    {
        this->ReloadAnimInProgress = false;
    }
}


bool USTUWeaponComponent::CanFire() const
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    return Player && !Player->IsStuned() && this->CurrentWeapon && !this->EquipAnimInProgress && !this->ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    return Player && !Player->IsStuned() && !this->EquipAnimInProgress && !this->ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    auto Player = Cast<ASTUBaseCharacter>(GetOwner());
    return Player && !Player->IsStuned() && 
        this->CurrentWeapon && 
        !this->EquipAnimInProgress && 
        !this->ReloadAnimInProgress &&
        this->CurrentWeapon->CanReload();
}

void USTUWeaponComponent::Reload() 
{
    this->ChangeClip();
}


void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon)
{
    if (!AmmoEmptyWeapon)
        return;

    if (this->CurrentWeapon == AmmoEmptyWeapon)
    {
        this->ChangeClip();
    }
    else
    {
        for (const auto Weapon: this->Weapons)
        {
            if (Weapon == AmmoEmptyWeapon)
            {
                Weapon->ChangeClip();
            }
        }
    }
}

void USTUWeaponComponent::ChangeClip() 
{
    if(!this->CanReload()) return;

    this->CurrentWeapon->StopFire();
    this->CurrentWeapon->ChangeClip();
    this->ReloadAnimInProgress = true;
    this->PlayAnimMontage(this->CurrentReloadAnimMontage);
}


bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
    if (this->CurrentWeapon)
    {
        UIData = this->CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetAmmoData(FAmmoData& AmmoData) const
{
    if (this->CurrentWeapon)
    {
        AmmoData = this->CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;    
}

bool USTUWeaponComponent::GetChosenAmmoData(TSubclassOf<ASTUBaseWeapon> WeaponType, FAmmoData& AmmoData) const
{
    for (auto& Weapon : this->Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            AmmoData = Weapon->GetAmmoData();
            return true;   
        }
    }
    return false;    
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for (const auto Weapon : this->Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

ASTUBaseWeapon* USTUWeaponComponent::GetCurrentWeapon()
{
    return CurrentWeapon;
}

void USTUWeaponComponent::DropAllWeapons(int LifeTime)
{
    for(auto Weapon : Weapons)
    {
        if(Weapon)
        {
            const FTransform SpawnTransform(Weapon->GetActorRotation(), Weapon->GetActorLocation());            
            auto newWeapon = GetWorld()->SpawnActorAbsolute<ASTUBaseWeapon>(Weapon->GetClass(), SpawnTransform);
            newWeapon->SetLifeSpan(LifeTime);

            auto Components = newWeapon->GetComponents();
            for (auto& Component : Components)
            {
                auto PrimitiveComponent = Cast<UPrimitiveComponent>(Component);
                if(PrimitiveComponent)
                {
                    PrimitiveComponent->SetSimulatePhysics(true);
                    PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
                }
            }

            Weapon->Destroy();
        }
    }
    CurrentWeapon = nullptr;
}