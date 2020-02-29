call run_wix_clean.bat

copy src\sim.ico sim.ico
copy C:\OpenAL\1.1\redist\oalinst.exe oalinst.exe

call "%WIX%bin\heat.exe" dir ".\data" -gg -ke -sfrag -dr dir_mscsim -cg cg_mscsim_data -var var.SourceDir -template:fragment -out wix_data.wxs
call "%WIX%bin\heat.exe" dir "%OSG_ROOT%\bin\osgPlugins-3.4.0" -gg -ke -sfrag -dr dir_mscsim -cg cg_mscsim_osg_plugins -var var.SourceDir -template:fragment -out wix_osg_plugins.wxs

copy "C:\Program Files (x86)\Common Files\Merge Modules\Microsoft_VC90_CRT_x86.msm" Microsoft_VC90_CRT_x86.msm

call "%WIX%bin\candle.exe" -ext WiXUtilExtension wix_alut.wxs
call "%WIX%bin\candle.exe" -ext WiXUtilExtension -dSourceDir=".\data" wix_data.wxs
call "%WIX%bin\candle.exe" -ext WiXUtilExtension wix_main.wxs
call "%WIX%bin\candle.exe" -ext WiXUtilExtension wix_osg.wxs
call "%WIX%bin\candle.exe" -ext WiXUtilExtension -dSourceDir="%OSG_ROOT%\bin\osgPlugins-3.4.0" wix_osg_plugins.wxs
call "%WIX%bin\candle.exe" -ext WiXUtilExtension wix_qt.wxs
call "%WIX%bin\light.exe" -ext WiXUtilExtension -ext WixUIExtension -cultures:en-us -out mscsim-0.3-win32.msi wix_alut.wixobj wix_data.wixobj wix_main.wixobj wix_osg.wixobj wix_osg_plugins.wixobj wix_qt.wixobj

call run_wix_bundle.bat

pause
