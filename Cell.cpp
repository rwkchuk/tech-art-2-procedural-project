// Fill out your copyright notice in the Description page of Project Settings.

#include "Cell.h"


UCell::UCell()
{
	PreviousCell = nullptr;
	Position = FVector2D(0, 0);
	Direction = FVector2D(1, 0);
	TileDirection = Direction;
	IsMainPathCell = true;
	TileType = "End";
}

void UCell::SetPreviousCell(UCell* previous)
{
	PreviousCell = previous;
	if (PreviousCell)
	{
		PreviousCell->ConnectingCells.Add(this);
		//TODO maybe don't check?
		if(!ConnectingCells.Contains(PreviousCell))
			ConnectingCells.Add(PreviousCell);
	}
}

