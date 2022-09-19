// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GAS/STUAbilitySystemComponent.h"
#include "GAS/CharacterAttributeSet.h"
#include "GAS/STUGameplayAbility.h"
#include "GameplayEffectTypes.h"
#include "STUBaseCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUWeaponComponent;
class USphereComponent;


UCLASS()
class MY1STSHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

    // Called to bind functionality to input
    //virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    /*
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;
    */

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUHealthComponent* HealthComponent;
        
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUWeaponComponent* WeaponComponent;
    
    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorName = "Paint Color";

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    USphereComponent* CollisionComponent;
        
    UPROPERTY(EditDefaultsOnly, Category = "Dash", meta = (ClampMin = "0.0", ClampMax = "1000.0"))
    float DashDistance{};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
    USTUAbilitySystemComponent* AbilitySystemComponent;
    //UAbilitySystemComponent* 

    UPROPERTY(BlueprintReadWrite, Category = "Abilities")
    UCharacterAttributeSet* AttributeSet;

    //FGameplayAttributeData asd;
    //GAMEPLAYATTRIBUTE_PROPERTY_GETTER(asd);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
    TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
    TArray<TSubclassOf<USTUGameplayAbility>> DefaultAbilities;
        
    virtual void InitializeAttributes();
    virtual void GiveAbilities();

    virtual void PossessedBy(AController* NewContriller) override; //Gets called on server, init GAS
    virtual void OnRep_PlayerState() override; //Gets called on client, init GAS

    virtual void OnHealthChanged(float Health, float HealthDelta);
    virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
    virtual void OnStuntChanged(const FOnAttributeChangeData& Data);

    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChangedBP(float Health);

    UFUNCTION(BlueprintImplementableEvent)
    void OnStuntChangedBP(bool IsStunt);

    virtual void OnDeath();

    virtual void BeginPlay() override;

public:	
	// Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool GetRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    virtual bool TryDash(TArray<ASTUBaseCharacter*>& DamagedActors);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
    virtual float GetHealth() const;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
    virtual bool GetIsStunt() const;

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    bool IsStuned() const;

    void SetPlayerColor(const FLinearColor& Color);
    bool TryToAddHealth(int32 HelthAmount);

private:

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

    /*
    bool Running{false};
    bool IsWalkForward{false};

    void LookUp(float Amount);
    void TurnAround(float Amount);
    void MoveForaward(float Amount);
    void MoveRight(float Amount);
    void RunStart();
    void RunEnd();
    */


};
