// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Grid.h"
#include "Hilbert.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LayoutGenerator.generated.h"

UCLASS()
class DUNG_API ALayoutGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layout Generator")
		UGrid* Layout;
	UPROPERTY()
	UHilbert* hilbert;
public:	
	// Sets default values for this actor's properties
	ALayoutGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Layout Generator")
	void Test();

	UFUNCTION(BlueprintCallable, Category = "Layout Generator")
	void Spawn();

	//Make grids until I have one that has the right number of dead cells

	//Populate the world with the data we have
	UFUNCTION(BlueprintCallable, Category = "Layout Generator")
		void PopulateGrid();

	//load sub level and position it
	UFUNCTION()
	void LoadTile(FName TileToLoad, FTransform TileTransform);
};
