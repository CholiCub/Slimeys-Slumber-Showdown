// Fill out your copyright notice in the Description page of Project Settings.

#define printf(text, string) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text),fstring));
#include "Trigger.h"
#include"DrawDebugHelpers.h"

// Sets default values
ATrigger::ATrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &ATrigger::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ATrigger::OnEndOverlap);
}

// Called when the game starts or when spawned
void ATrigger::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, -1, 0, 5);
}

// Called every frame
void ATrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrigger::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor&&(OtherActor!=this)&& OtherActor==SpecificActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Text, %S %S "), "Enter: ", * OtherActor->GetName());
	}
}

void ATrigger::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this) && OtherActor == SpecificActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Text, %S %S "), "Exit: ", *OtherActor->GetName());

	}
}

