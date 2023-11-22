
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystemComponent.h"

void UAuraGameplayAbility::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, UAbilitySystemComponent* ASC, float Level)
{
	check(IsValid(ASC));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
}
