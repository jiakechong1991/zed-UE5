//======= Copyright (c) Stereolabs Corporation, All rights reserved. ===============

#pragma once

#include "Zed/Public/HUD/ZEDWidget.h"
#include "ZED/Public/Core/ZEDCamera.h"
#include "ZED/Public/Core/ZEDBaseTypes.h"

#include "CineCameraComponent.h"
#include "GameFramework/Pawn.h"

#include "ZEDPawn.generated.h"

/*
 * Base class for pawn using the Zed.
 * Inherit from this class and set the PawnClassType variable in the controller to spawn a pawn of your type.
 */
UCLASS(Category = "Stereolabs|Zed")
class ZED_API AZEDPawn : public APawn
{
	friend class AZEDPlayerController;  // 声明 AZEDPlayerController 为友元类，允许它访问 AZEDPawn 的私有成员

	GENERATED_BODY()

public:
	AZEDPawn();

private:
	/*
	 * Event binded to OnTrackingDataUpdated
	 * @param NewTrackingData The new tracking data
	 当ZED相机的追踪数据更新时调用的事件处理函数，用于处理新的追踪数据。
	 */
	UFUNCTION()
	void ZedCameraTrackingUpdated(const FZEDTrackingData& NewTrackingData);

	UFUNCTION()
	virtual void Tick(float DeltaSeconds) override;

	/*
	* Initialisation
	*/
	void InitRemap(FName HMDname, sl::RESOLUTION camRes, float dp);

public:
	/** Custom spring arm that offset the camera */
	UPROPERTY()
	USceneComponent* SpringArm;

	/** Main camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCineCameraComponent* Camera;  //定义了相机的类型

	/** Should enable lerp[平滑插值] with specified alpha*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool EnableLerp;  // 使能平滑插值

	/** Lerp Speed value*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LerpIntensity; // 平滑插值的速度

	/** Transform used during Lerp smoothing*/
	FTransform LerpTransform;  // 差值后的 位姿

	/** Boolean used as toggle[切换] to enable/disable freezing[冻结] the virtual camera to reposition the real camera*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ToggleFreeze;

	/** Save the rotation of the camera when freezing, and apply offset to the camera after unfreezing.*/
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseRotationOffset;

	/** Has the camera been frozen by the toggle freeze*/
	// 是否处于冻结状态  frozen[冻结]
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsFrozen;

	/** Set the location offset, i.e. when starting the level */
	UFUNCTION()
	void SetStartOffsetLocation(const FVector& locOffset);

	FVector StartOffsetLocation;  // 起始位置偏移量
  
	// If there is a translation modifier different than (1,1,1), it will also apply on the translation
	// given by the boolean "bSetFloorAsOrigin" of the tracking parameters, which will move the ZEDPawn
	// accordingly to the height of the real camera.
	// This bool is to ignore the translation modifier on the first offset.
	bool SetFloorAsOriginCorrected;
	
	/** The previous location given by the slcamera (not the last frame)*/
	FVector PreviousLocation; //上一帧的位置
	
	/** From Previous to current location given by the slcamera*/
	FVector PreviousToCurrentLocation; // 从上一帧到当前帧的位置差

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TranslationMultiplier;  //位置缩放因子

	FVector RealTranslationToVirtualTranslation(const FVector& realTranslation);
	/** The new virtual target location for lerp and direct movement*/
	FVector VirtualLocation;  //虚拟目标位置
	FVector PrevVirtualLocation; //上一帧的虚拟目标位置


	/** Zed loading widget */
	UPROPERTY()
	UZEDWidget* ZedLoadingWidget;  // 加载界面

	/** Zed error widget */
	UPROPERTY()
	UZEDWidget* ZedErrorWidget; //错误提示的UI界面

	/** Remap material resource */
	UPROPERTY()
	UMaterial* RemapSourceMaterial;

	/** Remap material*/
	UPROPERTY(BlueprintReadWrite, Category = "Zed|Rendering")
	UMaterialInstanceDynamic* RemapMaterialInstanceDynamic;

	/** Remap Mx */
	UPROPERTY()
	UTexture2D* RemapMx;

	/** Remap My */
	UPROPERTY()
	UTexture2D* RemapMy;

private:
	/** Zed loading source widget */
	UPROPERTY()
	UClass* ZedLoadingSourceWidget;

	/** Zed error source widget */
	UPROPERTY()
	UClass* ZedErrorSourceWidget;

	/** Zed widget material */
	UPROPERTY()
	UMaterial* ZedWidgetSourceMaterial;

	/** Transform offset used to reposition the camera, taking into account movement multipliers or offsetters*/
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform TransformOffset;

	/** Real camera position and rotation, to manipulate the camera with multiipliers or lerp or anything else.*/
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform RealCameraTransform;
};
