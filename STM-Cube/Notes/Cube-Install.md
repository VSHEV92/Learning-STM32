# STM Cube Install

------

## CubeIDE Install

1. Download Cube IDE from STM [website](https://www.st.com/en/development-tools/stm32cubeide.html)

2. Unzip archive and run shell script:

   ```
   # unzip 
   unzip <stm32-cube-ide>.zip
   
   # run shell script
   chmod +x <stm32-cube-ide>.sh
   ./<stm32-cube-ide>.sh 
   ```

3. Agree licenses and choose installation directory

   

------

## CubeMX Install

1. Download Cube MX from STM [website](https://www.st.com/en/development-tools/stm32cubemx.html)

2. Unzip archive and run shell script:

   ```
   # unzip 
   unzip <stm32-cube-mx>.zip
   
   # run intalation script
   ./<stm32-cube-mx-setup-script> 
   ```

3. Agree licenses and choose installation directory



------

## CubePackages Install

1. Create packages folder:

   ```
   mkdir -p ~/STM32Cube/Repository
   ```

2. Clone needed repository to packages folder:

   ```
   # Clone repository. 
   # package repo example - STM32CubeF4.git
   # package folder example: STM32Cube_FW_F4_V1.28.3
   git clone --recursive https://github.com/STMicroelectronics/<package-repo> <package-folder>
   
   # Checkout to needed version. For example v1.28.3
   cd <package-folder>
   git checkout --force vX.Y.Z
   
   ```

   

   ------
   
   ## CubeProgrammer Install
   
   1. Download Cube MX from STM [website](https://www.st.com/en/development-tools/stm32cubeprog.html)
   
   2. Unzip archive and run shell script:
   
      ```
      # unzip 
      unzip <stm32-cube-programmer>.zip
      
      # run intalation script
      ./<stm32-cube-programmer-setup-script> 
      ```
   
   3. Agree licenses and choose installation directory
   
   