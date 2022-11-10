// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Projectile.h"
#include"TriggerSphereActor.h"
#include "Pickup.h"
#include "Components/SphereComponent.h"
#include"DrawDebugHelpers.h"

#include "Enemy.generated.h"

UCLASS()
class ALTERNATELANGUAGE_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* ExplosionFX;

	UPROPERTY(EditAnywhere)
		UAudioComponent* ExplosionSound;

	UWorld* ThisWorld;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<APickup> PickUpCan;

	UPROPERTY(EditAnywhere)
		FVector CurrentVelocity;

	UPROPERTY(EditAnywhere)
		UAudioComponent* MusicTrack;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SphereCo;

	float TotalTime;
	float TimeSinceLastShot;
	float RandomStart;

	bool bHit;
	bool bDestroy;
	bool inZone;

	float fDestroyTimer;
	float fBurstDelay;
	float DamageAmount;

	UPROPERTY(EditAnywhere)
	FVector DamageRadius;

	FVector CurrentLocation;
	FRotator CurrentRotation;

	UFUNCTION()
	void AreaOfEffectDamage(AActor* EnemyActor, AActor* OtherActor);

	//tell delegate system that bind this to a delegate
	//collision detection
	UFUNCTION()
		void OnBeginOverlap(AActor* EnemyActor, AActor* OtherActor);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
