# 小车改装电路&PCB介绍
- 电路部分主要包含：主控小板，TP4056充电小板，开关小板，L9110s驱动小板，SI2302驱动小板。
- 涉及知识：PWM波控制，红外接收，中断，定时器，电机驱动，步进电机驱动，充电控制等，涉及芯片：主控芯片stm32f103c6t6,充电芯片TP4056,驱动芯片L9110s，Mos管等，都是些入门的器件和知识，初学者练手极佳。
- [【视频教程】套件焊接教程](https://www.bilibili.com/video/BV16s4y1m7GQ/?spm_id_from=333.999.0.0&vd_source=89cad0e1890ff49027d6a9f92e9147a6)
- [【文档教程】改装套件使用教程](https://www.wolai.com/ksv9qGV1nwmhjUrozgD7f1)

### 主控小板
- 基于stm32f103c6t6(性价比高)，48引脚，小小板子可控制通道巨多

<img src="../../docs/control_pcb.png" width="180" height="180"> <img src="../../docs/control.jpg" width="140" height="180">

### TP4056充电小板
- 为小车电池充电，3.7v锂电池通用，type-c充电接口。

<img src="../../docs/charge_pcb.png" width="180" height="180"> <img src="../../docs/charge.jpg" width="140" height="180">

### L9110s驱动小板
- 基于L9110s驱动芯片，控制一路电机正反转，外围电路少，简单实用。

<img src="../../docs/driver_pcb1.png" width="180" height="180"> <img src="../../docs/deriver1.jpg" width="170" height="180">

### SI2302驱动小板
- 基于型号为SI2302的3脚mos管，控制一路通断。

<img src="../../docs/driver_pcb2.png" width="180" height="180"> <img src="../../docs/deriver2.jpg" width="140" height="180">

### 开关小板
- 就是带开关的插线板，充电接口也在这。

<img src="../../docs/off-on_pcb.jpg" width="180" height="180"> <img src="../../docs/off-on.jpg" width="160" height="180">

## 使用方法
- 嘉立创绘制，打开嘉立创eda，导入即可，.epro或.zip格式都可导入

<img src="../../docs/jlceda_input.png" width="280" height="180">


## 量产区up主福利
- 懒得画电路敲代码可淘宝搜“积木研究圆的小车改装店”，有部分现成提供(随缘上架哦！)
- 1:64芯片套件：[https://item.taobao.com/item.htm?ft=t&id=737362891443](https://item.taobao.com/item.htm?ft=t&id=737362891443)
- 1:87改装套件：[https://item.taobao.com/item.htm?ft=t&id=737272654141](https://item.taobao.com/item.htm?ft=t&id=737272654141)

- 量产芯片组套件长这样

<img src="../../docs/套件封面.jpg" width="100" height="100"> <img src="../../docs/套件封面2.jpg" width="120" height="100">