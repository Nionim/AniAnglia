cmake -G Xcode -B build_sim -DCMAKE_TOOLCHAIN_FILE=ios.toolchain.cmake -DPLATFORM=SIMULATOR64 -DARCHS="arm64 x86_64"
cmake -G Xcode -B build_mob -DCMAKE_TOOLCHAIN_FILE=ios.toolchain.cmake -DPLATFORM=OS64

cmake --build build_sim --config Release
cmake --build build_mob --config Release

mkdir libnetsess.xcframework
mkdir libnetsess.xcframework/ios-arm64
mkdir libnetsess.xcframework/ios-arm64_x86_64-simulator
cp build_mob/Release-iphoneos/libnetsess.a libnetsess.xcframework/ios-arm64/libnetsess.a
cp build_sim/Release-iphonesimulator/libnetsess.a libnetsess.xcframework/ios-arm64_x86_64-simulator/libnetsess.a
cp Info.plist libnetsess.xcframework/Info.plist