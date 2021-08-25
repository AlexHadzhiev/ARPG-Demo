#include "SplitArrowsProjectile.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"

ASplitArrowsProjectile::ASplitArrowsProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent->InitialSpeed = 600.0f;
	ProjectileMovementComponent->MaxSpeed = 600.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

void ASplitArrowsProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void ASplitArrowsProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplitArrowsProjectile::OnBeginOverlap(class UPrimitiveComponent* HitComponent,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Wall"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Wall Hit"));

		this->DestroyProjectile();
	}
	if (OtherActor->ActorHasTag("Enemy"))
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
		if (IsValid(Enemy) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("ASplitArrowsProjectile::OnBeginOverlap IsValid(Enemy) == false"));
		}

		AActor* PlayerOwner = GetOwner();
		if (IsValid(PlayerOwner) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("ASplitArrowsProjectile::OnBeginOverlap IsValid(PlayerOwner) == false"));
			return;
		}

		APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerOwner);
		if (IsValid(Player) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("ASplitArrowsProjectile::OnBeginOverlap IsValid(Player) == false"));
			return;
		}

		AController* PC = Player->GetController();
		if (IsValid(PC) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("ASplitArrowsProjectile::OnBeginOverlap IsValid(PC) == false"));
			return;
		}

		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);
		if (IsValid(PlayerController) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("ASplitArrowsProjectile::OnBeginOverlap IsValid(PlayerController) == false"));
			return;
		}

		FDamageEvent DamageEvent(UDamageType::StaticClass());
		Enemy->TakeDamage(Damage, DamageEvent, PlayerController, Player);

		this->DestroyProjectile();
	}
}
