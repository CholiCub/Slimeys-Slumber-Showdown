// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "SS_Player.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  Can turn this off to improve performance if don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	ExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionFX"));
	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSound"));
	
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MusicTrack = CreateDefaultSubobject<UAudioComponent>(TEXT("Music"));
	MusicTrack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	fBurstDelay = 0.15f;
	
	DamageAmount = 1;
	DamageRadius = CurrentLocation+100;

}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	TotalTime = 0.0f;
	TimeSinceLastShot = 0.0f;

	bHit = false;
	bDestroy = false;
	inZone = false;
	fDestroyTimer = 1.0f;

	ExplosionFX->Deactivate();
	ExplosionSound->Deactivate();

	ThisWorld = GetWorld();

	//return a random int between 0 and RAND_MAX
	RandomStart = FMath::Rand();

	//add dynamic to delegate
	OnActorBeginOverlap.AddDynamic(this, &AEnemy::OnBeginOverlap);
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TotalTime += DeltaTime;
	TimeSinceLastShot += DeltaTime;


	CurrentLocation = this->GetActorLocation();
	CurrentRotation = this->GetActorRotation();

	CurrentLocation.Y += FMath::Sin(TotalTime + RandomStart);
	this->SetActorLocation(CurrentLocation - (CurrentVelocity * DeltaTime));

	
	////handle destory
	
	if (bDestroy)
	{
		this->Destroy();
	}
	if (bHit)
	{
		StaticMesh->SetVisibility(false);
		this->SetActorEnableCollision(false);
		ExplosionFX->Activate();
		ExplosionSound->Activate();

		fDestroyTimer -= DeltaTime;
	}
	if (fDestroyTimer<=0.0f)
	{
		//take health from player
		this->Destroy();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::AreaOfEffectDamage(AActor* EnemyActor, AActor* OtherActor)
{
	ASS_Player* Player = Cast<ASS_Player>(OtherActor);
}

void AEnemy::OnBeginOverlap(AActor* EnemyActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Bounds"))
	{
		bDestroy=true;
	}

	if (OtherActor->ActorHasTag("Hazard")|| OtherActor->ActorHasTag("Player"))
	{
		bHit = true;
	}
	if (OtherActor->ActorHasTag("Projectile"))
	{
		if (ThisWorld)
		{
			bHit = true;

			if (ThisWorld)
			{
				//30% spawn pickup
				if (FMath::RandRange(0,10)>7)
				{
					FVector CurrentLoc = this->GetActorLocation();
					FRotator CurrentRot = this->GetActorRotation();
					FActorSpawnParameters Params = {};

					ThisWorld->SpawnActor(PickUpCan, &CurrentLoc, &CurrentRot, Params);
					
				}
			}
		}
	}
}

