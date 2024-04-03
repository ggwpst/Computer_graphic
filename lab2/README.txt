windows

開啟cmd -> 找到執行檔路徑 -> 輸入執行檔.exe 檔名.in

註解(#)
出現#時，不須執行該行的內容

reset
清除transform_matrix

translate
輸入的指令為 translate x y
x 代表沿著x軸移動x單位
y 代表沿著y軸移動y單位

scale
輸入的指令為 scale x y
x 代表沿著x軸縮放x單位
y 代表沿著y軸縮放y單位

rotate
輸入的指令為 rotate θ
θ代表對著z軸旋轉θ度

clearData
清除所創建的物件(square、triangle)

clearScreen
清除畫面

view
輸入的指令為 view wxl wxr wyb wyt vxl vxr vyb vyt
wxl wxr wyb wyt所代表的是映射前的位置
vxl vxr vyb vyt為映射後的位置
system("pause")按下任意鍵後繼續執行

square
創建一個頂點為 (-1,-1) (1,-1) (1,1) (-1,1) 的正方形

triangle
創建一個頂點為 (0,1) (-1,-1) (1,-1) 的三角形

end
system("pause")按下任意鍵結束視窗