// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initial Camera Component Setup.
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Can"));

	//Attaching Camera to the Player Mesh at the head.
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	//Using the controllers rotation to change the rotation of the camera also.
	PlayerCamComp->bUsePawnControlRotation = true;

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Setting the bindings and functions for the movement and looking controls.
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);

	//Setting up the bindings and functions for the jump controls.
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);

	//Setting up the binding and function for the interact control.
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);

}

//Call for the MoveForward function.
void APlayerChar::MoveForward(float axisValue)
{
	//Takes the vector and rotation of the X axis and adds that axis value to the Movement input of the corresponding axis direction for the char.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

//Call the MoveRight function.
void APlayerChar::MoveRight(float axisValue)
{
	//Takes the vector and rotation of the X axis and adds that axis value to the Movement input of the corresponding axis direction for the char.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}

//Call the StartJump function.
void APlayerChar::StartJump()
{
	//Using Unreal's built in jump to set the value to true to start the jump.
	bPressedJump = true;
}

//Call the StopJump function.
void APlayerChar::StopJump()
{
	//Using Unreal's built in jump to set the value to false to stop the jumping.
	bPressedJump = false;
}

//Call the FindObject function.
void APlayerChar::FindObject()
{
}

