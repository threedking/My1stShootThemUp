// My 1st Unreal Shoot Them Up Game

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
 	PrimaryActorTick.bCanEverTick = false;

	this->WeaponMesh = this->CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    this->SetRootComponent(this->WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(this->WeaponMesh);
    checkf(this->DefaultAmmo.Bullets > 0, TEXT("Bullets count couldnt be <= 0"));
    checkf(this->DefaultAmmo.Clips > 0, TEXT("Clips count couldnt be <= 0"));
    this->CurrentAmmo = this->DefaultAmmo;
}

void ASTUBaseWeapon::StartFire() 
{
}

void ASTUBaseWeapon::StopFire() 
{
}

void ASTUBaseWeapon::MakeShot()
{}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(this->GetOwner());
    if (!Player)
    {
        return nullptr;
    }   
    return Player->GetController<APlayerController>();
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return this->WeaponMesh->GetSocketLocation(this->MuzzleSocketName);
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto STUCharacter = Cast<ACharacter>(this->GetOwner());
    if (!STUCharacter)
        return false;

    if (STUCharacter->IsPlayerControlled())
    {
        const auto Controller = this->GetPlayerController();
        if (!Controller)
            return false;

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    }
    else
    {
        ViewLocation = this->GetMuzzleWorldLocation();
        ViewRotation = this->WeaponMesh->GetSocketRotation(this->MuzzleSocketName);
    }


    return true;
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;

    bool isPlayerViewPontOk = this->GetPlayerViewPoint(ViewLocation, ViewRotation);
    if (!isPlayerViewPontOk)
        return false;


    // const FTransform SocketTransform = this->WeaponMesh->GetSocketTransform(this->MuzzleSocketName);
    TraceStart = ViewLocation; // SocketTransform.GetLocation();
    
    //const auto HalfRad = FMath::DegreesToRadians(this->BulletSpread);
    //const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad); // SocketTransform.GetRotation().GetForwardVector();

    const FVector ShootDirection = ViewRotation.Vector(); // SocketTransform.GetRotation().GetForwardVector();
    
    TraceEnd = TraceStart + ShootDirection * this->TraceMaxDistance;
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!this->GetWorld())
        return;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this->GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;    

    this->GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}


void ASTUBaseWeapon::DecreaseAmmo() 
{
    if (this->CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Clip is empty"));
        return;
    }
    this->CurrentAmmo.Bullets--;
    //this->LogAmmo();

    if (this->IsClipEmpty() && !this->IsAmmoEmpty())
    {
        this->StopFire();
        this->OnClipEmpty.Broadcast(this);
    }
}
bool ASTUBaseWeapon::IsAmmoEmpty() const 
{
    return !this->CurrentAmmo.Infinite && this->CurrentAmmo.Clips == 0 && this->IsClipEmpty();
}
bool ASTUBaseWeapon::IsClipEmpty() const 
{
    return this->CurrentAmmo.Bullets == 0;
}
void ASTUBaseWeapon::ChangeClip() 
{
    if (!this->CurrentAmmo.Infinite)
    {
        if (this->CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Display, TEXT("No more clips"));
            return;
        }
        this->CurrentAmmo.Clips--;
    }
    this->CurrentAmmo.Bullets = this->DefaultAmmo.Bullets;
   // UE_LOG(LogBaseWeapon, Display, TEXT("----Clip Changed-----"));
}

bool ASTUBaseWeapon::CanReload() const 
{
    return this->CurrentAmmo.Bullets<this->DefaultAmmo.Bullets&& this->CurrentAmmo.Clips> 0;
}
void ASTUBaseWeapon::LogAmmo() 
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(this->CurrentAmmo.Bullets) + " / ";
    AmmoInfo += this->CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(this->CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

FWeaponUIData ASTUBaseWeapon::GetUIData() const 
{
    return this->UIData;
}

FAmmoData ASTUBaseWeapon::GetAmmoData() const 
{
    return this->CurrentAmmo;
}

bool ASTUBaseWeapon::IsAmmoFull() const 
{
    return this->CurrentAmmo.Clips == this->DefaultAmmo.Clips && //
           this->CurrentAmmo.Bullets == this->DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount) 
{
    if(this->CurrentAmmo.Infinite || this->IsAmmoFull() || ClipsAmount <= 0)
        return false;
    
    if (this->IsAmmoEmpty())
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Ammo Was Empty"));
        this->CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, this->DefaultAmmo.Clips + 1);
        this->OnClipEmpty.Broadcast(this);
    }
    else if (this->CurrentAmmo.Clips < this->DefaultAmmo.Clips)
    {
        const auto NextClipsAmount = this->CurrentAmmo.Clips + ClipsAmount;
        if (this->DefaultAmmo.Clips - NextClipsAmount >= 0)
        {
            this->CurrentAmmo.Clips = NextClipsAmount;
            UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added"));
        }
        else 
        {
            this->CurrentAmmo.Clips = this->DefaultAmmo.Clips;
            this->CurrentAmmo.Bullets = this->DefaultAmmo.Bullets;
            UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is full now"));
        }
    }
    else
    {
        this->CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added"));
    }

    return true;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX() 
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(this->MuzzleFX, //
        this->WeaponMesh,//
        this->MuzzleSocketName,//
        FVector::ZeroVector,//
        FRotator::ZeroRotator,//
        EAttachLocation::SnapToTarget,//
        true);
}