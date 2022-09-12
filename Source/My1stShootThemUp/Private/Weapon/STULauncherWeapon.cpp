// My 1st Unreal Shoot Them Up Game

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponLauncher, All, All);

void ASTULauncherWeapon::StartFire() 
{
    this->MakeShot();
}

void ASTULauncherWeapon::MakeShot() 
{
    if (!this->GetWorld() || this->IsAmmoEmpty())
        return;

    bool IsManualMod = true;

    FVector EndPoint;
    FVector SpeedVector;
    if (!GetTargetPointAndSpeedVector(EndPoint, SpeedVector))
        return;

    SpeedVector.Normalize();
    const FVector Direction = IsManualMod ? (EndPoint - this->GetMuzzleWorldLocation()).GetSafeNormal() : SpeedVector;
    
    const FTransform SpawnTransform(FRotator::ZeroRotator, this->GetMuzzleWorldLocation());
    ASTUProjectile* Projectile = this->GetWorld()->SpawnActorDeferred<ASTUProjectile>(this->ProjectileClass, SpawnTransform);
    //auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(this->GetWorld(), this->ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(this->GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
        //UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
        //UE_LOG(LogWeaponLauncher, Display, TEXT("BIG SHOOT!!!"));
    }

    this->DecreaseAmmo();
    this->SpawnMuzzleFX();
}

FVector ASTULauncherWeapon::GetPredictPointByTime(float FlyTime)
{
    FVector EndPoint;
    if(!GetTargetPoint(EndPoint))
        return GetMuzzleWorldLocation();
    
    return STUUtils::PointToHorizon::GetPredictPoint(FlyTime, ASTUProjectile::GetInitialSpeed(), GetMuzzleWorldLocation(), EndPoint);
}
bool ASTULauncherWeapon::GetTargetPoint(FVector& TargetPoint) 
{
    FVector TraceStart, TraceEnd;
    if (!this->GetTraceData(TraceStart, TraceEnd))
        return false;

    FHitResult HitResult;
    this->MakeHit(HitResult, TraceStart, TraceEnd);
    TargetPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;

    return true;
}

bool ASTULauncherWeapon::GetTargetPointAndSpeedVector(FVector& TargetPoint, FVector& SpeedVector)
{
    FVector TraceStart, TraceEnd;
    if (!this->GetTraceData(TraceStart, TraceEnd))
        return false;

    FHitResult HitResult;
    this->MakeHit(HitResult, TraceStart, TraceEnd);
    TargetPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    
    float InitialSpeed = ASTUProjectile::GetInitialSpeed();

    SpeedVector = (TargetPoint - GetMuzzleWorldLocation()).GetSafeNormal();
    SpeedVector.Normalize();
    FVector Direction = SpeedVector;
    SpeedVector *= InitialSpeed;

    FVector TempSpeedVector = SpeedVector;
    TempSpeedVector.Z = 0;

    float Time = TempSpeedVector.Size() / InitialSpeed;
    
    float CosAngle = FMath::Clamp((SpeedVector.Z + STUUtils::PointToHorizon::G() * Time * Time / 2) / (Time * InitialSpeed), 0.0f, 1.0f);
    float Angle = UKismetMathLibrary::DegAsin(CosAngle);

    FVector NewSpeedVector = Direction;

    
    NewSpeedVector.X *= UKismetMathLibrary::DegSin(90 - Angle);
    NewSpeedVector.Y *= UKismetMathLibrary::DegSin(90 - Angle);
    NewSpeedVector.Normalize();
    NewSpeedVector.X *= InitialSpeed;
    NewSpeedVector.Y *= InitialSpeed;

    NewSpeedVector.Z = InitialSpeed * UKismetMathLibrary::DegCos(90 - Angle);

    SpeedVector = NewSpeedVector;

    return true;
}