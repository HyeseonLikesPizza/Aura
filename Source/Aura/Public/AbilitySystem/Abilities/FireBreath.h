#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "FireBreath.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UFireBreath : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float MinEffectVelocity;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float MaxEffectVelocity;
	
};
