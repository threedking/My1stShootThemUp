// My 1st Unreal Shoot Them Up Game


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);


ASTUBasePickup::ASTUBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	
    this->CollisionComponent = this->CreateDefaultSubobject<USphereComponent>("SphereComponent");
    this->SetRootComponent(this->CollisionComponent);
    this->CollisionComponent->InitSphereRadius(50.0f);
    this->CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //Только триггер
    this->CollisionComponent->SetCollisionResponseToAllChannels(
        ECollisionResponse::ECR_Overlap); //Блокировать все каналы внешних обработок пересечений
}

void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();

    check(this->CollisionComponent);
    
    this->GenerateRotationYaw();
}

void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    this->AddActorLocalRotation(FRotator(0.0f, this->RotationYaw, 0.0f));
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor) {
    Super::NotifyActorBeginOverlap(OtherActor);
    
    const auto Pawn = Cast<APawn>(OtherActor);
    if (this->GivePickupTo(Pawn))
    {
        this->PickupWasTaken();
    }
}


void ASTUBasePickup::PickupWasTaken() 
{
    this->CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    if (this->GetRootComponent())
    {
        this->GetRootComponent()->SetVisibility(false, true);
    }
    FTimerHandle RespawnTimerHandle;
    this->GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);    
}
void ASTUBasePickup::Respawn() 
{
    this->GenerateRotationYaw();
    this->CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    if (this->GetRootComponent())
    {
        this->GetRootComponent()->SetVisibility(true, true);
    }
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

void ASTUBasePickup::GenerateRotationYaw() 
{
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
    this->RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}