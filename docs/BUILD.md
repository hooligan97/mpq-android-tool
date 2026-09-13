# Building MPQ Android Tool

## Prerequisites

### Required Software
- **Android Studio** 4.0 or later
- **Android NDK** r21 or later
- **CMake** 3.10 or later
- **Gradle** 6.7 or later
- **Java Development Kit (JDK)** 11 or later

### Required Android SDK
- **Minimum SDK**: Android 5.0 (API 21)
- **Target SDK**: Android 13 (API 33)
- **Build Tools**: 33.0.0 or later

### Required System Resources
- **Disk Space**: ~5GB (for SDK, NDK, build artifacts)
- **RAM**: 8GB recommended (for compilation)
- **Network**: Required for gradle/dependency downloads

## Setup Instructions

### 1. Install Android Studio

1. Download from https://developer.android.com/studio
2. Run the installer and follow instructions
3. Launch Android Studio
4. Open SDK Manager (Tools → SDK Manager)
5. Install the required SDK components:
   - Android SDK Build-Tools 33
   - Android SDK Platform 33
   - Android NDK (Side by side) - r21 or later
   - CMake 3.22.1

### 2. Configure NDK Path

**Option A: Use Android Studio (Recommended)**
1. Open SDK Manager
2. Go to SDK Tools tab
3. Check "Show Package Details"
4. Install NDK r21+ (Side by side)
5. Android Studio automatically configures `ndk.dir` in `local.properties`

**Option B: Manual Configuration**
1. Create `local.properties` in project root
2. Add the following lines:
```properties
sdk.dir=/path/to/Android/sdk
ndk.dir=/path/to/Android/ndk/r21
```

### 3. Clone Repository

```bash
git clone https://github.com/hooligan97/mpq-android-tool.git
cd mpq-android-tool
```

### 4. Verify Gradle Configuration

```bash
./gradlew --version
```

Expected output:
```
Gradle 7.x.x
...
JVM: 11.x.x
```

## Building

### Build Variants

#### Debug Build (All Architectures)
```bash
./gradlew assembleDebug
```

Output: `app/build/outputs/apk/debug/app-debug.apk`

#### Debug Build (Specific Architecture)
```bash
# armeabi-v7a only
./gradlew assembleDebug -PNDK_ARCHITECTURES=armeabi-v7a

# armeabi only
./gradlew assembleDebug -PNDK_ARCHITECTURES=armeabi

# x86 only
./gradlew assembleDebug -PNDK_ARCHITECTURES=x86

# Multiple
./gradlew assembleDebug -PNDK_ARCHITECTURES=armeabi-v7a,x86
```

#### Release Build
```bash
./gradlew assembleRelease
```

Output: `app/build/outputs/apk/release/app-release.apk`

#### Install on Device
```bash
./gradlew installDebug
```

### Build Configuration

Edit `app/build.gradle` to modify:

```gradle
defaultConfig {
    applicationId "com.mpq.android.tool"     // App package name
    minSdk 21                                 // Minimum Android version
    targetSdk 33                              // Target Android version
    versionCode 1                             // Build version
    versionName "1.0.0"                       // App version
    
    externalNativeBuild {
        cmake {
            abiFilters 'armeabi-v7a', 'armeabi', 'x86'  // Architectures
        }
    }
}
```

## Compilation Process

### Step-by-Step Build Flow

1. **Gradle Initialization**
   ```bash
   gradle init
   ```
   - Loads `build.gradle` and `settings.gradle`
   - Configures modules (app, mpq-lib)
   - Initializes NDK/CMake paths

2. **Java Compilation**
   ```bash
   gradle compileDebugKotlin
   ```
   - Compiles Kotlin source files
   - Generates R.java for resources
   - Creates class files

3. **C++ Compilation (via CMake)**
   ```bash
   cmake . -DANDROID_ABI=armeabi-v7a
   make
   ```
   - Processes `CMakeLists.txt`
   - Compiles C++ source files
   - Creates native `.so` libraries

4. **APK Packaging**
   ```bash
   gradle packageDebug
   ```
   - Combines resources, classes, and native libraries
   - Creates APK archive
   - Signs with debug key

5. **Installation**
   ```bash
   adb install app-debug.apk
   ```
   - Transfers APK to device
   - Installs application

## Troubleshooting

### Error: NDK not found

**Solution:**
```bash
# Check local.properties
cat local.properties

# Or set NDK path manually
echo "ndk.dir=/path/to/ndk" >> local.properties
```

### Error: CMake version mismatch

**Solution:**
```bash
# Update CMake version in build.gradle
externalNativeBuild {
    cmake {
        version "3.22.1"  # Update to your installed version
    }
}
```

### Error: Out of memory during compilation

**Solution:**
```bash
# Increase Gradle heap size
export GRADLE_OPTS="-Xmx4g"
./gradlew assembleDebug
```

### Error: ABI mismatch

**Solution:**
```bash
# Clean build
./gradlew clean

# Rebuild with specific ABI
./gradlew assembleDebug -PNDK_ARCHITECTURES=armeabi-v7a
```

### Error: Slow compilation

**Solution:**
1. Use parallel compilation:
   ```bash
   ./gradlew assembleDebug --parallel --max-workers=4
   ```

2. Enable gradle daemon:
   ```bash
   echo "org.gradle.daemon=true" >> gradle.properties
   ```

3. Increase gradle memory:
   ```bash
   echo "org.gradle.jvmargs=-Xmx4g" >> gradle.properties
   ```

## Building Native Libraries Only

### Standalone CMake Build

```bash
cd mpq-lib/src/main/cpp
mkdir build && cd build

# Configure for armeabi-v7a
cmake .. -DANDROID_ABI=armeabi-v7a \
         -DANDROID_PLATFORM=android-21 \
         -DCMAKE_TOOLCHAIN_FILE=$NDK_PATH/build/cmake/android.toolchain.cmake

# Compile
make

# Output: libmpq-core.so
ls lib/
```

### Build All Architectures

```bash
#!/bin/bash
NDK_PATH="/path/to/ndk"
OUT_DIR="./build_output"

for ABI in armeabi-v7a armeabi x86; do
    mkdir -p "$OUT_DIR/$ABI"
    cd "$OUT_DIR/$ABI"
    
    cmake ../.. -DANDROID_ABI=$ABI \
           -DANDROID_PLATFORM=android-21 \
           -DCMAKE_TOOLCHAIN_FILE=$NDK_PATH/build/cmake/android.toolchain.cmake
    
    make
    cd ../../
done
```

## CI/CD Integration

### GitHub Actions Example

Create `.github/workflows/android-build.yml`:

```yaml
name: Android Build

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      
      - name: Set up JDK 11
        uses: actions/setup-java@v2
        with:
          java-version: '11'
          distribution: 'temurin'
      
      - name: Setup Android NDK
        run: |
          $ANDROID_HOME/cmdline-tools/latest/bin/sdkmanager "ndk;21.4.7075529"
      
      - name: Build with Gradle
        run: ./gradlew assembleDebug
      
      - name: Upload APK
        uses: actions/upload-artifact@v2
        with:
          name: app-debug
          path: app/build/outputs/apk/debug/
```

## Next Steps

1. **Build the project**: `./gradlew assembleDebug`
2. **Install on device**: `./gradlew installDebug`
3. **Run on device**: See [Running](#running) section
4. **Develop**: Edit source files in `app/src/main/` and `mpq-lib/src/main/cpp/`

## Performance Optimization

### Parallel Compilation
```bash
./gradlew assembleDebug --parallel
```

### Incremental Builds
```bash
./gradlew assembleDebug --build-cache
```

### Using Gradle Daemon
```gradle
# gradle.properties
org.gradle.daemon=true
org.gradle.parallel=true
org.gradle.workers.max=4
```

## Additional Resources

- [Android Developer Guide](https://developer.android.com/)
- [Android NDK Documentation](https://developer.android.com/ndk)
- [CMake Android Documentation](https://developer.android.com/ndk/guides/cmake)
- [Gradle Documentation](https://docs.gradle.org/)
