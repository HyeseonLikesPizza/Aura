
#include "Character/AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Aura/AuraLogChannels.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	ReflectSphere = CreateDefaultSubobject<USphereComponent>("ReflectSphere");
	ReflectSphere->SetupAttachment(GetRootComponent());
	ReflectSphere->SetCollisionObjectType(ECC_Reflect);
	ReflectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ReflectSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	ReflectSphere->SetCollisionResponseToChannel(ECC_Reflect, ECR_Overlap);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	BaseWalkSpeed = 250.f;

	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->MarkRenderStateDirty();
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->MarkRenderStateDirty();
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(NewController);
	UBlackboardComponent* BlackboardComponent = AuraAIController->GetBlackboardComponent();

	if (BlackboardComponent)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		AuraAIController->RunBehaviorTree(BehaviorTree);
		BlackboardComponent->SetValueAsBool(FName("HitReacting"), false);
		BlackboardComponent->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);

		
	}
}


void AAuraEnemy::HighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	// Do not change OutDestination
}

int32 AAuraEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}

void AAuraEnemy::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (AuraAIController) AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	SpawnLoot();
	
	Super::Die(DeathImpulse);
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AAuraEnemy::OnReflectSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bManaReflexActive && OtherActor)
	{
		if (auto MoveComp = OtherActor->FindComponentByClass<UProjectileMovementComponent>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Before reflect, velocity: %s"), *MoveComp->Velocity.ToString());
		}

		ReflectToPlayer(OtherActor);
	}
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();

	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	if (UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		UE_LOG(LogAura, Warning, TEXT("Health : %f"), AuraAS->GetHealth());

		if (AuraAIController && AuraAIController->GetBlackboardComponent() && AuraAS)
		{
			AuraAIController->GetBlackboardComponent()->SetValueAsFloat(FName("Health"), AuraAS->GetHealth());
		}

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				// 블랙보드 키 값 업데이트
				if (AuraAIController && AuraAIController->GetBlackboardComponent())
				{
					AuraAIController->GetBlackboardComponent()->SetValueAsFloat(FName("Health"), Data.NewValue);
				}

				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}

	if (UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		
	}
	
	ReflectSphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEnemy::OnReflectSphereOverlap);
}


void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::StunTagChanged);

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	OnAscRegistered.Broadcast(AbilitySystemComponent);
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);

	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
	}
}

void AAuraEnemy::ReflectToPlayer(AActor* Target)
{
	if (!Target || Target->Tags.Contains("Reflected")) return;

	AActor* Player = Target->GetInstigator();

	FVector Direction = (Player->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
	FRotator NewRot = Direction.Rotation();

	Target->SetActorRotation(NewRot);
	Target->SetActorEnableCollision(false); // 충돌 중단



	if (UProjectileMovementComponent* MoveComp = Target->FindComponentByClass<UProjectileMovementComponent>())
	{
		MoveComp->bIsHomingProjectile = false;
		MoveComp->HomingTargetComponent = nullptr;
		MoveComp->Velocity = Direction * MoveComp->InitialSpeed;
		MoveComp->UpdateComponentVelocity();
	}

	Target->Tags.Add("Reflected");

	// 0.1초 후 다시 충돌 켜기
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [Target]()
		{
			if (IsValid(Target))
			{
				Target->SetActorEnableCollision(true);
			}
		}, 0.1f, false);
}
