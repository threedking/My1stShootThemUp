// My 1st Unreal Shoot Them Up Game


#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    this->OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}