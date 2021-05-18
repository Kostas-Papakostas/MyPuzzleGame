// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyPuzzleGameHUD.generated.h"

UCLASS()
class AMyPuzzleGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyPuzzleGameHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

