#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetBoxExtent(FVector(16.0f, 16.0f, 16.0f));

	RootComponent = BoxCollision;

	SM_Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Projectile"));
	SM_Projectile->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SM_Projectile->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(BoxCollision);
	ProjectileMovementComponent->InitialSpeed = 500.0f;
	ProjectileMovementComponent->MaxSpeed = 500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnOverlapEnd);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnBeginOverlap(class UPrimitiveComponent* HitComponent,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		if (IsValid(this->Target) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap IsValid(this->Target) == false"));
			return;
		}
		if (IsValid(OtherActor) == false)
		{
			UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap IsValid(OtherActor) == false"));
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
				UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap IsValid(Enemy) == false"));
			}

			AActor* OwnerWeapon = GetOwner();
			if (IsValid(OwnerWeapon) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap IsValid(OwnerWeapon) == false"));
				return;
			}

			AActor* PlayerOwner = OwnerWeapon->GetOwner();
			if (IsValid(PlayerOwner) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap IsValid(PlayerOwner) == false"));
				return;
			}

			APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerOwner);
			if (IsValid(Player) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap IsValid(Player) == false"));
				return;
			}

			AController* PC = Player->GetController();
			if (IsValid(PC) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap IsValid(PC) == false"));
				return;
			}

			APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(PC);
			if (IsValid(PlayerController) == false)
			{
				UE_LOG(LogTemp, Error, TEXT("AProjectile::OnBeginOverlap IsValid(PlayerController) == false"));
				return;
			}

			FDamageEvent DamageEvent(UDamageType::StaticClass());
			Enemy->TakeDamage(Damage, DamageEvent, PlayerController, Player);

			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, DestroyDelegate, 0.10, false);
		}
	}
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::DestroyProjectile()
{
	this->Destroy();
}

void AProjectile::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}