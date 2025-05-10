#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "JumpAttack.generated.h"

UCLASS()
class AURA_API UJumpAttack : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float ImpactRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float SmashRadius = 100.f;
};
