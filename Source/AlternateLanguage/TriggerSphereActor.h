#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "Components/SphereComponent.h"
#include "TriggerSphereActor.generated.h"

UCLASS()
class ALTERNATELANGUAGE_API ATriggerSphereActor : public ATriggerSphere
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ATriggerSphereActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float RadiusSize;

	FVector CurrentLocation;
	FRotator CurrentRotation;

	bool inZone;
	UPROPERTY(EditAnywhere)
		class AActor* SpecificActor;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereComponent;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyindex, bool bFromSweep, const FHitResult& SweepResults);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
