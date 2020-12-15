vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO Ryan-rsm-McKenzie/AutoTOML
    REF ab988e4b6b5f96d9cd5637e2e93ceafe85d2fb10
    SHA512 f11845ed4700109dc61739b2fe2d381bbad76f6b68ad82c1c6e02b7e24eb012d6366d07e75dc36f0a34187b87805729e557e06287fc93170804d4aad04b5800d
    HEAD_REF master
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
)

vcpkg_install_cmake()

vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/AutoTOML)

file(REMOVE_RECURSE
    ${CURRENT_PACKAGES_DIR}/debug
    ${CURRENT_PACKAGES_DIR}/lib
)

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
