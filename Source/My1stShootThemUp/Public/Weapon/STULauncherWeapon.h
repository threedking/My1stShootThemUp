// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

class ASTUProjectile;

UCLASS()
class MY1STSHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()

public:
    virtual void StartFire() override;

    FVector GetPredictPointByTime(float FlyTime);
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUProjectile> ProjectileClass;

    virtual void MakeShot() override;
    bool GetTargetPointAndSpeedVector(FVector& TargetPoint, FVector& SpeedVector);
    bool GetTargetPoint(FVector& TargetPoint);

};
