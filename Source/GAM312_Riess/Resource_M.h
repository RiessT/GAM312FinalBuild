// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Resource_M.generated.h"

UCLASS()
class GAM312_RIESS_API AResource_M : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource_M();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Setting the name of the resource defaulted as Wood.
	UPROPERTY(EditAnywhere)
		FString resourceName = "Wood";
	
	//Amount of the resource that is being gathered when hit.
	UPROPERTY(EditAnywhere)
		int resourceAmount = 5;

	//Amount of total resources that can be gathered until empty.
	UPROPERTY(EditAnywhere)
		int totalResource = 100;

	//Text set up to be used in the renderer.
	UPROPERTY()
		FText tempText;

	//UPROPERTY for the render for the name of the resource.
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* ResourceNameTxt;

	//Setting up the mesh for the resources.
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

};
