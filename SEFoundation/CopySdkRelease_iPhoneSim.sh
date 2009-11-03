#!/bin/tcsh
mkdir -p ../SDK/Include
mkdir -p ../SDK/Lib

set DIRS = `ls`
foreach dir (${DIRS})
    if (-d $dir && $dir != SEFoundation.xcodeproj && $dir != build) then
        echo $dir
        cd $dir
        set CURDIR = `pwd`
        set HEADERS = `ls *.h *.inl`
        foreach header (${HEADERS})
            cp -fp "${CURDIR}"/$header ../../SDK/Include
        end
        cd ..
    endif
end

set RHEADERS = `ls *.h`
foreach rheader (${RHEADERS})
    cp -fp "${SRCROOT}"/$rheader ../SDK/Include
end

cp -fp "${SRCROOT}"/build/Default/libSEFoundation_iPhoneSim.a ../SDK/Lib/libSEFoundation_iPhoneSim.a
ranlib ../SDK/Lib/libSEFoundation_iPhoneSim.a
