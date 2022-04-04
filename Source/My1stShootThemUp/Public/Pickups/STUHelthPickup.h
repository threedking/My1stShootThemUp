// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHelthPickup.generated.h"

/**
 * 
 */
UCLASS()
class MY1STSHOOTTHEMUP_API ASTUHelthPickup : public ASTUBasePickup
{
	GENERATED_BODY()

		
private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    int32 HelthAmount{50};
	
};
