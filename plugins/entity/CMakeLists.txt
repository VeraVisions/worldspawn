radiant_plugin(entity
        angle.h
        angles.h
        colour.h
        curve.h
        eclassmodel.cpp eclassmodel.h
        entity.cpp entity.h
        filters.cpp filters.h
        generic.cpp generic.h
        group.cpp group.h
        keyobservers.h
        light.cpp light.h
        miscmodel.cpp miscmodel.h
        prop_dynamic.cpp prop_dynamic.h
        model.h
        modelskinkey.h
        namedentity.h
        namekeys.h
        origin.h
        plugin.cpp
        rotation.h
        scale.h
        skincache.cpp skincache.h
        targetable.cpp targetable.h
        )

target_include_directories(entity
        PRIVATE $<TARGET_PROPERTY:uilib,INTERFACE_INCLUDE_DIRECTORIES>
        )
