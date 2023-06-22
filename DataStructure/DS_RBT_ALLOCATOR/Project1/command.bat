@echo off
    cd C:\\Users\user\\AppData\\Roaming\\Com2uS\\Lv1Engine\\Projects
    for /L %%i in (0,1,21) do (
        set foldername = 
        if exist Project_%%i (
           RD /S /Q Project_%%i 
        )
    )