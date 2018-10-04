// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"


UGrid::UGrid()
{
	//Position = FVector2D(2, 2);
	//Size = FVector2D(3, 3);
	//Need to multiply by hilbet n - this grids size so 8 - 4 or 16 - 4
	Position = FVector2D((int)(FMath::FRand() * 12), (int)(FMath::FRand() * 12));
	Size = FVector2D(3, 3);
}

void UGrid::AddCell(UCell* cell)
{
	///cell->PreviousCell = nullptr;
	cell->SetPreviousCell(nullptr);
	float distance = 0;
	
	//pop cells from main path until we have a neighbor
	for (int i = MainPath.Num() - 1; i >= 0; i--)
	{
		if (MainPath[i])
		{
			distance = FVector2D::Distance(cell->Position, MainPath[i]->Position);
			if (fabsf(distance) > 1)
			{
				//pop it from main path and add it to notmainpath
				MainPath[i]->IsMainPathCell = false;
				NotMainPath.Add(MainPath.Pop());
			}
			else
			{
				///cell->PreviousCell = MainPath[i];
				cell->SetPreviousCell(MainPath[i]);
				break;
			}
		}
		else
		{
			///cell->PreviousCell = nullptr;
			cell->SetPreviousCell(nullptr);
		}
	}
	
	//we are part of the main path
	cell->IsMainPathCell = true;

	MainPath.Add(cell);
}
// check rejects to see if they can conenct to main path
// if they don't remove them
void UGrid::CheckNotMainPath()
{
	float distance = 0;
	for (int j = NotMainPath.Num() - 1; j >= 0; j--)
	{
		for (int i = 0; i < MainPath.Num(); i++)
		{
			distance = FVector2D::Distance(NotMainPath[j]->Position, MainPath[i]->Position);
			if (fabsf(distance) > 1)
			{
				//UE_LOG(LogTemp, Error, TEXT(">>>> %f <<<<"), distance);
				///NotMainPath[j]->PreviousCell = nullptr;
				NotMainPath[j]->SetPreviousCell(nullptr);
			}
			else
			{
				///NotMainPath[j]->PreviousCell = MainPath[i];
				NotMainPath[j]->SetPreviousCell(MainPath[i]);
				break;
			}
		}

		if (NotMainPath[j]->PreviousCell == nullptr)
		{
			UCell* temp = NotMainPath[j];
			NotMainPath.RemoveAt(j);
			UE_LOG(LogTemp, Error, TEXT(">>>> %s <<<<"), *temp->GetFName().ToString());
		}
	}
}

// update each collection so they know directions (normalized) to previous cell
void UGrid::UpdateCollections()
{
	FVector2D UnitDirection;
	for (int i = MainPath.Num() - 1; i >= 0 ; i--)
	{
		UnitDirection = FVector2D::ZeroVector;
		//setting direction values for each cell
		if (MainPath[i]->PreviousCell)
		{
			UnitDirection = MainPath[i]->Position - MainPath[i]->PreviousCell->Position;
			UnitDirection.Normalize();
			MainPath[i]->Direction = -1.0f * UnitDirection;
			MainPath[i]->TileDirection = MainPath[i]->Direction;
		}
		else
		{
			//what direction if we have no direction!
			//mark it for later?
		}

		//decide what type of tile the cell should be
		MainPath[i]->TileType = "I";
		if (MainPath[i]->ConnectingCells.Num() == 1)
		{
			//should be end
			if (!MainPath[i]->PreviousCell)
			{
				MainPath[i]->TileType = "End";
				//update direction to be opposite of conencting cell

				///MainPath[i]->Direction = -1.0f * MainPath[i]->ConnectingCells[0]->Direction;
				UnitDirection = MainPath[i]->Position - MainPath[i]->ConnectingCells[0]->Position;
				UnitDirection.Normalize();
				MainPath[i]->TileDirection = -1.0f * UnitDirection;
				
			}
			else if(MainPath[i]->PreviousCell == MainPath[i]->ConnectingCells[0])
			{
				MainPath[i]->TileType = "End";
				///MainPath[i]->Direction = 1.0f * MainPath[i]->ConnectingCells[0]->Direction;
				UnitDirection = MainPath[i]->Position - MainPath[i]->ConnectingCells[0]->Position;
				UnitDirection.Normalize();
				MainPath[i]->TileDirection = -1.0f * UnitDirection;
			}
			else
			{
				//TODO is this ever possible?
				MainPath[i]->TileType = "I";
			}
		}
		else if (MainPath[i]->ConnectingCells.Num() == 2)
		{
			//either a cornor or a straight peice
			MainPath[i]->TileType = "I";
			float dot = FVector2D::DotProduct(MainPath[i]->Direction, MainPath[i]->ConnectingCells[1]->Direction);
			UE_LOG(LogTemp, Error, TEXT(">>>> %s : %f : %s <<<<"), *MainPath[i]->GetFName().ToString(), dot, *MainPath[i]->ConnectingCells[1]->GetFName().ToString());
			if (dot > 0 || dot < 0)
			{
				MainPath[i]->TileType = "I";
				//so direction...
				
			}
			else
			{
				MainPath[i]->TileType = "L";

				///WARNING
				//TODO Fix coordinate system from this to world. FOr some reason the x and y get flipped so 1,0 is actually 0,1!
				if (MainPath[i]->ConnectingCells[1]->Direction == FVector2D(1, 0))
				{
					if (MainPath[i]->Direction == FVector2D(0, 1))
					{///
						MainPath[i]->TileDirection = FVector2D(0,1);
					}
					else if(MainPath[i]->Direction == FVector2D(0, -1))
					{///
						MainPath[i]->TileDirection = FVector2D(-1, 0);
					}
				}
				else if (MainPath[i]->ConnectingCells[1]->Direction == FVector2D(-1, 0))
				{
					if (MainPath[i]->Direction == FVector2D(0, -1))
					{///
						MainPath[i]->TileDirection = FVector2D(0, -1);
					}
					else if (MainPath[i]->Direction == FVector2D(0, 1))
					{///
						MainPath[i]->TileDirection = FVector2D(1, 0);
					}
				}
				else if (MainPath[i]->ConnectingCells[1]->Direction == FVector2D(0, 1))
				{
					if (MainPath[i]->Direction == FVector2D(-1, 0))
					{///
						MainPath[i]->TileDirection = FVector2D(-1, 0);
					}
					else if (MainPath[i]->Direction == FVector2D(1, 0))
					{///
						MainPath[i]->TileDirection = FVector2D(0, -1);
					}
				}
				else if (MainPath[i]->ConnectingCells[1]->Direction == FVector2D(0, -1))
				{
					if (MainPath[i]->Direction == FVector2D(1, 0))
					{///
						MainPath[i]->TileDirection = FVector2D(1, 0);
					}
					else if (MainPath[i]->Direction == FVector2D(-1, 0))
					{///
						MainPath[i]->TileDirection = FVector2D(0, 1);
					}
				}
			}
		}
		else if(MainPath[i]->ConnectingCells.Num() == 3)
		{
			//T
			MainPath[i]->TileType = "T";
			int Ys = 0;
			int Xs = 0;
			for (int j = 0; j < MainPath[i]->ConnectingCells.Num(); j++)
			{
				if (MainPath[i]->Position.X == MainPath[i]->ConnectingCells[j]->Position.X)
				{
					Xs++;
				}
				if (MainPath[i]->Position.Y == MainPath[i]->ConnectingCells[j]->Position.Y)
				{
					Ys++;
				}
			}
			if (Ys > Xs)
			{
				//the one that had the same x is the direction we face
				for (int k = 0; k < MainPath[i]->ConnectingCells.Num(); k++)
				{
					if (MainPath[i]->ConnectingCells[k]->Position.X == MainPath[i]->Position.X)
					{
						//then this is the position and direction we should update to
						UnitDirection = MainPath[i]->Position - MainPath[i]->ConnectingCells[k]->Position;
						UnitDirection.Normalize();
						MainPath[i]->TileDirection = -1.0f * UnitDirection;
					}
				}
			}
			else
			{
				for (int k = 0; k < MainPath[i]->ConnectingCells.Num(); k++)
				{
					if (MainPath[i]->ConnectingCells[k]->Position.Y == MainPath[i]->Position.Y)
					{
						//then this is the position and direction we should update to
						UnitDirection = MainPath[i]->Position - MainPath[i]->ConnectingCells[k]->Position;
						UnitDirection.Normalize();
						MainPath[i]->TileDirection = -1.0f * UnitDirection;
					}
				}
			}

		}
		else
		{
			//cross
			MainPath[i]->TileType = "X";
		}


	}
	for (int i = 0; i < NotMainPath.Num(); i++)
	{
		if (NotMainPath[i]->PreviousCell)
		{
			UnitDirection = NotMainPath[i]->Position - NotMainPath[i]->PreviousCell->Position;
			UnitDirection.Normalize();
			NotMainPath[i]->Direction = -1.0f * UnitDirection;
			NotMainPath[i]->TileDirection = NotMainPath[i]->Direction;
		}

		//if it's not on the main path it'll always be a dead end
		NotMainPath[i]->TileType = "End";
	}
}
//

