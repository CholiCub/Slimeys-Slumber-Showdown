#include "Pickup.h"
#include "SS_Player.h"

// Sets default values
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionMesh = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));

	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}


// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->SetActorLocation(FVector(this->GetActorLocation().X - (DeltaTime * 100.0f), this->GetActorLocation().Y, 0.0f));
}

void APickup::OnBeginOverlap(AActor* PickupActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ASS_Player* PlayerActor = Cast<ASS_Player>(OtherActor);
		PlayerActor->CollectablePickup();
		this->Destroy();
	}
	if (OtherActor->ActorHasTag("Bounds") || OtherActor->ActorHasTag("Player"))
	{
		this->Destroy();
	}
}
