#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "SplitArrowsProjectile.generated.h"

UCLASS()
class MYPROJECT4_API ASplitArrowsProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	ASplitArrowsProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnBeginOverlap(class UPrimitiveComponent* HitComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
