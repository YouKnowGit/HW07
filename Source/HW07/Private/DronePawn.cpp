#include "DronePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ADronePawn::ADronePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// --- 수동 회전 구현 ---
	bUseControllerRotationYaw = false;
	SpringArmComponent->bUsePawnControlRotation = false;
	CameraComponent->bUsePawnControlRotation = false;
}

void ADronePawn::BeginPlay()
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

void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADronePawn::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ADronePawn::Move);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADronePawn::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &ADronePawn::Look);
		
		EnhancedInputComponent->BindAction(FlyAction, ETriggerEvent::Triggered, this, &ADronePawn::Fly);
		EnhancedInputComponent->BindAction(FlyAction, ETriggerEvent::Completed, this, &ADronePawn::Fly);
		
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &ADronePawn::Roll);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Completed, this, &ADronePawn::Roll);
	}
}

void ADronePawn::Move(const FInputActionValue& Value)
{
	const FVector2D MoveValue = Value.Get<FVector2D>();
	CurrentMoveInput.X = MoveValue.X;
	CurrentMoveInput.Y = MoveValue.Y;
}

void ADronePawn::Look(const FInputActionValue& Value)
{
	CurrentLookInput = Value.Get<FVector2D>();
}

void ADronePawn::Fly(const FInputActionValue& Value)
{
	CurrentMoveInput.Z = Value.Get<float>();
}

void ADronePawn::Roll(const FInputActionValue& Value)
{
	CurrentRollInput = Value.Get<float>();
}

void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleGroundCheck();
	UpdateRotation(DeltaTime);
	UpdateMovement(DeltaTime);
}

void ADronePawn::HandleGroundCheck()
{
	FHitResult HitResult;
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, CapsuleComponent->GetScaledCapsuleHalfHeight() + 15.f);
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
    
	bIsGrounded = HitResult.bBlockingHit;
}

void ADronePawn::UpdateRotation(float DeltaTime)
{
	const float YawValue = CurrentLookInput.X * RotationSpeed * DeltaTime;
	const float PitchValue = CurrentLookInput.Y * RotationSpeed * DeltaTime;
	const float RollValue = CurrentRollInput * RotationSpeed * DeltaTime;
	
	AddActorLocalRotation(FRotator(PitchValue, YawValue, RollValue), true);
}

void ADronePawn::UpdateMovement(float DeltaTime)
{
	if (bIsGrounded)
	{
		if (CurrentMoveInput.Z > 0)
		{
			Velocity.Z = CurrentMoveInput.Z * FlySpeed;
		}
		else
		{
			Velocity.Z = 0;
		}
	}
	else
	{
		Velocity.Z += (CurrentMoveInput.Z * FlySpeed + Gravity) * DeltaTime;
	}

	const FRotator FlatRotation = FRotator(0.f, GetActorRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(FlatRotation).GetUnitAxis(EAxis::Y);
	const FVector RightDirection = FRotationMatrix(FlatRotation).GetUnitAxis(EAxis::X);
	const FVector HorizontalVelocity = (ForwardDirection * CurrentMoveInput.Y + RightDirection * CurrentMoveInput.X) * MoveSpeed;
	const FVector FinalVelocity = HorizontalVelocity + FVector(0.f, 0.f, Velocity.Z);
    
	AddActorWorldOffset(FinalVelocity * DeltaTime, true);
}