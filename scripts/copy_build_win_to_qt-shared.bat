rem run from scripts dir
if "%PS_QT%"=="" set PS_QT=C:\PS_QT
xcopy /s /y ..\include %PS_QT%\qt-shared\libs\libdatachannel\include\
xcopy /s /y ..\deps\json\single_include %PS_QT%\qt-shared\libs\libdatachannel\deps\json\single_include\
xcopy /s /y ..\deps\plog\include %PS_QT%\qt-shared\libs\libdatachannel\deps\plog\include\

xcopy /y ..\..\libdatachannel_build_debug_static_openssl_debug\datachannel.dll %PS_QT%\qt-shared\libs\libdatachannel\build\win\debug\
xcopy /y ..\..\libdatachannel_build_debug_static_openssl_debug\datachannel.lib %PS_QT%\qt-shared\libs\libdatachannel\build\win\debug\
xcopy /y ..\..\libdatachannel_build_debug_static_openssl_debug\datachannel.pdb %PS_QT%\qt-shared\libs\libdatachannel\build\win\debug\
.
xcopy /y ..\..\libdatachannel_build_relsym_static_openssl\datachannel.dll %PS_QT%\qt-shared\libs\libdatachannel\build\win\release\
xcopy /y ..\..\libdatachannel_build_relsym_static_openssl\datachannel.lib %PS_QT%\qt-shared\libs\libdatachannel\build\win\release\
xcopy /y ..\..\libdatachannel_build_relsym_static_openssl\datachannel.pdb %PS_QT%\qt-shared\libs\libdatachannel\build\win\release\
