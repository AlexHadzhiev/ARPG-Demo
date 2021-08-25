#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "Projectile.h"

#include "FireballAbility.generated.h"

UCLASS()
class ARPGDEMO_API UFireballAbility : public UAbility
{
	GENERATED_BODY()

public:
	UFireballAbility();

	UPROPERTY()
	AProjectile* Projectile;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	virtual void UseAbility(AActor* Target) override;

	virtual void SetInCooldown() override;
};
