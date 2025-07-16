#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DronePawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class HW07_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	ADronePawn();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// --- Components ---
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USkeletalMeshComponent* MeshComponent;
    
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCameraComponent* CameraComponent;

	// --- Inputs ---
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FlyAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RollAction;
	

	// --- Movements ---
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 600.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float FlySpeed = 1200.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationSpeed = 100.f;
	UPROPERTY(EditAnywhere, Category = "Gravity")
	float Gravity = -980.f;
	
	// --- Internal Values ---
	bool bIsGrounded = true;
	FVector Velocity = FVector::ZeroVector;
	FVector CurrentMoveInput;
    FVector2D CurrentLookInput;
    float CurrentRollInput = 0.f;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Fly(const FInputActionValue& Value);
	void Roll(const FInputActionValue& Value);

	void HandleGroundCheck();
	void UpdateRotation(float DeltaTime);
	void UpdateMovement(float DeltaTime);
};
