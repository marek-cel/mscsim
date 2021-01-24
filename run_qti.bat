set INSTALLER_NAME=mscsim-0.4-win32

del %INSTALLER_NAME%.exe
del qti\%INSTALLER_NAME%.exe

set mscsim_dir=qti\packages\pl.marekcel.mscsim

rmdir /s /q %mscsim_dir%\data\bin
rmdir /s /q %mscsim_dir%\data\data

mkdir %mscsim_dir%\data\bin
mkdir %mscsim_dir%\data\bin\osgPlugins-3.4.0
mkdir %mscsim_dir%\data\data

copy /y bin\mscsim.exe %mscsim_dir%\data\bin
copy /y src\mscsim.ico %mscsim_dir%\data\bin

xcopy /y data %mscsim_dir%\data\data /E

copy /y %QTDIR%\bin\QtCore4.dll %mscsim_dir%\data\bin
copy /y %QTDIR%\bin\QtGui4.dll %mscsim_dir%\data\bin
copy /y %QTDIR%\bin\QtNetwork4.dll %mscsim_dir%\data\bin
copy /y %QTDIR%\bin\QtOpenGL4.dll %mscsim_dir%\data\bin
copy /y %QTDIR%\bin\QtSvg4.dll %mscsim_dir%\data\bin
copy /y %QTDIR%\bin\QtXml4.dll %mscsim_dir%\data\bin

copy /y %OSG_ROOT%\bin\libpng15.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\libxml2.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osg.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osgDB.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osgGA.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osgParticle.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osgQt.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osgSim.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osgTerrain.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osgText.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osgUtil.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osgViewer.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\osg130-osgWidget.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\ot20-OpenThreads.dll %mscsim_dir%\data\bin
copy /y %OSG_ROOT%\bin\zlib1.dll %mscsim_dir%\data\bin

copy /y %OSG_ROOT%\bin\osgPlugins-3.4.0\*.dll %mscsim_dir%\data\bin\osgPlugins-3.4.0

copy /y C:\Windows\System32\OpenAL32.dll %mscsim_dir%\data\bin
copy /y %OPENAL_DIR%\bin\alut.dll %mscsim_dir%\data\bin

copy /y LICENSE-MIT.txt %mscsim_dir%\meta

cd qti

C:\Qt\QtIFW-3.1.0\bin\binarycreator.exe -c config\config.xml -p packages %INSTALLER_NAME%.exe

move %INSTALLER_NAME%.exe ..\%INSTALLER_NAME%.exe

pause
