// My 1st Unreal Shoot Them Up Game

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectile, All, All);

const float ASTUProjectile::InitialSpeed{2000.0f};

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = true;
    
    this->CollisionComponent = this->CreateDefaultSubobject<USphereComponent>("SphereComponent");
    this->SetRootComponent(this->CollisionComponent);
    this->CollisionComponent->InitSphereRadius(10);
    this->CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //Только триггер
    this->CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); //Блокировать все каналы внешних обработок пересечений
    this->CollisionComponent->bReturnMaterialOnMove = true;
    
    this->MovementComponent = this->CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
    this->MovementComponent->InitialSpeed = InitialSpeed;
    this->MovementComponent->ProjectileGravityScale = 1.0f;
    this->bGenerateOverlapEventsDuringLevelStreaming = true;
    this->WeaponFXComponent = this->CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTUProjectile::Tick(float DeltaSeconds)
{    
    //DrawDebugSphere(this->GetWorld(), GetActorLocation(), CollisionComponent->GetScaledSphereRadius(), 8, FColor::Red, false, 10.0f);
}

void ASTUProjectile::SetShotDirection(const FVector& Direction)
{
    this->ShotDirection = Direction;
}

float ASTUProjectile::GetInitialSpeed() 
{
    return InitialSpeed;
}

float ASTUProjectile::GetStartTime() const
{
    return StartTime;
}


void ASTUProjectile::SetInitialSpeed(float NewInitialSpeed)
{
    if (MovementComponent) MovementComponent->InitialSpeed = NewInitialSpeed;
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(this->CollisionComponent);
    check(this->MovementComponent);
    check(this->WeaponFXComponent);

    this->MovementComponent->Velocity = this->ShotDirection * this->MovementComponent->InitialSpeed;

    auto actor = GetOwner();
    if(actor) this->CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);

    this->CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);

    this->SetLifeSpan(this->LifeSeconds);

    StartTime = GetWorld()->GetRealTimeSeconds();

    UE_LOG(LogProjectile, Display, TEXT("Projectile start at: %s"), *GetActorLocation().ToCompactString());
}

void ASTUProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    if(!this->GetWorld())
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

    //DrawDebugSphere(this->GetWorld(), this->GetActorLocation(), this->DamageRadius, 24, FColor::Blue, false, 10.0f);
    this->WeaponFXComponent->PlayImpactFX(Hit);
    this->Destroy();
}

AController* ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(this->GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}

void ASTUProjectile::NotifyActorBeginOverlap(AActor* OtherActor) 
{
    auto Projectile = Cast<ASTUProjectile>(OtherActor);
    if (Projectile)
    {
        Projectile->Destroy();
        this->Destroy();
        UE_LOG(LogProjectile, Display, TEXT("Projectiles destroyed!!!!!!!!!!!!!!!!!"));
    }
}

float ASTUProjectile::GetCollisionSphereRadius() const
{
    return CollisionComponent->GetScaledSphereRadius();
}

FVector ASTUProjectile::GetVelocity() const
{
    FVector Velocity = Super::GetVelocity();
    if(Velocity.Y == 0.0f && Velocity.Z == 0.0f)
    {
        Velocity = this->ShotDirection * this->InitialSpeed;
    }
    return Velocity;
}

FVector ASTUProjectile::GetVelocityByMovementComponent() const
{
    //ASTUProjectile::GetVelocity()
    FVector Velocity;
    this->MovementComponent->UpdateComponentVelocity();
    return Velocity;
}