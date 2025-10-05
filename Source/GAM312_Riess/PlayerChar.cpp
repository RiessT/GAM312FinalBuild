// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initial Camera Component Setup.
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	//Attaching Camera to the Player Mesh at the head.
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	//Using the controllers rotation to change the rotation of the camera also.
	PlayerCamComp->bUsePawnControlRotation = true;

	//Setting the number of objects in the building array to the correct number of buildings.
	BuildingArray.SetNum(3);

	//Setting the number of objects in the Array and then matching the corresponding resource to the location it is in.
	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));


}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	//Creating a FTimerHandle variable named StatsTimerHandle.
	FTimerHandle StatsTimerHandle;
	//Setting up the world timer to call the DecreaseStats function every 2 seconds in the game.
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &APlayerChar::DecreaseStats, 2.0f, true);
	
	//Setting the values of the objectives to 0 off the start.
	if (objWidget)
	{
		objWidget->UpdatebuildObj(0.0f);
		objWidget->UpdatematOBJ(0.0f);
	}
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Calling the UpdateBars function every tick and dispalying it to the playerUI.
	playerUI->UpdateBars(Health, Hunger, Stamina);

	//Checking if the character is building a building.
	if (isBuilding)
	{
		//Creating a trace to start building an object 400 units away from the character.
		if (spawnedPart)
		{
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			spawnedPart->SetActorLocation(EndLocation);
		}
	}
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

	//Setting up the binding and function for the interact and rotate control.
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject);
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding);

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
	//Setting up the line trace to detect 800 units in from of the camers current direction and ending there.
	FHitResult HitResult;
	FVector StartLocation = PlayerCamComp->GetComponentLocation();
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	//Setting up the collision to make the line trace ignore the player and show complex collision and the face values.
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	//Making it so the trace event does not occur if the character is building something.
	if (!isBuilding)
	{
		//Casting the line trace and then detecting if it collided with anything and if it did, getting that actor.
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

			//Checking if the object hit by the trace is a valid object and then if it is subtracting the resources from the total amount allowed for that given resource.
			if (Stamina > 5.0f)
			{
				if (HitResource)
				{
					FString hitName = HitResource->resourceName;
					int resourceValue = HitResource->resourceAmount;

					HitResource->totalResource = HitResource->totalResource - resourceValue;

					if (HitResource->totalResource > resourceValue)
					{
						GiveResource(resourceValue, hitName);

						//Adding the amount collected to the amount the player had before.
						matsCollected = matsCollected + resourceValue;

						//Updating the widget with the amount collected.
						objWidget->UpdatematOBJ(matsCollected);

						//Checking if the ptr is null and outputting to the screen that the resource was collected.
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

						//Spawning the hit decal at the location hit.
						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);

						//Decreasing the stamina by 5 every action.
						SetStamina(-5.0f);
					}
					else
					{
						//Destroying the resource hit after it is empty and then outputting that the resource was depleted.
						HitResource->Destroy();
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
					}
				}
			}

		}
	}

	//Sets the isBuilding bool to false if it is set to true after the tick event ends.
	else
	{
		isBuilding = false;

		//Updating the number of objects built for the objective.
		objectsBuilt = objectsBuilt + 1.0f;
		objWidget->UpdatebuildObj(objectsBuilt);

	}

	
}

//Calling the SetHealth function and checking if the Health is already past the cap of 100 to run the statement.
void APlayerChar::SetHealth(float amount)
{
	if (Health + amount < 100)
	{
		Health = Health + amount;
	}
}

//Calling the SetHunger function and checking if the Hunger is already past the cap of 100 to run the statement.
void APlayerChar::SetHunger(float amount)
{
	if (Hunger + amount < 100)
	{
		Hunger = Hunger + amount;
	}
}


//Calling the SetStamina function and checking if the Stamina is already past the cap of 100 to run the statement.
void APlayerChar::SetStamina(float amount)
{
	if (Stamina + amount < 100)
	{
		Stamina = Stamina + amount;
	}
}

//Calling the DecreaseStats function and which tests if the hunger should be decreased and if the health should be decreased based of hunger.
void APlayerChar::DecreaseStats()
{
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}

	SetStamina(10.0f);

	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}
}
//Calling the GiveResource function and checking which resource should be given based on the object hit.
void APlayerChar::GiveResource(float amount, FString resourceType)
{
	if (resourceType == "Wood")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}

	if (resourceType == "Stone")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}

	if (resourceType == "Berry")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

//Defining the UpdateResources() function which takes the building that is chosen and subtracts the resources used according to the cost and adds to the number of buildings in inv.
void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject)
{
	if (woodAmount <= ResourcesArray[0])
	{
		if (stoneAmount <= ResourcesArray[1])
		{
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;

			if (buildingObject == "Wall")
			{
				BuildingArray[0] = BuildingArray[0] + 1;
			}

			if (buildingObject == "Floor")
			{
				BuildingArray[1] = BuildingArray[1] + 1;
			}

			if (buildingObject == "Ceiling")
			{
				BuildingArray[2] = BuildingArray[2] + 1;
			}
		}
	}
}

//Defining the SpawnBuilding() function that creates a line trace in front of the player to place the building and checks if the user has the required buildings and then places if available.
void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess)
{
	if (!isBuilding)
	{
		if (BuildingArray[buildingID] >= 1)
		{
			isBuilding = true;
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			FRotator myRot(0, 0, 0);

			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;

			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);

			isSuccess = true;
		}

		isSuccess = false;
	}
}

//Defining the RotateBuilding() function which rotates the building 90 degrees if the button is pressed.
void APlayerChar::RotateBuilding()
{
	if (isBuilding)
	{
		spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}

