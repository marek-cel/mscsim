del wix_data.wxs
del wix_osg_plugins.wxs

copy src/sim.ico sim.ico

call "%WIX%bin\heat.exe" dir ".\data" -gg -ke -sfrag -dr dir_mscsim -cg cg_mscsim_data -var var.SourceDir -template:fragment -out wix_data.wxs
call "%WIX%bin\heat.exe" dir "%OSG_ROOT%\bin\osgPlugins-3.4.0" -gg -ke -sfrag -dr dir_mscsim -cg cg_mscsim_osg_plugins -var var.SourceDir -template:fragment -out wix_osg_plugins.wxs

del Microsoft_VC90_CRT_x86.msm
copy "C:\Program Files (x86)\Common Files\Merge Modules\Microsoft_VC90_CRT_x86.msm" Microsoft_VC90_CRT_x86.msm

del wix_data.wixobj
del wix_main.wixobj
del wix_osg.wixobj
del wix_osg_plugins.wixobj
del wix_qt.wixobj

del mscsim-0.1.2-win32.msi
del mscsim-0.1.2-win32.wixpdb

call "%WIX%bin\candle.exe" -ext WiXUtilExtension -dSourceDir=".\data" wix_data.wxs
call "%WIX%bin\candle.exe" -ext WiXUtilExtension wix_main.wxs
call "%WIX%bin\candle.exe" -ext WiXUtilExtension wix_osg.wxs
call "%WIX%bin\candle.exe" -ext WiXUtilExtension -dSourceDir="%OSG_ROOT%\bin\osgPlugins-3.4.0" wix_osg_plugins.wxs
call "%WIX%bin\candle.exe" -ext WiXUtilExtension wix_qt.wxs

call "%WIX%bin\light.exe" -ext WiXUtilExtension -ext WixUIExtension -cultures:en-us -out mscsim-0.2-win32.msi wix_data.wixobj wix_main.wixobj wix_osg.wixobj wix_osg_plugins.wixobj wix_qt.wixobj

pause
