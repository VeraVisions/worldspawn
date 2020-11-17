radiant_plugin(mapq3
        parse.cpp parse.h
        plugin.cpp
        write.cpp write.h
        )

target_include_directories(mapq3
        PRIVATE $<TARGET_PROPERTY:uilib,INTERFACE_INCLUDE_DIRECTORIES>
        )
