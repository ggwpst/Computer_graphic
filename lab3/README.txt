windows

開啟cmd -> 找到執行檔路徑 -> 輸入執行檔.exe 檔名.in

註解(#)
出現#時，不執行該行的內容

開頭
開頭兩個數字，分別是window的width和height

reset
清除transform_matrix

translate
輸入的指令為 translate x y z
x 代表沿著x軸移動x單位
y 代表沿著y軸移動y單位
z 代表沿著z軸移動z單位

scale
輸入的指令為 scale x y z
x 代表沿著x軸縮放x單位
y 代表沿著y軸縮放y單位
z 代表沿著z軸縮放z單位

rotate
輸入的指令為 rotate x y z
x 代表沿著x軸旋轉x 度
y 代表沿著y軸旋轉y 度
z 代表沿著z軸旋轉z 度
先對y 軸旋轉，再來是 z 軸，最後 x軸。

viewport
輸入的指令為 view vxl vxr vyb vyt
在view外面的圖形做剪裁
在 display 才畫圖

object
指令為 object obj

object 後面接的是檔案名稱，將圖形切成三角形(此次檔案只有四邊形及三角形)。

observer
指令為 observer epx epy epz COIx COIy COIz Tilt Hither Yon Hav
設定攝影機的位置
epx epy epz 為攝影機的 x y z 位置
COIx COIy COIz 為攝影機看的點
Hither 為 near 的平面
Yon 為 far 的平面
Hav 為 FOV 即 field of view

display
在做display的時候就做一次clearScreen
每一次display東西出來時，加上system("pause");

end
結束視窗