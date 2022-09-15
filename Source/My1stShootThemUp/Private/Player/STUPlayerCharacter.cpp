// My 1st Unreal Shoot Them Up Game


#include "Player/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/STULauncherWeapon.h"
#include "DrawDebugHelpers.h"


ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit): Super(ObjInit)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    
    this->SpringArmComponent = this->CreateAbstractDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    this->SpringArmComponent->SetupAttachment(this->GetRootComponent());
    this->SpringArmComponent->bUsePawnControlRotation = true;
    this->SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    this->CameraComponent = this->CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    this->CameraComponent->SetupAttachment(this->SpringArmComponent);

    this->CameraCollisionComponent = this->CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    this->CameraCollisionComponent->SetupAttachment(this->CameraComponent);
    this->CameraCollisionComponent->SetSphereRadius(10.0f);
    this->CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ASTUPlayerCharacter::BeginPlay() 
{
    Super::BeginPlay();

    check(this->CameraCollisionComponent);

    this->CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    this->CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
}

// Called to bind functionality to input
void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(this->WeaponComponent);


    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForaward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::LookUp);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::TurnAround);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::RunStart);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::RunEnd);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this->WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this->WeaponComponent, &USTUWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this->WeaponComponent, &USTUWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, this->WeaponComponent, &USTUWeaponComponent::Reload);
    PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASTUPlayerCharacter::Dash);

    if (AbilitySystemComponent && InputComponent)
    {
        FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
        AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
    }
}

bool ASTUPlayerCharacter::GetRunning() const
{
    return this->Running && this->IsWalkForward && !this->GetVelocity().IsZero();
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if(this->Controller)
    {
        this->Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUPlayerCharacter::MoveForaward(float Amount)
{
    this->IsWalkForward = Amount > 0.0f;
    this->AddMovementInput(this->GetActorForwardVector(), Amount);
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
    this->AddMovementInput(this->GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::LookUp(float Amount)
{
    this->AddControllerPitchInput(Amount);
}

void ASTUPlayerCharacter::TurnAround(float Amount)
{
    this->AddControllerYawInput(Amount);
}

void ASTUPlayerCharacter::RunStart()
{
    this->Running = true;
    /* UCharacterMovementComponent* ptrCM = this->GetCharacterMovement();
    if (ptrCM)
    {
        ptrCM->MaxWalkSpeed = 1500;
    }*/
}

void ASTUPlayerCharacter::RunEnd()
{
    this->Running = false;
    /* UCharacterMovementComponent* ptrCM = this->GetCharacterMovement();
    if (ptrCM)
    {
        ptrCM->MaxWalkSpeed = 600;
    }*/
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    this->ChackCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    this->ChackCameraOverlap();
}

void ASTUPlayerCharacter::ChackCameraOverlap()
{
    const auto HideMesh = this->CameraCollisionComponent->IsOverlappingComponent(this->GetCapsuleComponent());
    this->GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent*> MeshChildren;
    this->GetMesh()->GetChildrenComponents(true, MeshChildren);

    for(auto MeshChild : MeshChildren)
    {
        const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
        if(MeshChildGeometry)
        {
            MeshChildGeometry->SetOwnerNoSee(HideMesh);
        }
    }

}

void ASTUPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    auto Weapon = WeaponComponent->GetCurrentWeapon();
    if(!Weapon) return;

    auto Launcher = Cast<ASTULauncherWeapon>(Weapon);
    if (Launcher)
    {
        for(float i = 0.1f; i <= 2.0f; i+=0.1f)
        {
            DrawDebugSphere(this->GetWorld(), Launcher->GetPredictPointByTime(i), 10, 8, FColor::Red);
        }
    }        
}