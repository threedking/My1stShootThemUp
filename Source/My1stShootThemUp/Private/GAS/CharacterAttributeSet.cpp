// My 1st Unreal Shoot Them Up Game

#include "GAS/CharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "Player/STUBaseCharacter.h"

UCharacterAttributeSet::UCharacterAttributeSet() 
{

}

void UCharacterAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    auto Context = Data.EffectSpec.GetContext();
    auto InstigatorASC = Context.GetOriginalInstigatorAbilitySystemComponent();

    float DeltaVulue{0.0f};
    if(Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
    {
        DeltaVulue = Data.EvaluatedData.Magnitude;
    }

    ASTUBaseCharacter* Character{nullptr};
    if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        Character = Cast<ASTUBaseCharacter>(Data.Target.AbilityActorInfo->AvatarActor.Get());
    }

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, 100.0f));
    }

    if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
    {
        SetStamina(FMath::Clamp(GetStamina(), 0.0f, 100.0f));
    }
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, DashDamge, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, IsStunt, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Frost, COND_None, REPNOTIFY_Always);
}


void UCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Health, OldHealth);
}

void UCharacterAttributeSet::OnRep_DashDamge(const FGameplayAttributeData& OldDashDamge)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, DashDamge, OldDashDamge);
}

void UCharacterAttributeSet::OnRep_IsStunt(const FGameplayAttributeData& OldIsStunt) 
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, IsStunt, OldIsStunt);
}

void UCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Stamina, OldStamina);
}

void UCharacterAttributeSet::OnRep_Frost(const FGameplayAttributeData& OldFrost) 
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Frost, OldFrost);
}