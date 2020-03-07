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
      "@TargetDir@/mscsim.exe",
      "@StartMenuDir@/MScSim.lnk",
      "workingDirectory=@TargetDir@",
      "iconPath=@TargetDir@/mscsim.ico",
      "iconId=0",
      "description=MScSim"
    );
  }
}
