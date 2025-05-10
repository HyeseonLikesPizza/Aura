#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BossInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBossInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IBossInterface
{
	GENERATED_BODY()

public:
	
	virtual void BossDie() = 0;
};
