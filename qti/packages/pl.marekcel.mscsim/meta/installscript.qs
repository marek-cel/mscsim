function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
  component.createOperations();

  if ( systemInfo.productType === "windows" )
  {
    component.addOperation(
      "CreateShortcut",
      "@TargetDir@/bin/mscsim.exe",
      "@StartMenuDir@/MScSim.lnk",
      "workingDirectory=@TargetDir@/bin/",
      "iconPath=@TargetDir@/bin/mscsim.ico",
      "iconId=0",
      "description=MScSim"
    );
  }
}
