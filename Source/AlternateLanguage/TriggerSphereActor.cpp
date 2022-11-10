#include "TriggerSphereActor.h"
#include "SS_Player.h"
#include"DrawDebugHelpers.h"

// Sets default values
ATriggerSphereActor::ATriggerSphereActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RadiusSize = 400.f;

	SphereComponent= CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(RadiusSize);
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
	SphereComponent->SetupAttachment(RootComponent);
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATriggerSphereActor::OnBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATriggerSphereActor::OnEndOverlap);
}

// Called when the game starts or when spawned
void ATriggerSphereActor::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugSphere(GetWorld(), GetActorLocation(), RadiusSize, 10, FColor::FColor(179, 230, 255), true, 0, 0, 0);
}

// Called every frame
void ATriggerSphereActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ATriggerSphereActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyindex, bool bFromSweep, const FHitResult& SweepResults)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		SphereComponent->ToggleVisibility();
		UE_LOG(LogTemp, Warning, TEXT("Text, %S %S "), "ENTER: ", *OtherActor->GetName());
		ASS_Player* PlayerActor = Cast<ASS_Player>(OtherActor);
		PlayerActor->AreaOfEffectDamage(true);
	}
}

void ATriggerSphereActor::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		SphereComponent->ToggleVisibility();
		UE_LOG(LogTemp, Warning, TEXT("Text, %S %S "), "END: ", *OtherActor->GetName());

		ASS_Player* PlayerActor = Cast<ASS_Player>(OtherActor);
		PlayerActor->AreaOfEffectDamage(false);
	}
}
