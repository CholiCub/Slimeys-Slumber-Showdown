#include "Hazard.h"

// Sets default values
AHazard::AHazard()
{
 	// Set this actor to call Tick() every frame.  Can turn this off to improve performance if don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	ToyExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ToyExplosionSound"));
	ToyExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ToyExplosionFX"));

	//calling functions on a pointer
	//Keep the transform relative to the parent - when moving the game wiull move with parent
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ToyExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ToyExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//movement
	Spawn_X_Velocity = -500.0f;
	Spawn_Y_Velocity = 0.0f;

	SelfDestructTimer = 1.0f;
	bHit = false;
}


// Called when the game starts or when spawned
void AHazard::BeginPlay()
{
	Super::BeginPlay();

	Initial_Rotation = 0.0f;
	Initial_X_Location = 1500.0f;
	Initial_Y_Location = FMath::RandRange(-400, 400);

	RandSize = FMath::RandRange(3, 5);
	this->SetActorScale3D(FVector(RandSize, RandSize, RandSize));
	ToyExplosionSound->Deactivate();
	ToyExplosionSound->bStopWhenOwnerDestroyed = false;
	
	ToyExplosionFX->Deactivate();

	//bind the event
	OnActorBeginOverlap.AddDynamic(this, &AHazard::OnBeginOverlap);
}

// Called every frame
void AHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(this != nullptr);

	Initial_X_Location += DeltaTime * Spawn_X_Velocity;
	Initial_Y_Location += DeltaTime * Spawn_Y_Velocity;
	this->SetActorRotation(FRotator(Initial_Rotation * 100.0f, Initial_Rotation * 50.0f, 0.0f));
	this->SetActorLocation(FVector(Initial_X_Location, Initial_Y_Location, 0.0f));

	//destroy hazard
	if (SelfDestructTimer<=0.0f)
	{
		this->Destroy();
	}
	if (bHit)
	{
		bHit = false;
		bStartDestroyTimer = true;
		ToyExplosionFX->Activate();
		ToyExplosionFX->SetWorldLocation(this->GetActorLocation());
		ToyExplosionFX->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		
		ToyExplosionSound->Activate();
		ToyExplosionSound->Play();

		this->StaticMesh->SetVisibility(false);
		this->SetActorEnableCollision(false);

		if (this->GetActorScale3D().X>6.0f)
		{
			SpawnChildren(FMath::FRandRange(1,3));
		}
	}
	if (bStartDestroyTimer)
	{
		SelfDestructTimer -= DeltaTime;
	}
}

void AHazard::SpawnChildren(int32 NumChildren)
{
	FActorSpawnParameters Params = {};
	Params.Owner = this;

	for ( int i = 0; i < NumChildren; i++)
	{
		if (ChildSpawn!=nullptr)
		{
			AHazard* NewHazard = Cast<AHazard>(GetWorld()->SpawnActor(ChildSpawn, new FVector(this->GetActorLocation()), new FRotator(this->GetActorRotation()), Params));
		
			NewHazard->Initial_X_Location = this->GetActorLocation().X;
			NewHazard->Initial_Y_Location = this->GetActorLocation().Y;

			NewHazard->SetHazardVelocity(FVector(FMath::RandRange(-250, 100), FMath::RandRange(-50, 50), 0.0f));
			
			float RandScale = FMath::RandRange(2, 5);

			NewHazard->SetActorScale3D(FVector(RandScale, RandScale, RandScale));
			NewHazard->SetActorEnableCollision(true);
			NewHazard->StaticMesh->SetVisibility(true);
		}
	}
}

void AHazard::SetHazardVelocity(FVector NewVelocity)
{
	Spawn_X_Velocity = NewVelocity.X;
	Spawn_Y_Velocity = NewVelocity.Y;
}

void AHazard::OnBeginOverlap(AActor* AsteroidActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Bounds"))
	{
		SelfDestructTimer=0.0f;
	}
	if (OtherActor->ActorHasTag("Projectile")|| OtherActor->ActorHasTag("Player"))
	{
		bHit = true;
	}
}
