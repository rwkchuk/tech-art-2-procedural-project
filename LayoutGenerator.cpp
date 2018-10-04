// Fill out your copyright notice in the Description page of Project Settings.

#include "LayoutGenerator.h"

#include "PackageName.h"
#include "Engine/LevelStreamingKismet.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"


// Sets default values
ALayoutGenerator::ALayoutGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Layout = NewObject<UGrid>();
}

// Called when the game starts or when spawned
void ALayoutGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALayoutGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALayoutGenerator::Test()
{
	Layout = NewObject<UGrid>();
	int d = 0;
	int x = 0;
	int y = 0;
	
	while (true)
	{
		Layout = NewObject<UGrid>();
		for (int j = 0; j < 16; j++)
		{
			for (int i = 0; i < 16; i++)
			{
				hilbert->d2xy(16, d, x, y);


				UE_LOG(LogTemp, Warning, TEXT("d %d = x %d : y %d"), d, x, y);
				//layoutGrid.Path.Add(FVector2D(x, y));
				if (Layout)
				{
					if (Layout->Position.X <= x && Layout->Position.Y <= y && Layout->Position.X + Layout->Size.X >= x && Layout->Position.Y + Layout->Size.Y >= y)
					{
						UE_LOG(LogTemp, Error, TEXT("d %d = x %d : y %d ... "), d, x, y);
						//UPROPERTY()
						UCell* cell = NewObject<UCell>();
						cell->Position = FVector2D(x, y);
						//Can this cell connect to the last cell?
						//check distance
						////if yes then it's main path
						////else it's not main path and should be discarded


						Layout->AddCell(cell);
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT(">>>> Layout Invalid <<<<"));
				}

				d++;
			}
		}
		Layout->CheckNotMainPath();
		if (Layout->NotMainPath.Num() >= 2)
			break;
	}
	Layout->UpdateCollections();
}

void ALayoutGenerator::Spawn()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> Cube(TEXT("/Game/BP_Cube.BP_Cube"));
	if (Cube.Object)
		TSubclassOf<class AActor> MyCube = (UClass*)Cube.Object->GeneratedClass;
	else
		UE_LOG(LogTemp, Error, TEXT(">>>> No Cube BP <<<<"));

}

//Don't look at this!!!
void ALayoutGenerator::PopulateGrid()
{
	FVector cellPosition;
	FVector previousCellPosition;
	FVector UnitDirection;
	FRotator XRotation = FRotator(0,0,0);
	FTransform TileTranform;

	float PositionScale = 200;
	float ZScale = 200;

	for (int i = 0; i < Layout->MainPath.Num(); i++)
	{
		cellPosition = FVector(Layout->MainPath[i]->Position * PositionScale, ZScale);
		if (Layout->MainPath[i]->PreviousCell)
		{
			previousCellPosition = FVector(Layout->MainPath[i]->PreviousCell->Position * PositionScale, ZScale);
			UnitDirection = cellPosition - previousCellPosition;
			UnitDirection.Normalize();
			XRotation = UnitDirection.Rotation();

			
		}

		TileTranform = FTransform(XRotation, cellPosition, FVector(1, 1, 1));

		LoadTile("", TileTranform);
	}
	for (int i = 0; i < Layout->NotMainPath.Num(); i++)
	{

	}
}

void ALayoutGenerator::LoadTile(FName TileToLoad, FTransform TileTransform)
{
	TileToLoad = "TestTile";
	if (TileToLoad != "")
	{
		//FLatentActionInfo LatenInfo;
		//UGameplayStatics::LoadStreamLevel(GetWorld(), TileToLoad, true, true, LatenInfo);

		//ULevelStreaming* level = nullptr; //= UGameplayStatics::GetStreamingLevel(this, TileToLoad);

		ULevelStreamingKismet* StreamingLevel = static_cast<ULevelStreamingKismet*>(NewObject<ULevelStreamingKismet>(GetWorld(), NAME_None, RF_NoFlags, NULL));

		// Associate a package name.
		StreamingLevel->SetWorldAssetByPackageName("/Game/Levels/Tiles/TestTile.TestTile");
		

		StreamingLevel->LevelColor = FColor::MakeRandomColor();
		StreamingLevel->bShouldBeLoaded = true;
		StreamingLevel->bShouldBeVisible = true;
		StreamingLevel->bShouldBlockOnLoad = false;
		StreamingLevel->bInitiallyLoaded = true;
		StreamingLevel->bInitiallyVisible = true;

		StreamingLevel->LevelTransform = TileTransform;

		StreamingLevel->PackageNameToLoad = "/Game/Levels/Tiles/TestTile.TestTile";
		
			FString PackageFileName;
		if (!FPackageName::DoesPackageExist(StreamingLevel->PackageNameToLoad.ToString(), NULL, &PackageFileName))
		{
			//UE_LOG(LogStreamingLevel, Error, TEXT("trying to load invalid level %s"), *StreamingLevel->PackageNameToLoad.ToString());
			UE_LOG(LogTemp, Error, TEXT(">>>> No Level Stream!! <<<<"));
			return;
		}

		StreamingLevel->PackageNameToLoad = FName(*FPackageName::FilenameToLongPackageName(PackageFileName));

		// Add the new level to world.
		GetWorld()->StreamingLevels.Add(StreamingLevel);

		
	}
}
