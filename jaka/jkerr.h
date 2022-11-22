#ifndef _JHERR_H_
#define _JHERR_H_

#define ERR_SUCC                 0          //成功
#define ERR_INVALID_HANDLER		 -1         //无效的句柄
#define	ERR_INVALID_PARAMETER    -2         //无效的参数
#define ERR_COMMUNICATION_ERR	 -3         //tcp/ip通信错误
#define ERR_KINE_INVERSE_ERR     -4         //逆解失败
#define ERR_EMERGENCY_PRESSED    -5         //急停按钮按下
#define ERR_NOT_POWERED          -6         //没有上电
#define ERR_NOT_ENABLED          -7         //没有使能
#define ERR_DISABLE_SERVOMODE    -8         //没有进入servo模式
#define ERR_NOT_OFF_ENABLE       -9         //没有关闭机器人使能
#define ERR_PROGRAM_IS_RUNNING   -10        //作业程序正在运行
#define ERR_CANNOT_OPEN_FILE     -11        //无法打开文件

#endif
