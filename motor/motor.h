/*
 * General Atomic Force Microscope (AFM) - Header of Motor
 * ===========================================================
 * Date: 09 June 2014
 * Co-Author: Liwen Zhang
 * Email: Scott.zhangliwen@gmail.com/chadkidzhang@126.com
 * ===========================================================
 * 该头文件定义了AFM中步进电机接口
 */

void motor_origin_set();
void motor_auto_forward();
void motor_auto_backward();
void motor_stop();
void motor_step_forward(int step);
void motor_step_backward(int step);
void motor_originate();
