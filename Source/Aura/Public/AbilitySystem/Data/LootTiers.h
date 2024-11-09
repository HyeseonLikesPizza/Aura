#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RootTiers | Spawning")
	TSubclassOf<AActor> LootClass;

	UPROPERTY(EditAnywhere, Category = "RootTiers | Spawning")
	float ChanceToSpawn = 0.f;

	UPROPERTY(EditAnywhere, Category = "RootTiers | Spawning")
	int32 MaxNumberToSpawn = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RootTiers | Spawning")
	bool bLootLevelOveride = true;
};


UCLASS()
class AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();

	UPROPERTY(EditDefaultsOnly, Category = "RootTiers | Spawning")
	TArray<FLootItem> LootItems;
};
