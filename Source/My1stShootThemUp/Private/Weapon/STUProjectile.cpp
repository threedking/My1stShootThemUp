// My 1st Unreal Shoot Them Up Game

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h"

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    this->CollisionComponent = this->CreateDefaultSubobject<USphereComponent>("SphereComponent");
    this->SetRootComponent(this->CollisionComponent);
    this->CollisionComponent->InitSphereRadius(5.0f);
    this->CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //Только триггер
    this->CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); //Блокировать все каналы внешних обработок пересечений
    this->CollisionComponent->bReturnMaterialOnMove = true;

    this->MovementComponent = this->CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
    this->MovementComponent->InitialSpeed = 1500.0f;
    this->MovementComponent->ProjectileGravityScale = 1.0f;

    this->WeaponFXComponent = this->CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTUProjectile::SetShotDirection(const FVector& Direction)
{
    this->ShotDirection = Direction;
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(this->CollisionComponent);
    check(this->MovementComponent);
    check(this->WeaponFXComponent);

    this->MovementComponent->Velocity = this->ShotDirection * this->MovementComponent->InitialSpeed;

    this->CollisionComponent->IgnoreActorWhenMoving(this->GetOwner(), true);
    this->CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);

    this->SetLifeSpan(this->LifeSeconds);
}

void ASTUProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    if (!this->GetWorld())
        return;

    this->MovementComponent->StopMovementImmediately();

    UGameplayStatics::ApplyRadialDamage(this->GetWorld(),
        this->DamageAmount,         //
        this->GetActorLocation(),   //
        this->DamageRadius,         //
        UDamageType::StaticClass(), //
        {},                         //игнорируемые уроном акторы
        this,                       //
        this->GetController(),      //
        this->DoFullDamage);

    //DrawDebugSphere(this->GetWorld(), this->GetActorLocation(), this->DamageRadius, 24, FColor::Red, false, 5.0f);
    this->WeaponFXComponent->PlayImpactFX(Hit);
    this->Destroy();
}

AController* ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(this->GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}