// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "PlayerChar.generated.h"

UCLASS()
class GAM312_RIESS_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Creating a UFUNCTION to define the MoveForward function.
	UFUNCTION()
		void MoveForward(float axisValue);

	//Creating a UFUNCTION to define the MoveRight function.
	UFUNCTION()
		void MoveRight(float axisValue);

	//Creating a UFUNCTION to define the StartJump function.
	UFUNCTION()
		void StartJump();

	//Creating a UFUNCTION to define the StopJump function.
	UFUNCTION()
		void StopJump();

	//Creating a UFUNCTION to define the FindObject function.
	UFUNCTION()
		void FindObject();

	//Creating a UPROPERTY for the camera with visibleanywhere so the camera can be used in all blueprints.
	UPROPERTY(VisibleAnywhere)
	//Creating the variable for the camera names PlayerCamComp.
	UCameraComponent* PlayerCamComp;

};
