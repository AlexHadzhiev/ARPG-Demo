#include "ArrowProjectile.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"

AArrowProjectile::AArrowProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent->InitialSpeed = 600.0f;
	ProjectileMovementComponent->MaxSpeed = 600.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

void AArrowProjectile::BeginPlay()
{
	Super::BeginPlay();

}

void AArrowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArrowProjectile::OnBeginOverlap(class UPrimitiveComponent* HitComponent,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Wall"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Wall Hit"));

		this->DestroyProjectile();

		return;
	}
	if (OtherActor->ActorHasTag("Enemy"))
	{
		if (IsValid(this->Target) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AFireballProjectile::OnBeginOverlap IsValid(this->Target) == false"));
			return;
		}
		if (IsValid(OtherActor) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AFireballProjectile::OnBeginOverlap IsValid(OtherActor) == false"));
			return;
		}

		if (this->Target == OtherActor)
		{
			FTimerHandle DestroyTimerHandle;

			FTimerDelegate DestroyDelegate;
			DestroyDelegate.BindUFunction(this, FName("DestroyProjectile"));

			AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
			if (IsValid(Enemy) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AArrowProjectile::OnBeginOverlap IsValid(Enemy) == false"));
			}

			AActor* PlayerOwner = GetOwner();
			if (IsValid(PlayerOwner) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AArrowProjectile::OnBeginOverlap IsValid(PlayerOwner) == false"));
				return;
			}

			APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerOwner);
			if (IsValid(Player) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AArrowProjectile::OnBeginOverlap IsValid(Player) == false"));
				return;
			}

			AController* PC = Player->GetController();
			if (IsValid(PC) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AArrowProjectile::OnBeginOverlap IsValid(PC) == false"));
				return;
			}

			APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);
			if (IsValid(PlayerController) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AArrowProjectile::OnBeginOverlap IsValid(PlayerController) == false"));
				return;
			}

			FDamageEvent DamageEvent(UDamageType::StaticClass());
			Enemy->TakeDamage(Damage, DamageEvent, PlayerController, Player);

			this->DestroyProjectile();
		}
	}
}

void AArrowProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}