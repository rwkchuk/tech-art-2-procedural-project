// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cell.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Grid.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DUNG_API UGrid : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		FVector2D Position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		FVector2D Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		TArray<UCell*> MainPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		TArray<UCell*> NotMainPath;

public:	
	UGrid();

	void AddCell(UCell* cell);
	void CheckNotMainPath();
	void UpdateCollections();
};
