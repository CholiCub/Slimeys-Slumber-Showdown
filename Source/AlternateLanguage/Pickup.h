#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Pickup.generated.h"

UCLASS()
class ALTERNATELANGUAGE_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionMesh;

	UFUNCTION()
		void OnBeginOverlap(AActor* PickupActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
