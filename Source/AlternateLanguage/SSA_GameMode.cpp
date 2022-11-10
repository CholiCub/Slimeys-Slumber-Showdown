#include "SSA_GameMode.h"


ASSA_GameMode::ASSA_GameMode()
{
	SpawnTimer = 0.0f;
	MusicTrack = CreateDefaultSubobject<UAudioComponent>(TEXT("Music"));
	MusicTrack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HazardSpawnLoc = FVector(1000.0f, 1000.0f, 1000.0f);
	HazardSpawnRot = FRotator(0.0f, 0.0f, 0.0f);

	ThisWorld = GetWorld();
}
void ASSA_GameMode::BeginPlay() {
		Super::BeginPlay();

		MusicTrack->Play();

		bPlayerDead = false;

		if (Score_Widget_Class != nullptr) {
			Score_Widget = CreateWidget<UUserWidget>(GetWorld(), Score_Widget_Class);
			Score_Widget->AddToViewport();
		}

		if (Sleep_And_Sugar_Widget_Class != nullptr) {
			Sleep_Sugar_Widget = CreateWidget<UUserWidget>(GetWorld(), Sleep_And_Sugar_Widget_Class);
			Sleep_Sugar_Widget->AddToViewport();
		}

		if (Restart_Widget_Class != nullptr) {
			Restart_Widget = CreateWidget<UUserWidget>(GetWorld(), Restart_Widget_Class);
			Restart_Widget->AddToViewport();

			Restart_Widget->SetVisibility(ESlateVisibility::Hidden);
		}

		PC_Ref = GetWorld()->GetFirstPlayerController();
}

void ASSA_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnTimer = FMath::RandRange(0, 500);

	if (Enemy)
	{
		if (SpawnTimer>440)
		{
			FVector EnemySpawnLoc(0.0f, 0.0f, 0.0f);
			FRotator EnemySpawnRot(0.0f, 0.0f, 0.0f);

			AActor* NewEnemy = GetWorld()->SpawnActor(Enemy, &EnemySpawnLoc, &EnemySpawnRot, HazardSpawnParameters);

			if (NewEnemy)
			{
				NewEnemy->SetActorLocation(FVector(1000.0f, FMath::RandRange(-500, 500), 0.0f));
			}
		}
	}
	if (HazardTemplate)
	{
		if (SpawnTimer>450)
		{
			AActor* NewHazard=GetWorld()->SpawnActor(HazardTemplate, &HazardSpawnLoc, &HazardSpawnRot, HazardSpawnParameters);

			if (NewHazard)
			{
				NewHazard->SetActorLocation(FVector(1000.0f, 1000.0f, 1000.0f));
			}
		}
	}

	if (bPlayerDead)
	{
		Restart_Widget->SetVisibility(ESlateVisibility::Visible);

		PC_Ref->bShowMouseCursor = true;

		//bPlayerDead = false;
	}
}

void ASSA_GameMode::RestartLevel(FName LevelName)
{
}
