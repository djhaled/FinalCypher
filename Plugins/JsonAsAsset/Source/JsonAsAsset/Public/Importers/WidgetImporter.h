// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Importer.h"

class UWidgetTree;
class UWidgetBlueprint;
class UWidget;
class UPanelSlot;
class UPanelWidget;
class UWidgetImporter : public IImporter {
public:
	UWidgetImporter(const FString& FileName, const FString& FilePath, const TSharedPtr<FJsonObject>& JsonObject, UPackage* Package, UPackage* OutermostPkg, const TArray<TSharedPtr<FJsonValue>>& AllJsonObjects):
		IImporter(FileName, FilePath, JsonObject, Package, OutermostPkg,AllJsonObjects) {
	}
	UWidgetTree* WidgetTree;
	UWidgetBlueprint* WidgetBlueprint;
	virtual bool ImportData() override;
	UObject* CreateEmptyWidget(UObject* Parent, FName Name, FName Type);
	void ProcessWidgetProperties(TSharedPtr<FJsonObject> WidgetProperties, FString& WidgetType, FName& WidgetName,  TSharedPtr<FJsonObject>& WidgetPropertiesObject);
	UObject* CreateEmptySlot(UObject* Parent, FName Name, FName Type);
	UPanelSlot* CreateSlot(TSharedPtr<FJsonValue> SlotProp, UObject* Parent, UPanelWidget* PanWid);
	UWidget* CreateContentWidget(TSharedPtr <FJsonObject> ContentProp);
	void test();
};
