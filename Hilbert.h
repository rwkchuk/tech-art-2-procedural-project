// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Hilbert.generated.h"

/**
 * 
 */
UCLASS()
class DUNG_API UHilbert : public UObject
{
	GENERATED_BODY()
	
		/// hilbert curve functions
public:
	//rotate the hilbert curve section
	UFUNCTION(BlueprintCallable, Category = "Hilbert")
		void rot(int n, int& x, int& y, int rx, int ry);

	//take 1 dimension hilbert int and transfer it into a 2 dimension coordinate
	UFUNCTION(BlueprintCallable, Category = "Hilbert")
		void d2xy(int n, int d, int& x, int& y);

	//take 2 dimension coordinate and transfer it into a 1 dimension hilbert int
	UFUNCTION(BlueprintCallable, Category = "Hilbert")
		int xy2d(int n, int x, int y);
	
	
};
