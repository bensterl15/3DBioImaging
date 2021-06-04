for /l %%I in (1, 1, 5) do for /l %%X in (1, 1, 100) do (
move masks\T%%I\MaskZ%%X_T%%I.tif masks\T%%I\%%X.tif
move mrcnn_pred\images\T%%I\RawZ%%X_T%%I.tif mrcnn_pred\images\T%%I\%%X.tif
move mrcnn_pred\masks\T%%I\RawZ%%X_T%%I.tif mrcnn_pred\masks\T%%I\%%X.tif
)