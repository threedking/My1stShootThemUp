// My 1st Unreal Shoot Them Up Game


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ASTURifleWeapon::ASTURifleWeapon() 
{
    this->WeaponFXComponent = this->CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay(); 
    check(this->WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
    //UE_LOG(LogWeapon, Display, TEXT("Bang!"));
    this->InitMuzzleFX();
    this->GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon ::MakeShot, this->TimeBetweenShots, true);
    this->MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    this->GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    this->SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::MakeShot()
{
    if (!this->GetWorld() || this->IsAmmoEmpty())
    {
        this->StopFire();
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!this->GetTraceData(TraceStart, TraceEnd))
    {
        this->StopFire();
        return;
    }

    FHitResult HitResult;
    this->MakeHit(HitResult, TraceStart, TraceEnd);

    FVector TraceFXEnd = TraceEnd;

    if (HitResult.bBlockingHit) //Было ли попадание
    {
        TraceFXEnd = HitResult.ImpactPoint;
        //DrawDebugLine(this->GetWorld(), this->GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        //DrawDebugSphere(this->GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        //UE_LOG(LogWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());

        auto DamagedActor = HitResult.GetActor();
        if (DamagedActor)
        {
            float ResultDamage = this->Damage;

            if (HitResult.BoneName.IsEqual(FName("b_head")))
            {
                UE_LOG(LogWeapon, Display, TEXT("HEAD SHOOT!!!"));
                ResultDamage = Damage * 10;
            }
            else
            {
                ResultDamage = Damage;
            }

            DamagedActor->TakeDamage(ResultDamage, FDamageEvent{}, this->GetController(), this);
        }
        this->WeaponFXComponent->PlayImpactFX(HitResult);
    }
    else
    {
        //DrawDebugLine(this->GetWorld(), this->GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
    this->SpawnTraceFX(this->GetMuzzleWorldLocation(), TraceFXEnd);
    this->DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (!this->GetPlayerViewPoint(ViewLocation, ViewRotation))
        return false;

    // const FTransform SocketTransform = this->WeaponMesh->GetSocketTransform(this->MuzzleSocketName);
    TraceStart = ViewLocation; // SocketTransform.GetLocation();

    const auto HalfRad = FMath::DegreesToRadians(this->BulletSpread);
    //const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    const FVector ShootDirection = ViewRotation.Vector();


    TraceEnd = TraceStart + ShootDirection * this->TraceMaxDistance;
    return true;
}

void ASTURifleWeapon::InitMuzzleFX()
{
    if (!this->MuzzleFXComponent)
    {
        this->MuzzleFXComponent = SpawnMuzzleFX();
    }
    this->SetMuzzleFXVisibility(true);
}
void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visible) 
{
    if (this->MuzzleFXComponent)
    {
        this->MuzzleFXComponent->SetPaused(!Visible);
        this->MuzzleFXComponent->SetVisibility(Visible, true);
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this->GetWorld(), this->TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(this->TraceTargetName, TraceEnd);
    }
}

AController* ASTURifleWeapon::GetController() const
{
    const auto Pawn = Cast<APawn>(this->GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}