// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"


class USphereComponent;
class UProjectileMovementComponent;
class USTUWeaponFXComponent;

UCLASS()
class MY1STSHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUProjectile();

	void SetShotDirection(const FVector& Direction);
    void SetInitialSpeed(float InitialSpeed);

	static float GetInitialSpeed();
    float GetStartTime() const;		
    FVector GetVelocityByMovementComponent() const;
    float GetCollisionSphereRadius() const;    
	virtual FVector GetVelocity() const override;

protected:
	virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool DoFullDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float LifeSeconds = 5.0f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    static const float InitialSpeed;
    
    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

	float StartTime{0.0f};

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:	
	FVector ShotDirection;

	UFUNCTION()
    void OnProjectileHit (UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;
};
