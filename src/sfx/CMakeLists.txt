qt4_wrap_cpp( MOC_SRCS
    sfx_Thread.h
)

################################################################################

add_library( sfx STATIC
    sfx_Manager.cpp
    sfx_Sample.cpp
    sfx_Thread.cpp
    
    ${MOC_SRCS}
)
