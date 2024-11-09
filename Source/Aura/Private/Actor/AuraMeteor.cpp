#include "Actor/AuraMeteor.h"
#include "Components/SphereComponent.h"

AAuraMeteor::AAuraMeteor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
		
}

void AAuraMeteor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraMeteor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
