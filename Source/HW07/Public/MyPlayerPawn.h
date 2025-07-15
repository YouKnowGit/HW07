#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "MyPlayerPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class HW07_API AMyPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPlayerPawn();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	
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
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LookAction;

	// --- Movements ---
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 500.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationSpeed = 100.f;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
