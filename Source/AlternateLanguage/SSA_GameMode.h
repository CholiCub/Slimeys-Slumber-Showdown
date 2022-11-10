#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "Enemy.h"
#include "Hazard.h"
#include "Blueprint/UserWidget.h"

#include "SSA_GameMode.generated.h"

UCLASS()
class ALTERNATELANGUAGE_API ASSA_GameMode : public AGameMode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy> Enemy;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHazard> HazardTemplate;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ATriggerSphereActor> Trigger;
	
	UPROPERTY(EditAnywhere)
		UAudioComponent* MusicTrack;

	ASSA_GameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	TSubclassOf<AHazard>GetHazardTemplate();

public:
	int32 SpawnTimer;
	bool bPlayerDead;

	UFUNCTION()
		void RestartLevel(FName LevelName);

	bool bStart;

protected:
	FActorSpawnParameters HazardSpawnParameters;
	FVector HazardSpawnLoc;
	FRotator HazardSpawnRot;

	UWorld* ThisWorld;

	//add widgets
	UPROPERTY(EditAnywhere, Category = "HUD Elements")
		TSubclassOf<UUserWidget> Score_Widget_Class;
	UUserWidget* Score_Widget;

	UPROPERTY(EditAnywhere, Category = "HUD Elements")
		TSubclassOf<UUserWidget> Sleep_And_Sugar_Widget_Class;
	UUserWidget* Sleep_Sugar_Widget;
	
	UPROPERTY(EditAnywhere, Category = "HUD Elements")
		TSubclassOf<UUserWidget> Restart_Widget_Class;
	UUserWidget* Restart_Widget;

	APlayerController* PC_Ref;
};
