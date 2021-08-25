#include "FireballProjectile.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"

AFireballProjectile::AFireballProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent->InitialSpeed = 450.0f;
	ProjectileMovementComponent->MaxSpeed = 450.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

void AFireballProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerDelegate DestroyDelegate;
	DestroyDelegate.BindUFunction(this, FName("DestroyProjectile"));

	if (!(GetWorld()->GetTimerManager().IsTimerActive(DeathTimerHandle)))
	{
		GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, DestroyDelegate, Lifetime, false);
	}
}

void AFireballProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireballProjectile::OnBeginOverlap(class UPrimitiveComponent* HitComponent,
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
		FTimerHandle DestroyTimerHandle;

		FTimerDelegate DestroyDelegate;
		DestroyDelegate.BindUFunction(this, FName("DestroyProjectile"));

		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
		if (IsValid(Enemy) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AFireballProjectile::OnBeginOverlap IsValid(Enemy) == false"));
		}

		AActor* PlayerOwner = GetOwner();
		if (IsValid(PlayerOwner) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AFireballProjectile::OnBeginOverlap IsValid(PlayerOwner) == false"));
			return;
		}

		APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerOwner);
		if (IsValid(Player) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AFireballProjectile::OnBeginOverlap IsValid(Player) == false"));
			return;
		}

		AController* PC = Player->GetController();
		if (IsValid(PC) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AFireballProjectile::OnBeginOverlap IsValid(PC) == false"));
			return;
		}

		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);
		if (IsValid(PlayerController) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AFireballProjectile::OnBeginOverlap IsValid(PlayerController) == false"));
			return;
		}

		FDamageEvent DamageEvent(UDamageType::StaticClass());
		Enemy->TakeDamage(Damage, DamageEvent, PlayerController, Player);
	}
}