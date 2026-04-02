// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "PlayerWidget.h"
#include "ObjectiveWidget.h"
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

	//UProperty for the hit sound effect.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		USoundBase* soundToPlay;

		//UProperty for the hit sound effect.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
		USoundBase* hurtSound;

	//Creating a UPROPERTY for the health player stat initialized as a float with default of 100.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Health = 100.0f;

	//Creating a UPROPERTY for the hunger player stat initialized as a float with default of 100.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Hunger = 100.0f;

	//Creating a UPROPERTY for the stamina player stat initialized as a float with default of 100.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float Stamina = 100.0f;
	
	//Creating a UPROPERTY for the Wood resource as an int.
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Wood;

	//Creating a UPROPERTY for the Stone resource as an int.
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Stone;

	//Creating a UPROPERTY for the Berry resource as an int.
	UPROPERTY(EditAnywhere, Category = "Resources")
		int Berry;

	//Creating a UPROPERTY for an array of the resources that are in the game.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<int> ResourcesArray;

	//Creating a UPROPERTY for the array of the resources names.
	UPROPERTY(EditAnywhere, Category = "Resources")
		TArray<FString> ResourcesNameArray;

	//Creating a UPROPERTY for the hit marker.
	UPROPERTY(EditAnywhere, Category = "HitMarker")
		UMaterialInterface* hitDecal;

	//Creating a UPROPERTY array for the Building Array.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies")
		TArray<int> BuildingArray;

	//Creating a UPROPERTY for the bool isBuilding.
	UPROPERTY()
		bool isBuilding;

	//Creating a UPROPERTY for the subclass building part.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<ABuildingPart> BuildPartClass;
		
	//Using the subclass building part to create a variable spawnedPart.
	UPROPERTY()
	ABuildingPart* spawnedPart;
	
	//Creating a UPROPERTY using the playerwidget header to create a variable playerUI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPlayerWidget* playerUI;

	//Creating a UPROPERTY for the widget named objWidget.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UObjectiveWidget* objWidget;

	//Creating a UPROPERTY variable named objectsBuilt.
	UPROPERTY()
		float objectsBuilt;

	//Creating a UPROPERTY variable named matsCollected.
	UPROPERTY()
		float matsCollected;


	//Creating the UFUNCTION as a callable blueprint to be able to set the health by an amount.
	UFUNCTION(BlueprintCallable)
		void SetHealth(float amount);
	
	//Creating a UFUNCTION to be callable and be able to set the hunger by an amount.
	UFUNCTION(BlueprintCallable)
		void SetHunger(float amount);

	//Creating a UFUNCTION to be callable and able to set the stamina by an amount.
	UFUNCTION(BlueprintCallable)
		void SetStamina(float amount);

	//Creating a UFUNCTION that decreases all of these stats at a certain interval in the game.
	UFUNCTION()
		void DecreaseStats();

	//Creating the UFUNCTION for the GiveResource function that gets the amount and type of resource that was hit.
	UFUNCTION()
		void GiveResource(float amount, FString resourceType);

	//Creating a UFUNCTION to detect how much of each resource is needed.
	UFUNCTION(BlueprintCallable)
		void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);
	
	//Creating a UFUNCTION to spawn the correct building.
	UFUNCTION(BlueprintCallable)
		void SpawnBuilding(int buildingID, bool& isSuccess);

	//Creating a UFUNCTION to rotate the building.
	UFUNCTION()
		void RotateBuilding();
};
