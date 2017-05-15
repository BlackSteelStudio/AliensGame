// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "SoldierCameraManager.generated.h"

/**
*
*/
UCLASS()
class ALIENSPROJECT_API ASoldierCameraManager : public APlayerCameraManager
{
	GENERATED_UCLASS_BODY()

public:

	/** normal FOV */
	float NormalFOV;

	/** targeting FOV */
	float TargetingFOV;

	/** Update Camera Player */
	virtual void UpdateCamera(float DeltaTime) override;

};
