#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Projectile.generated.h"

class UBoxComponent;

UCLASS()
class MYPROJECT4_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SM_Projectile;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY()
	AActor* Target;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnBeginOverlap(class UPrimitiveComponent* HitComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
	void DestroyProjectile();

	FORCEINLINE void SetTarget(AActor* Enemy)
	{
		this->Target = Enemy;
	}
};
