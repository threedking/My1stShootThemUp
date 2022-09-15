// My 1st Unreal Shoot Them Up Game


#include "Player/STUBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharecterLog, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    this->HealthComponent = this->CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
        // нет SetupAttachment потому что нет представления на сцене
    
    //this->HealthTextComponent = this->CreateDefaultSubobject<UTextRenderComponent>("TextRenderComponent");
    //this->HealthTextComponent->SetupAttachment(this->GetRootComponent());
    //this->HealthTextComponent->SetOwnerNoSee(true);
    
    this->WeaponComponent = this->CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");

    CollisionComponent = this->CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(70.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    AbilitySystemComponent = CreateDefaultSubobject<USTUAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>("AttributeSet");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(this->HealthComponent);
    //check(this->HealthTextComponent);
    check(this->GetCharacterMovement());
    check(this->GetMesh());
    check(CollisionComponent);
    //Проверка макросом чек, который сгенерит assrtion, есди компоненты нулевые
    //Работает только в дебаг и дев билдах



    this->OnHealthChanged(this->HealthComponent->GetHealth(), 0.0f);
    this->HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    this->HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    this->LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}


void ASTUBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //const auto Health = this->HealthComponent->GetHealth();
    

   // this->TakeDamage(0.1f, FDamageEvent{}, Controller, this);
    //урон, тип урона, контроллер нанесший, актор нанесший
}

// Called to bind functionality to input
/* void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(this->WeaponComponent);

    
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForaward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::LookUp);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::TurnAround);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::RunStart);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::RunEnd);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this->WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this->WeaponComponent, &USTUWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this->WeaponComponent, &USTUWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, this->WeaponComponent, &USTUWeaponComponent::Reload);


}*/

bool ASTUBaseCharacter::GetRunning() const
{
    return false; //this->Running&& this->IsWalkForward && !this->GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (this->GetVelocity().IsZero())
    {
        return 0.0f;
    }
    const auto VelocityNormal = this->GetVelocity().GetSafeNormal(); // Нормированный вектор скорости (нормаль)
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(this->GetActorForwardVector(), VelocityNormal)); // Скалярное произведение f-вектора и вектора нормали, далее arccosine от произв и получаем угол между ними
    const auto CrossProduct = FVector::CrossProduct(this->GetActorForwardVector(), VelocityNormal); //Ортогональный вектор
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}


void ASTUBaseCharacter::OnDeath() 
{
    //UE_LOG(BaseCharecterLog, Display, TEXT("Player %s is DEAD!"), *this->GetName());

    //this->PlayAnimMontage(this->DeathAnimMontage);

    this->GetCharacterMovement()->DisableMovement();

    this->SetLifeSpan(5.0f);

    this->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    this->WeaponComponent->StopFire();

    this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    this->GetMesh()->SetSimulatePhysics(true);

}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    //this->HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -this->GetCharacterMovement()->Velocity.Z;
    UE_LOG(BaseCharecterLog, Display, TEXT("On Landed = %f"), FallVelocityZ);

    if (FallVelocityZ < this->LandedDamageVelocity.X)
    {
        return;
    }

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(this->LandedDamageVelocity, this->LandedDamage, FallVelocityZ);
    UE_LOG(BaseCharecterLog, Display, TEXT("FinalDamage  = %f"), FinalDamage);
    this->TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto MaterialInst = this->GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if(!MaterialInst) return;

    MaterialInst->SetVectorParameterValue(this->MaterialColorName, Color);
}

void ASTUBaseCharacter::Dash() 
{
    if(!GetWorld()) return;
    
    FCollisionQueryParams CollisionParams;
    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTUBaseCharacter::StaticClass(), AllActors);
    CollisionParams.AddIgnoredActors(AllActors);

    FVector DestLocation = GetActorLocation() + GetActorForwardVector() * DashDistance;
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), DestLocation, ECollisionChannel::ECC_Visibility, CollisionParams);

    if(HitResult.bBlockingHit)
    {
        float NewDashDistance = HitResult.Distance - GetCapsuleComponent()->GetUnscaledCapsuleRadius();
        if(NewDashDistance < DashDistance && NewDashDistance > GetCapsuleComponent()->GetUnscaledCapsuleRadius())
        {
            DestLocation = GetActorLocation() + GetActorForwardVector() * NewDashDistance;
            HitResult.Reset();
            GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), DestLocation, ECollisionChannel::ECC_Visibility, CollisionParams);
        }            
    }

    if(!HitResult.bBlockingHit)
    {
        if(this->TeleportTo(DestLocation, this->GetActorRotation(), true))
        {
            //TODO hit others FVector:: LineSphereIntersection
            this->TeleportTo(DestLocation, this->GetActorRotation());
        }
    }
}

UAbilitySystemComponent* ASTUBaseCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void ASTUBaseCharacter::InitializeAttributes() 
{
    if(!AbilitySystemComponent || !DefaultAttributeEffect) return;
    
    FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
    EffectContextHandle.AddSourceObject(this);

    FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContextHandle);
    if(!EffectSpecHandle.IsValid()) return;
    
    FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

void ASTUBaseCharacter::GiveAbilities()
{
    if(HasAuthority() && AbilitySystemComponent) // Is Server
    {
        for(auto& StartupAbility : DefaultAbilities)
        {
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
        }
    }
    
}

void ASTUBaseCharacter::PossessedBy(AController* NewContriller) 
{
    Super::PossessedBy(NewContriller);

    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    InitializeAttributes();
    GiveAbilities();
}

void ASTUBaseCharacter::OnRep_PlayerState() 
{
    Super::OnRep_PlayerState();

    AbilitySystemComponent->InitAbilityActorInfo(this, this);
    InitializeAttributes();

    if(AbilitySystemComponent && InputComponent)
    {
        FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm),static_cast<int32>(EGASAbilityInputID::Cancel));
        AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
    }
}
