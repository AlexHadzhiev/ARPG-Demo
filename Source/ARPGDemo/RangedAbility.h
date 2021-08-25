#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "Projectile.h"

#include "RangedAbility.generated.h"

UCLASS()
class ARPGDEMO_API URangedAbility : public UAbility
{
	GENERATED_BODY()
	
public:
	URangedAbility();

	UPROPERTY(EditAnywhere)
	uint16 NumberOfProjectiles;

	UPROPERTY()
	AProjectile* Projectile;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	virtual void UseAbility() override;

	virtual void UseAbility(AActor* Target) override;

	virtual void SetInCooldown() override;
};
