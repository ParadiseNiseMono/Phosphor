// copyright Paradise_NiseMono


#include "UI/WidgetController/PhosphorWidgetController.h"

void UPhosphorWidgetController::SetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController=WCParams.PlayerController;
	PlayerState=WCParams.PlayerState;
	AbilitySystemComponent=WCParams.AbilitySystemComponent;
	AttributeSet=WCParams.AttributeSet;
}

void UPhosphorWidgetController::BroadcastInitialValues()
{
	
}

void UPhosphorWidgetController::BindCallbacksToDependencies()
{
	
}
