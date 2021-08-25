#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "Projectile.h"

#include "SplitArrowsAbility.generated.h"

UCLASS()
class MYPROJECT4_API USplitArrowsAbility : public UAbility
{
	GENERATED_BODY()

public:
	USplitArrowsAbility();

	UPROPERTY()
	AProjectile* ProjectileForward;

	UPROPERTY()
	AProjectile* ProjectileLeft;

	UPROPERTY()
	AProjectile* ProjectileRight;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	virtual void UseAbility(AActor* Target) override;

	virtual void SetInCooldown() override;
};
