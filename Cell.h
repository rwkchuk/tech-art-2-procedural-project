// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Cell.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DUNG_API UCell : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		UCell* PreviousCell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		FVector2D Position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		FVector2D Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		FVector2D TileDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		bool IsMainPathCell;
	//This could be End, Cross, T, I, Corner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		FString TileType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon Generator")
		TArray<UCell*> ConnectingCells;
	
public:
	UCell();

	void SetPreviousCell(UCell* previous);
};
