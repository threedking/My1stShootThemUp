// My 1st Unreal Shoot Them Up Game


#include "Dev/STUDevDamageActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ASTUDevDamageActor::ASTUDevDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->SceneComponent = this->CreateDefaultSubobject<USceneComponent>("SceneComponent");
    this->SetRootComponent(this->SceneComponent);
}

// Called when the game starts or when spawned
void ASTUDevDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTUDevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(this->GetWorld(), this->GetActorLocation(), this->Radius, 24, this->SphereColor);

    UGameplayStatics::ApplyRadialDamage(
        this->GetWorld(), this->Damage, this->GetActorLocation(), this->Radius, this->DamageType, {}, this, nullptr, this->DoFullDamage);
	
}

