# Installation

- Create a `NppOpenPath` folder in the `plugins` folder
- Copy the `NppOpenPath.dll` into the folder

- Add the following line in the `contextMenu.xml` file:
```xml
<Item PluginEntryName="Open Path" PluginCommandItemName="Open Path" ItemNameAs="Open Path" />
```

- Or replace the line:
```xml
<Item MenuEntryName="Edit" MenuItemName="Open File"/>
```
by:

```xml
<Item PluginEntryName="Open Path" PluginCommandItemName="Open Path" ItemNameAs="Open Path" />
```

