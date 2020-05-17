add_library( nav STATIC
    nav_DataBase.cpp
    nav_Frequency.cpp
    nav_Manager.cpp
)

qt5_use_modules( nav Xml )