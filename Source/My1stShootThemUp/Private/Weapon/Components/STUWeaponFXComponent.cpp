// My 1st Unreal Shoot Them Up Game

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = true;	
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit) 
{
    auto ImpactData = this->DefaultImpactData;
     
	if (Hit.PhysMaterial.IsValid())
    {
        const auto PhysMat = Hit.PhysMaterial.Get();
        if (this->ImpactDataMap.Contains(PhysMat))
        {
            ImpactData = this->ImpactDataMap[PhysMat];
        }
    }
    
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this->GetWorld(), //
        ImpactData.NiagaraEffect, //
        Hit.ImpactPoint, //
        Hit.ImpactNormal.Rotation());

    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(this->GetWorld(), //
        ImpactData.DecalData.Material,//
        ImpactData.DecalData.Size,//
        Hit.ImpactPoint,//
        Hit.ImpactNormal.Rotation());

    if (DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
    }
}

