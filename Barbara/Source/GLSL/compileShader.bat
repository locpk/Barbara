FOR %%i IN (*.vert) DO %VULKAN_SDK%/Bin32/glslangValidator.exe -V %%i -o %%i.spv
FOR %%i IN (*.frag) DO %VULKAN_SDK%/Bin32/glslangValidator.exe -V %%i -o %%i.spv
