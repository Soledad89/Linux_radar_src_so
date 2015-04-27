datapool.h 主要用来给其他函数提供数据表，比如：x近区地址表，y近区地址表，x远区地址表，y远区地址表
还有就是随机余辉表 以及我们要绘图的地址ppi，win以及AR等，还有就是对应这三种的层表

它会把这些数据传递给 ppi，yh，painterpool等函数文件中


ppi主要是完成PPI的显示，各种显示模式，注意小窗口的显示是和PPI密切联系的，所以ppi内部有两个绘图的地址
一个是ppi的绘图地址，一个就是小窗口的绘图地址

数据扔到一个数据缓存区

最终绘图采用的是 			ppi->ppiDraw(dispdat);
				ar ->ARDraw(dispdat);



