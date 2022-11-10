#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"

#include "Projectile.h"

#include "SS_Player.generated.h"

UCLASS()
class ALTERNATELANGUAGE_API ASS_Player : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASS_Player();

	USceneComponent* DefaultSceneRoot;

	// variable will be available to view in World Outliner
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* ParticleSystems;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* ExplosionFX;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SlimeMesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
		UAudioComponent* DeathExplosionSound;

	//restrict the moevement of player beyond bounds
	UPROPERTY(EditAnywhere)
		float Field_Width;

	UPROPERTY(EditAnywhere)
		float Field_Height;

	UPROPERTY(BlueprintReadonly)
		float Max_Health;

	UPROPERTY(BlueprintReadonly)
		float Current_Health;
	
	UPROPERTY(BlueprintReadonly)
		float Max_Armor;
	
	UPROPERTY(BlueprintReadonly)
		float Current_Armor;

	void MoveRigth(float AxisValue);
	void MoveUp(float AxisValue);

	void ThrowWeapon();
	void StartThrowing();
	void StopThrowing();

	UPROPERTY(EditAnywhere)
		float Max_Velocity;

	bool bIsFiring;
	float WeaponFireRate;
	float TimeSinceLastShot;

	bool bHit;
	bool bDead;
	bool inZone;

	float Current_X_Velocity;
	float Current_Y_Velocity;

	UPROPERTY(BlueprintReadonly)
		float PlayerScore;

	FVector Current_Location;
	FRotator Current_Rotation;
	FVector New_Location;

	//detect when colliding
	//going to be bound to an event
	UFUNCTION()
		void OnBeginOverlap(AActor* PlayerActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile>  Projectile_BP;

	void CollectablePickup();
	void AreaOfEffectDamage(bool isZone);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
