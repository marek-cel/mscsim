add_library( sfx STATIC
    sfx_Manager.cpp
    sfx_Sample.cpp
    sfx_Thread.cpp
    
    sfx_Thread.h
)

qt5_use_modules( sfx Core )