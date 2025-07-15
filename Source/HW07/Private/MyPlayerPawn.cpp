#include "MyPlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AMyPlayerPawn::AMyPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetSimulatePhysics(false);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetSimulatePhysics(false);
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	MeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.0f;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AMyPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMyPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerPawn::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerPawn::Look);
	}
}

void AMyPlayerPawn::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	const FVector LocalMovementVector(MovementVector.X, MovementVector.Y, 0.f);
	
	AddActorLocalOffset(LocalMovementVector * MoveSpeed * DeltaTime);
}

void AMyPlayerPawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	AddActorLocalRotation(FRotator(0.f, LookAxisVector.X * RotationSpeed * DeltaTime, 0.f));

	const float NewPitch = SpringArmComponent->GetRelativeRotation().Pitch + (LookAxisVector.Y * -1.f * RotationSpeed * DeltaTime);
	const float ClampedPitch = FMath::Clamp(NewPitch, -60.f, 60.f); // 카메라 뒤집힘 방지
    
	SpringArmComponent->SetRelativeRotation(FRotator(ClampedPitch, 0.f, 0.f));
}