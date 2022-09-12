// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseTurret.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USceneComponent;
class ASTUProjectile;

UCLASS()
class MY1STSHOOTTHEMUP_API ASTUBaseTurret : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUBaseTurret();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* TurretBodyMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* TurretHeadMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    USceneComponent* TurretHead;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TSubclassOf<ASTUProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    float InitialSpeed = 1500.0f;

    float LerpDuration = 0.5f;

    FRotator TargetRotator;

    FBoomData BoomData;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

    bool GetMyBoomData(ASTUProjectile* Projectile);
    void FireByBoomData();

public:	
	virtual void Tick(float DeltaTime) override;

};
