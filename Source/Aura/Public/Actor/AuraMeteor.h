#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraMeteor.generated.h"

class USphereComponent;

UCLASS()
class AURA_API AAuraMeteor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraMeteor();
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystemComponent> FireEffect;

};
