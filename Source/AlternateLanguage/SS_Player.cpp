#include "SS_Player.h"
#include "SSA_GameMode.h"
#include "kismet/GameplayStatics.h"

// Sets default values
ASS_Player::ASS_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	SlimeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SlimeMesh"));
	ParticleSystems = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	ExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionFX"));
	DeathExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathExplosionSound"));

	//calling functions on a pointer
	//Keeping the transform relative to the parent - when moving the game wiull move with parent
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SlimeMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleSystems->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DeathExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Max_Velocity = 250.0f;
	Current_X_Velocity = 0.0f;
	Current_Y_Velocity = 0.0f;
	inZone = false;
	bIsFiring = false;
	WeaponFireRate = 0.25f;
	TimeSinceLastShot = 0.0f;

	PlayerScore = 0.0f;
}

void ASS_Player::MoveRigth(float AxisValue)
{
	Current_X_Velocity = Max_Velocity * AxisValue;
}

void ASS_Player::MoveUp(float AxisValue)
{
	Current_Y_Velocity = Max_Velocity * AxisValue;
}


void ASS_Player::StartThrowing()
{
	bIsFiring = true;
}

void ASS_Player::StopThrowing()
{
	bIsFiring = false;
}

void ASS_Player::ThrowWeapon()
{
	FActorSpawnParameters Params = {};
	Params.Owner = this;

	AActor* SpawnedProjectile = GetWorld()->SpawnActor(Projectile_BP, &Current_Location, &Current_Rotation, Params);
}


// Called when the game starts or when spawned
void ASS_Player::BeginPlay()
{
	Super::BeginPlay();
	//get current locvation
	Current_Location = this->GetActorLocation();
	Current_Rotation = this->GetActorRotation();
	bHit = false;
	bDead = false;

	ExplosionFX->Deactivate();
	DeathExplosionSound->Deactivate();

	Max_Health = 100.0f;
	Current_Health = 100.0f;

	Max_Armor = 100.0f;
	Current_Armor = 100.0f;

	OnActorBeginOverlap.AddDynamic(this, &ASS_Player::OnBeginOverlap);

}

// Called every frame
void ASS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Current_Armor -= DeltaTime;
	Max_Velocity = Current_Armor * 5;
	if (Current_Armor<30)
	{
		Max_Velocity = 150;
	}
	
	if (Current_X_Velocity!=0.0f || Current_Y_Velocity != 0.0f)
	{
		New_Location = FVector(Current_Location.X + (Current_X_Velocity * DeltaTime),
			Current_Location.Y + (Current_Y_Velocity * DeltaTime), 0);

		this->SetActorLocation(New_Location);
		Current_Location = New_Location;
	}
	if (Current_Y_Velocity > 0.1f)
	{
		this->SetActorRotation(FRotator(Current_Rotation.Pitch, Current_Rotation.Yaw + 45.0f, Current_Rotation.Roll));
	}
	else if (Current_Y_Velocity < -0.1f)
	{
		this->SetActorRotation(FRotator(Current_Rotation.Pitch, Current_Rotation.Yaw - 45.0f, Current_Rotation.Roll));
	}
	else
	{
		this->SetActorRotation(FRotator(Current_Rotation));
	}

	if (this->GetActorLocation().X < -Field_Width)
	{
		Current_Location = FVector(-Field_Width + 1, Current_Location.Y, Current_Location.Z);
	}
	if (this->GetActorLocation().X > Field_Width)
	{
		Current_Location = FVector(Field_Width - 1, Current_Location.Y, Current_Location.Z);
	}
	if (this->GetActorLocation().Y > Field_Height)
	{
		Current_Location = FVector(Current_Location.X, Field_Height-1, Current_Location.Z);
	}
	if (this->GetActorLocation().Y < -Field_Height)
	{
		Current_Location = FVector(Current_Location.X, -Field_Height + 1, Current_Location.Z);
	}

	//handle throw
	if (bIsFiring)
	{
		if (TimeSinceLastShot > WeaponFireRate)
		{
			ThrowWeapon();
			//throw rate
			TimeSinceLastShot = 0.0f;
		}
	}
	TimeSinceLastShot += DeltaTime;
	if (inZone)
	{
		Current_Health -= DeltaTime;
	}
	//Handle player hit
	if (bHit)
	{
		if (Current_Health >= 0) {
			bDead = true;
			this->SlimeMesh->SetVisibility(false);
			this->ParticleSystems->SetVisibility(false);
			this->ExplosionFX->Activate();
			this->DeathExplosionSound->Play();

			this->SetActorTickEnabled(false);
			ASSA_GameMode* GameModeRef = Cast<ASSA_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			GameModeRef->bPlayerDead = true;
		}
	}
}

// Called to bind functionality to input
void ASS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//axiz that are binding - functions will be called
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASS_Player::MoveRigth);
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ASS_Player::MoveUp);

	PlayerInputComponent->BindAction(FName("Throw"),IE_Pressed, this, &ASS_Player::StartThrowing);
	PlayerInputComponent->BindAction(FName("Throw"),IE_Released, this, &ASS_Player::StopThrowing);
}

void ASS_Player::CollectablePickup()
{
	UE_LOG(LogTemp, Warning, TEXT("COLLECTED PICKUP"));

	if (Current_Armor<100.0f )
	{
		Current_Armor += 100.0f;

		if (Current_Armor > 100.0f)
		{
			Current_Armor = 100.0f;
		}
	}
}
void ASS_Player::AreaOfEffectDamage(bool isZone)
{
	this->inZone = isZone;
}
void ASS_Player::OnBeginOverlap(AActor* PlayerActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Hazard")|| OtherActor->ActorHasTag("Enemy"))
	{
		if (Current_Health > 0.0f)
		{
			Current_Health -= 10.0f;
			if (Current_Health<=0.0f)
			{
				Current_Health = 0.0f;
				bHit = true;
			}
		}
	}
	if (OtherActor->ActorHasTag("EnemyProjectile"))
	{
		if (Current_Armor > 0.0f)
		{
			Current_Armor -= 25.0f;
			if (Current_Armor < 0.0f)
			{
				Current_Armor = 0.0f;
			}
		}
		else if (Current_Health > 0.0f)
		{
			Current_Health -= 10.0f;
			if (Current_Health <= 0.0f)
			{
				Current_Health = 0.0f;
				bHit = true;
			}
		}
	}
}
