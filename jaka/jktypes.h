#ifndef _JHTYPES_H_
#define _JHTYPES_H_

#define TRUE 1
#define FALSE 0

typedef int BOOL;		   //布尔类型
typedef int JKHD;          //机械臂控制句柄类型
typedef int errno_t;       //接口返回值类型

/**
* @brief 笛卡尔空间位置数据类型
*/
typedef struct
{
	double x;		///< x轴坐标，单位mm
	double y;		///< y轴坐标，单位mm
	double z;		///< z轴坐标，单位mm
}CartesianTran;

/**
* @brief 欧拉角姿态数据类型
*/
typedef struct
{
	double rx;	///< 绕固定轴X旋转角度，单位：rad
	double ry;	///< 绕固定轴Y旋转角度，单位：rad
	double rz;	///< 绕固定轴Z旋转角度，单位：rad
}Rpy;

/**
* @brief 四元数姿态数据类型
*/
typedef struct
{
	double s;
	double x;
	double y;
	double z;
}Quaternion;

/**
 *@brief 笛卡尔空间位姿类型
 */
typedef struct
{
	CartesianTran tran;  	///< 笛卡尔空间位置
	Rpy rpy;        		///< 笛卡尔空间姿态
}CartesianPose;

/**
* @brief 旋转矩阵数据类型
*/
typedef struct
{
	CartesianTran x;  ///< x轴列分量
	CartesianTran y;  ///< y轴列分量
	CartesianTran z; ///< z轴列分量
}RotMatrix;

/**
* @brief 程序运行状态枚举类型
*/
typedef enum
{
	PROGRAM_IDLE,       ///< 机器人停止运行
	PROGRAM_RUNNING,   	///< 机器人正在运行
	PROGRAM_PAUSED    	///< 机器人暂停	
}ProgramState;

/**
* @brief 坐标系选择枚举类型
*/
typedef enum
{
	COORD_BASE,    	///< 基坐标系
	COORD_JOINT,    ///< 关节空间
	COORD_TOOL    	///< 工具坐标系
}CoordType;

/**
* @brief jog运动模式枚举 
*/
typedef enum
{
	ABS = 0,    	///< 绝对运动
	INCR    	///< 增量运动
}MoveMode;

/**
* @brief 系统监测数据类型
*/
typedef struct
{
	int scbMajorVersion;			///<scb主版本号
	int scbMinorVersion;			///<scb次版本号
	int cabTemperature;				///<控制柜温度	
	double robotAveragePower;		///<控制柜总线平均功率
	double robotAverageCurrent;		///<控制柜总线平均电流
	double instCurrent[6];			///<机器人6个轴的瞬时电流
	double instVoltage[6];			///<机器人6个轴的瞬时电压
	double instTemperature[6];		///<机器人6个轴的瞬时温度
}SystemMonitorData;

/**
* @brief 负载数据类型
*/
typedef struct
{
	double mass;				///<负载质量，单位：kg
	CartesianTran centroid;		///<负载质心, 单位：mm
}PayLoad;

/**
* @brief 关节位置数据类型
*/
typedef struct
{
	double jVal[6];		///< 6关节位置值，单位：rad
}JointValue;

/**
* @brief IO类型枚举
*/
typedef enum
{
	IO_CABINET, 	///< 控制柜面板IO
	IO_TOOL, 		///< 工具IO
	IO_EXTEND		///< 扩展IO
}IOType;

/**
* @brief 机器人状态数据
*/
typedef struct
{
	BOOL estoped;		///< 是否急停
	BOOL poweredOn;		///< 是否打开电源
	BOOL servoEnabled;	///< 是否使能
}RobotState;

/**
* @brief 机器人回调函数指针
*/
typedef void(*CallBackFuncType)(int);

/**
* @brief 机器人力矩前馈数据
*/
typedef struct
{
	double jTorque[6];	///< 是否使能
}TorqueValue;

/**
* @brief 机器人状态监测数据,使用get_robot_status函数更新机器人状态数据
*/
typedef struct
{
	int errcode;								///< 机器人运行出错时错误编号，0为运行正常，其它为运行异常
	int inpos;									///< 机器人运动是否到位标志，0为没有到位，1为运动到位
	int powered_on;								///< 机器人是否上电标志，0为没有上电，1为上电
	int enabled;								///< 机器人是否使能标志，0为没有使能，1为使能
	double rapidrate;							///< 机器人运动倍率
	int protective_stop;						///< 机器人是否检测到碰撞，0为没有检测到碰撞，1为检测到碰撞			
	int dout[65];								///< 机器人控制柜数字输出信号,dout[0]为信号的个数
	int tio_dout[10];							///< 机器人末端工具数字输出信号,tio_dout[0]为信号的个数
	double extio[65];							///< 机器人外部应用数字输出信号,extio[0]为信号的个数
	int din[65];								///< 机器人控制柜数字输入信号,din[0]为信号的个数
	int tio_din[10];							///< 机器人末端工具数字输入信号,tio_din[0]为信号的个数
	double ain[65];								///< 机器人控制柜模拟输入信号,ain[0]为信号的个数
	double tio_ain[10];							///< 机器人末端工具模拟输入信号,tio_ain[0]为信号的个数
	double aout[65];							///< 机器人控制柜模拟输出信号,aout[0]为信号的个数
	unsigned int current_tool_id;				///< 机器人目前使用的工具坐标系id
	double cartesiantran_position[6];			///< 机器人末端所在的笛卡尔空间位置
	double joint_position[6];					///< 机器人关节空间位置
	unsigned int on_soft_limit;					///< 机器人是否处于限位，0为没有触发限位保护，1为触发限位保护
	unsigned int current_user_id;				///< 机器人目前使用的用户坐标系id
	int drag_status;							///< 机器人是否处于拖拽状态，0为没有处于拖拽状态，1为处于拖拽状态
}RobotStatus;

/**
* @brief 机器人错误码数据类型
*/
typedef struct
{
	long code;									///< 错误码编号
	char message[120];							///< 错误码对应提示信息
}ErrorCode;

#endif