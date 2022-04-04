// My 1st Unreal Shoot Them Up Game


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
//#include "Kismet/GameplayStatics.h"
//#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponLauncher, All, All);

void ASTULauncherWeapon::StartFire() 
{
    this->MakeShot();
}

void ASTULauncherWeapon::MakeShot() 
{
    if (!this->GetWorld() || this->IsAmmoEmpty())
        return;

    FVector TraceStart, TraceEnd;
    if (!this->GetTraceData(TraceStart, TraceEnd))
        return;

    FHitResult HitResult;
    this->MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - this->GetMuzzleWorldLocation()).GetSafeNormal();


    const FTransform SpawnTransform(FRotator::ZeroRotator, this->GetMuzzleWorldLocation());
    ASTUProjectile* Projectile = this->GetWorld()->SpawnActorDeferred<ASTUProjectile>(this->ProjectileClass, SpawnTransform);
    //auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(this->GetWorld(), this->ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->FinishSpawning(SpawnTransform);
        Projectile->SetOwner(this->GetOwner());
        //UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
        UE_LOG(LogWeaponLauncher, Display, TEXT("BIG SHOOT!!!"));
    }

    this->DecreaseAmmo();
    this->SpawnMuzzleFX();
}
