// copyright Paradise_NiseMono


#include "UI/Widget/PhosphorUserWidget.h"

void UPhosphorUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
