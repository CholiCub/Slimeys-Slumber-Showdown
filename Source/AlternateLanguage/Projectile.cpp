#include "Projectile.h"
#include "SS_PLayer.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ProjectileComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileComponent"));
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));

	ProjectileComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CapsuleCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ProjectileSpeed = 0.25f;
	bHit = false;
	bDestroy = false;
}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CurrentLocation = this->GetActorLocation();

	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector NewLocation = FVector(CurrentLocation.X - (ProjectileSpeed * DeltaTime), CurrentLocation.Y, CurrentLocation.Z);

	this->SetActorLocation(NewLocation);
	CurrentLocation = NewLocation;

	if (bHit)
	{
		this->Destroy();
	}
}

void AProjectile::OnBeginOverlap(AActor* ProjectileActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		ASS_Player* OwningPlayer = Cast<ASS_Player>(this->GetOwner());
		if (OtherActor->ActorHasTag(FName("Bounds")))
		{
			bHit=true;
		}
		if (this->ActorHasTag(FName("EnemyProjectile")) && OtherActor->ActorHasTag(FName("Player")))
		{
			bHit = true;
		}
		if (OtherActor->ActorHasTag(FName("Enemy")))
		{
			if (OwningPlayer)
			{
				OwningPlayer->PlayerScore += 50.0f;
				bHit = true;
			}
		}
		if (OtherActor->ActorHasTag(FName("Hazard")))
		{
			if (OwningPlayer)
			{
				OwningPlayer->PlayerScore += 10.0f;
				bHit = true;
			}
		}
		if (OtherActor->ActorHasTag(FName("BoundsRight")))
		{
			bHit = true;
		}

	}
}
