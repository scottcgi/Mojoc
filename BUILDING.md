### BUILDING ANDROID MOJOC PROJECTS FROM LINUX COMMANDLINE

#### 1. Install the required packages

- Java
    ```shell
    $ apt install openjdk-8-jdk
    ```

- Android SDK
    ```shell
    $ snap install androidsdk
    ```

- SDK tools
    ```shell
    $ androidsdk "platform-tools" "platforms;android-28"  
    $ androidsdk "build-tools;30.0.3" "cmake;3.10.2.4988404"
    ```
- NDK
    ```shell
    $ androidsdk "ndk;22.0.7026061"
    ```

- Gradle

    Install [SDKMAN](https://sdkman.io/install)

    ```shell
    $ curl -s "https://get.sdkman.io" | bash
    $ source "$HOME/.sdkman/bin/sdkman-init.sh"
    ```

    Install [Gradle](https://gradle.org/install/)
    ```shell
    $ sdk install gradle 6.8.2
    ```

#### 2. Building the sample project.

- Add `ndkVersion` in build.gradle found in app folder.
    ```gradle
    android {
      ndkVersion "22.0.7026061"
      ...  
    }
    ```

- Run `gladle build` in the Android folder.
    ```shell
    $ gradle build
    ```
