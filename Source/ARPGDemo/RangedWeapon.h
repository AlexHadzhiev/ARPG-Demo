#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "EnemyCharacter.h"
#include "Projectile.h"

#include "RangedWeapon.generated.h"

UCLASS()
class ARPGDEMO_API ARangedWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	ARangedWeapon();

	UPROPERTY()
	AProjectile* Projectile;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Attack(AActor* Target, AActor* PlayerActor) override;
};
