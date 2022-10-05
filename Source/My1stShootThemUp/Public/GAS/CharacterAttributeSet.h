// My 1st Unreal Shoot Them Up Game

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GAS/STUAbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

//Macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class MY1STSHOOTTHEMUP_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UCharacterAttributeSet();

    
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health{100};
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);

    UFUNCTION(Category = "Abilities")
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", ReplicatedUsing = OnRep_DashDamge)
    FGameplayAttributeData DashDamge;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, DashDamge);

    UFUNCTION(Category = "Abilities")
    virtual void OnRep_DashDamge(const FGameplayAttributeData& OldDashDamge);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", ReplicatedUsing = OnRep_IsStunt)
    FGameplayAttributeData IsStunt;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, IsStunt);

    UFUNCTION(Category = "Abilities")
    virtual void OnRep_IsStunt(const FGameplayAttributeData& OldIsStunt);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", ReplicatedUsing = OnRep_Stamina)
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina);

    UFUNCTION(Category = "Abilities")
    virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", ReplicatedUsing = OnRep_Frost)
    FGameplayAttributeData Frost;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Frost);

    UFUNCTION(Category = "Abilities")
    virtual void OnRep_Frost(const FGameplayAttributeData& OldFrost);
	
};
