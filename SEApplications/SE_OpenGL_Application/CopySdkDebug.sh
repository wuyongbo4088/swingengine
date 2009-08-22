#!/bin/tcsh

set DIRS = `ls`
foreach dir (${DIRS})
    if (-d $dir && $dir != SE_Agl_Application.xcodeproj && $dir != build) then
        echo $dir
        cd $dir
        set CURDIR = `pwd`
        set HEADERS = `ls *.h *.inl`
        foreach header (${HEADERS})
            cp -fp "${CURDIR}"/$header ../../../SDK/Include
        end
        cd ..
    endif
end

set RHEADERS = `ls *.h`
foreach rheader (${RHEADERS})
    cp -fp "${SRCROOT}"/$rheader ../../SDK/Include
end

cp -fp "${SRCROOT}"/build/Default/libSEAglApplicationd.a ../../SDK/Lib/libSEAglApplicationd.a
ranlib ../../SDK/Lib/libSEAglApplicationd.a

