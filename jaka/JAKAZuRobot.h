#ifndef _JAKAAPI_H_
#define _JAKAAPI_H_

#include "jkerr.h"
#include "jktypes.h"
#include <stdio.h>
#include <string>

#ifdef DLLEXPORT_EXPORTS
#define DLLEXPORT_API __declspec(dllexport)
#else
#define DLLEXPORT_API __declspec(dllimport)
#endif

class JAKAZuRobot
{
public:
	/**
	* @brief 机械臂控制类构造函数
	*/
	JAKAZuRobot();

	/**
	* @brief 创建机器人控制句柄
	* @param ip  控制器ip地址
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  login_in(const char* ip);

	/**
	* @brief 断开控制器连接
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  login_out();

	/**
	* @param handle  机器人控制句柄
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  power_on();

	/**
	* @brief 关闭机器人电源
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  power_off();

	/**
	* @brief 机器人控制柜关机
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  shut_down();

	/**
	* @brief 控制机器人上使能
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  enable_robot();

	/**
	* @brief 控制机器人下使能
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  disable_robot();

	/**
	* @brief 控制机器人手动模式下运动
	* @param aj_num 1_based标识值，在关节空间下代表关节号0-5，笛卡尔下依次为x，y，z，rx，ry，rz
	* @param move_mode 机器人运动模式，增量运动或者绝对运动
	* @param coord_type 机器人运动坐标系，工具坐标系，基坐标系（当前的世界/用户坐标系）或关节空间
	* @param vel_cmd 指令速度，旋转轴或关节运动单位为rad/s，移动轴单位为mm/s
	* @param pos_cmd 指令位置，旋转轴或关节运动单位为rad，移动轴单位为mm
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  jog(int aj_num, MoveMode move_mode, CoordType coord_type, double vel_cmd, double pos_cmd);

	/**
	* @brief 控制机器人手动模式下运动停止
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  jog_stop(int num);

	/**
	* @brief 机器人关节运动
	* @param joint_pos 机器人关节运动目标位置
	* @move_mode 指定运动模式：增量运动或绝对运动
	* @param is_block 设置接口是否为阻塞接口，TRUE为阻塞接口 FALSE为非阻塞接口
	* @param speed 机器人关节运动速度，单位：rad/s
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  joint_move(const JointValue* joint_pos, MoveMode move_mode, BOOL is_block, double speed);

	/**
	* @brief 机器人末端直线运动
	* @param end_pos 机器人末端运动目标位置
	* @param is_block 设置接口是否为阻塞接口，TRUE 为阻塞接口 FALSE 为非阻塞接口
	* @param speed 机器人直线运动速度，单位：mm/s
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  linear_move(const CartesianPose* end_pos, MoveMode move_mode, BOOL is_block, double speed);

	/**
	* @brief 机器人SERVO MOVE模式使能
	* @param enable  TRUE为进入SERVO MOVE模式，FALSE表示退出该模式
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  servo_move_enable(BOOL enable);

	/**
	* @brief 机器人关节空间位置控制模式
	* @param joint_pos 机器人关节运动目标位置
	* @move_mode 指定运动模式：增量运动或绝对运动
	* @return ERR_SUCC成功 其他失败
	*/
	errno_t  servo_j(const JointValue* joint_pos, MoveMode move_mode);

	/**
	* @brief 机器人笛卡尔空间位置控制模式
	* @param cartesian_pose 机器人笛卡尔空间运动目标位置
	* @move_mode 指定运动模式：增量运动或绝对运动
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  servo_p(const CartesianPose* cartesian_pose, MoveMode move_mode);

	/**
	* @brief 设置数字输出变量(DO)的值
	* @param type DO类型
	* @param index DO索引
	* @param value DO设置值
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  set_digital_output(IOType type, int index, BOOL value);

	/**
	* @brief 设置模拟输出变量的值(AO)的值
	* @param type AO类型
	* @param index AO索引
	* @param value AO设置值
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  set_analog_output(IOType type, int index, float value);

	/**
	* @brief 查询数字输入(DI)状态
	* @param type DI类型
	* @param index DI索引
	* @param result DI状态查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_digital_input(IOType type, int index, BOOL* result);

	/**
	* @brief 查询数字输出(DO)状态
	* @param type DO类型
	* @param index DO索引
	* @param result DO状态查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_digital_output(IOType type, int index, BOOL* result);

	/**
	* @brief 获取模拟量输入变量(AI)的值
	* @param type AI的类型
	* @param index AI索引
	* @param result 指定AI状态查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_analog_input(IOType type, int index, float* result);

	/**
	* @brief 获取模拟量输出变量(AO)的值
	* @param type AO的类型
	* @param index AO索引
	* @param result 指定AO状态查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_analog_output(IOType type, int index, float* result);

	/**
	* @brief 查询扩展IO模块是否运行
	* @param is_running 扩展IO模块运行状态查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  is_extio_running(BOOL* is_running);

	/**
	* @brief 运行当前加载的作业程序
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  program_run();

	/**
	* @brief 暂停当前运行的作业程序
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  program_pause();

	/**
	* @brief 继续运行当前暂停的作业程序
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  program_resume();

	/**
	* @brief 终止当前执行的作业程序
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  program_abort();

	/**
	* @brief 加载指定的作业程序
	* @param file 程序文件路径
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  program_load(const char* file);

	/**
	* @brief 获取已加载的作业程序名字
	* @param file 程序文件路径
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_loaded_program(char* file);

	/**
	* @brief 获取当前机器人作业程序的执行行号
	* @param curr_line 当前行号查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_current_line(int* curr_line);

	/**
	* @brief 获取机器人作业程序执行状态
	* @param status 作业程序执行状态查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_program_state(ProgramState* status);

	/**
	* @brief 设置机器人运行倍率
	* @param rapid_rate 是程序运行倍率，设置范围为[0,1]
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  set_rapidrate(double rapid_rate);

	/**
	* @brief 获取机器人运行倍率
	* @param rapid_rate 当前控制系统倍率
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_rapidrate(double* rapid_rate);

	/**
	* @brief 设置指定编号的工具信息
	* @param id 工具编号
	* @param tcp 工具坐标系相对法兰坐标系偏置
	* @param name 指定工具的别名
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  set_tool_data(int id, const CartesianPose* tcp, const char* name);

	/**
	* @brief 设置当前使用的工具ID
	* @param id 工具坐标系ID
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  set_tool_id(const int id);

	/**
	* @brief 查询当前使用的工具ID
	* @param id 工具ID查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_tool_id(int* id);

	/**
	* @brief 设置指定编号的用户坐标系信息
	* @param id 用户坐标系编号
	* @param user_frame 用户坐标系偏置值
	* @param name 用户坐标系别名
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  set_user_frame_data(int id, const CartesianPose* user_frame, const char* name);

	/**
	* @brief 设置当前使用的用户坐标系ID
	* @param id 用户坐标系ID
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  set_user_frame_id(const int id);

	/**
	* @brief 查询当前使用的用户坐标系ID
	* @param id 获取的结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_user_frame_id(int* id);

	/**
	* @brief 控制机器人进入或退出拖拽模式
	* @param enable  TRUE为进入拖拽模式，FALSE为退出拖拽模式
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  drag_mode_enable(BOOL enable);

	/**
	* @brief 查询机器人是否处于拖拽模式
	* @param in_drag 查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  is_in_drag_mode(BOOL* in_drag);

	/**
	* @brief 获取机器人状态
	* @param state 机器人状态查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_robot_state(RobotState* state);

	/**
	* @brief 获取当前设置下工具末端的位姿
	* @param tcp_position 工具末端位置查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_tcp_position(CartesianPose* tcp_position);

	/**
	* @brief 获取当前机器人关节角度
	* @param joint_position 关节角度查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_joint_position(JointValue* joint_position);

	/**
	* @brief 查询机器人是否处于碰撞保护模式
	* @param in_collision 查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  is_in_collision(BOOL* in_collision);

	/**
	* @brief 查询机器人是否超出限位
	* @param on_limit 查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  is_on_limit(BOOL* on_limit);

	/**
	* @brief 查询机器人运动是否停止
	* @param in_pos 查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  is_in_pos(BOOL* in_pos);

	/**
	* @brief 碰撞之后从碰撞保护模式恢复
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  collision_recover();

	/**
	* @brief  错误状态清除
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  clear_error();

	/**
	* @brief 设置机器人碰撞等级
	* @param level  碰撞等级，等级0-5 ，0为关闭碰撞，1为碰撞阈值25N，2为碰撞阈值50N，3为碰撞阈值75N，4为碰撞阈值100N，5为碰撞阈值125N
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t set_collision_level(const int level);

	/**
	* @brief 获取机器人设置的碰撞等级
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t get_collision_level(int* level);

	/**
	* @brief 计算指定位姿在当前工具、当前安装角度以及当前用户坐标系设置下的逆解
	* @param ref_pos 逆解计算用的参考关节空间位置
	* @param cartesian_pose 笛卡尔空间位姿值
	* @param joint_pos 计算成功时关节空间位置计算结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  kine_inverse(const JointValue* ref_pos, const CartesianPose* cartesian_pose, JointValue* joint_pos);

	/**
	* @brief 计算指定关节位置在当前工具、当前安装角度以及当前用户坐标系设置下的位姿值
	* @param joint_pos 关节空间位置
	* @param cartesian_pose 笛卡尔空间位姿计算结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  kine_forward(const JointValue* joint_pos, CartesianPose* cartesian_pose);

	/**
	* @brief 欧拉角到旋转矩阵的转换
	* @param rpy 待转换的欧拉角数据
	* @param rot_matrix 转换后的旋转矩阵
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  rpy_to_rot_matrix(const Rpy* rpy, RotMatrix* rot_matrix);

	/**
	* @brief 旋转矩阵到欧拉角的转换
	* @param rot_matrix 待转换的旋转矩阵数据
	* @param rpy 转换后的RPY欧拉角结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  rot_matrix_to_rpy(const RotMatrix* rot_matrix, Rpy* rpy);

	/**
	* @brief 四元数到旋转矩阵的转换
	* @param quaternion 待转换的四元数数据
	* @param rot_matrix 转换后的旋转矩阵结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t quaternion_to_rot_matrix(const Quaternion* quaternion, RotMatrix* rot_matrix);

	/**
	* @brief 旋转矩阵到四元数的转换
	* @param rot_matrix 待转换的旋转矩阵
	* @param quaternion 转换后的四元数结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  rot_matrix_to_quaternion(const RotMatrix* rot_matrix, Quaternion* quaternion);

	/**
	* @brief 注册机器人出现错误时的回调函数
	* @param func 指向用户定义的函数的函数指针
	* @param error_code 机器人的错误码
	*/
	errno_t set_error_handler(CallBackFuncType func);

	/**
	* @brief 机器人力矩控制模式使能
	* @param enable  TRUE为进入SERVO MOVE模式，FALSE表示退出该模式
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t torque_control_enable(BOOL enable);

	/**
	* @brief 机器人力矩前馈功能
	* @param tor_val  用于力矩前馈的各个关节的力矩值
	* @param grv_flag  0代表选用控制器自带的力矩前馈算法，1代表设置关节力矩控制的偏置，2代表关节控制的力矩完全由用户控制
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t torque_feedforward(TorqueValue tor_val, int grv_flag);

	/**
	* @brief 机器人负载设置
	* @param payload 负载质心、质量数据
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  set_payload(const PayLoad* payload);

	/**
	* @brief 获取机器人负载数据
	* @param payload 负载查询结果
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_payload(PayLoad* payload);

	/**
	* @brief 获取SDK版本号
	* @param version SDK版本号
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_sdk_version(char* version);

	/**
	* @brief 获取控制器IP
	* @param controller_name 控制器名字
	* @param ip_list 控制器ip列表，控制器名字为具体值时返回该名字所对应的控制器IP地址，控制器名字为空时，返回网段类内的所有控制器IP地址
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_controller_ip(char* controller_name, char* ip_list);

	/**
	* @brief 获取机器人状态数据
	* @param status 机器人状态
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_robot_status(RobotStatus* status);

	/**
	* @brief 终止当前机械臂运动
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  motion_abort();

	/**
	* @brief 设置错误码文件路径，需要使用get_last_error接口时需要设置错误码文件路径，如果不使用get_last_error接口，则不需要设置该接口
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t set_errorcode_file_path(char* path);

	/**
	* @brief 获取机器人运行过程中最后一个错误码,当调用clear_error时，最后一个错误码会清零
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  get_last_error(ErrorCode* code);

	/**
	* @brief 设置是否开启调试模式，选择TRUE时，开始调试模式，此时会在标准输出流中输出调试信息，选择FALSE时，不输出调试信息
	* @return ERR_SUCC 成功 其他失败
	*/
	errno_t  set_debug_mode(BOOL mode);

	~JAKAZuRobot();
private:
	class BIFClass;
	BIFClass* ptr;
};

#endif
