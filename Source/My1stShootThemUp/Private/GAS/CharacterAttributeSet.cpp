// My 1st Unreal Shoot Them Up Game


#include "GAS/CharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"

/* FGameplayAttribute UCharacterAttributeSet::StatusAttribute()
{
    static UProperty* Property = FindFieldChecked<UProperty>(UCharacterAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UCharacterAttributeSet, Status));
    return FGameplayAttribute(Property);
}*/

UCharacterAttributeSet::UCharacterAttributeSet() 
{

}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
}


void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}
/*
void UCharacterAttributeSet::OnRep_DashDamge(const FGameplayAttributeData& OldDashDamge)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, DashDamge, OldDashDamge);
}
*/