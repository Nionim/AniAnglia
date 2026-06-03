cmake -G Xcode -B build_sim -DCMAKE_TOOLCHAIN_FILE=ios.toolchain.cmake -DPLATFORM=SIMULATOR64 -DARCHS="arm64 x86_64"
cmake -G Xcode -B build_mob -DCMAKE_TOOLCHAIN_FILE=ios.toolchain.cmake -DPLATFORM=OS64

cmake --build build_sim --config Release
cmake --build build_mob --config Release

mkdir libanixart.xcframework
mkdir libanixart.xcframework/ios-arm64
mkdir libanixart.xcframework/ios-arm64_x86_64-simulator
cp build_mob/Release-iphoneos/libanixart.a libanixart.xcframework/ios-arm64/libanixart.a
cp build_sim/Release-iphonesimulator/libanixart.a libanixart.xcframework/ios-arm64_x86_64-simulator/libanixart.a
cp Info.plist libanixart.xcframework/Info.plist