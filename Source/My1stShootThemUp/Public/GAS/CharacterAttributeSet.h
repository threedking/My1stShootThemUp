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
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health);

    UFUNCTION(Category = "Abilities")
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
    /*
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", ReplicatedUsing = OnRep_DashDamge)
    FGameplayAttributeData DashDamge;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, DashDamge);

    UFUNCTION(Category = "Abilities")
    virtual void OnRep_DashDamge(const FGameplayAttributeData& OldDashDamge);
	*/
};
