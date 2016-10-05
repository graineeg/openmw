# local files
set(JNI_ANDROID
    vmstorage
    androidcontrolshider.cpp
    android_commandLine.cpp
    android_main.c
)

add_library(jni_android
        STATIC
        ${JNI_ANDROID}
)
set_target_properties(jni_android PROPERTIES LINKER_LANGUAGE CXX)
