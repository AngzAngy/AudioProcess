#ifndef _INTERFACE_H
#define _INTERFACE_H

#if defined(_MSC_VER)
#include <windows.h>
#define LINKING WINAPI
#else
#define LINKING
#endif


/* 混响参数，请勿更改结构体 */
/* 使用EAX接口标准和命名规则 */
typedef struct __EnvRealizeParameters_
{
	// 以下取值范围请严格限制，程序内部没有安全检查，超出范围可能会引起错误
	float flDensity;  // 密度（0~1.0）
	float flDiffusion;  // 声波传播系数（0~1.0）
	float flGain;  // 增益乘（0~1.0）
	float flGainHF;  // 高频波增益乘（0~1.0）
	float flGainLF;  // 低频波增益乘（0~1.0）
	float flDecayTime;  // 声音衰减时间（0.1~20.0，单位秒）
	float flDecayHFRatio;  // 高频波衰减速率（0~2.0）
	float flDecayLFRatio;  // 低频波衰减速率（0~2.0）
	float flReflectionsGain;  // 反射增益（0~3.16，又称为反射杂散波叠加增益）
	float flReflectionsDelay;  // 反射延迟比例（0~0.3）
	float flReflectionsPan[3];  // 反射偏移量（0~1.0，3表示XYZ，默认为0[表示自动计算]）
	float flLateReverbGain;  // 后混响增益（0~10.0，不宜过大，一般小于0.5）
	float flLateReverbDelay;  // 后混响延迟比例（0~0.1，1.0表示延迟一个反射周期，一般用0)
	float flLateReverbPan[3];  // 后混响偏移量（0~1.0，3表示XYZ，默认为0[表示自动计算]）
	float flEchoTime;  // 回声时间（0.075~0.25）
	float flEchoDepth;  // 回声深度（0~1.0）
	float flModulationTime;  // 杂波调制时间（0.04~4.0）
	float flModulationDepth;  // 杂波调制深度（0~1.0）
	float flAirAbsorptionGainHF;  // 空气对高频波的吸收增益量（0.892~1.0）
	float flHFReference;  // 高频波参考频率线（1000~20000，Hz）
	float flLFReference;  // 低频波参考频率线（20~1000，Hz）
	float flRoomRolloffFactor;  // 房间声波衰减量（0~10）
	int iDecayHFLimit;  // 高频波衰减时是否受高频率参考频率线限制（0表示FALSE，1表示TRUE）
} _EnvRealizeParameters_;


typedef struct AudPostprocess{

}AudPostprocess;
/*
	典型预设的参数：
	房间：
	{
	   0.4287f, 1.0000f, 0.3162f, 0.5929f, 1.0000f, 0.4000f, 0.8300f, 1.0000f,
	   0.1503f, 0.0020f, { 0.0000f, 0.0000f, 0.0000f },
	   1.0629f, 0.0030f, { 0.0000f, 0.0000f, 0.0000f },
	   0.2500f, 0.0000f, 0.2500f, 0.0000f, 0.9943f, 5000.0000f, 250.0000f, 0.0000f, TRUE
	}
	大礼堂：
	{
		1.0000f, 1.0000f, 0.3162f, 0.5781f, 1.0000f, 4.3200f, 0.5900f, 1.0000f,
		0.4032f, 0.0200f, { 0.0000f, 0.0000f, 0.0000f },
		0.7170f, 0.0300f, { 0.0000f, 0.0000f, 0.0000f },
		0.2500f, 0.0000f, 0.2500f, 0.0000f, 0.9943f, 5000.0000f, 250.0000f, 0.0000f, TRUE
	}
	音乐厅：
	{
		1.0000f, 1.0000f, 0.3162f, 0.5623f, 1.0000f, 3.9200f, 0.7000f, 1.0000f,
		0.2427f, 0.0200f, { 0.0000f, 0.0000f, 0.0000f },
		0.9977f, 0.0290f, { 0.0000f, 0.0000f, 0.0000f },
		0.2500f, 0.0000f, 0.2500f, 0.0000f, 0.9943f, 5000.0000f, 250.0000f, 0.0000f, TRUE
	}
*/


#ifdef __cplusplus
extern "C"
{
#endif

AudPostprocess* LINKING CreateVAPInstance();
void LINKING FreeVAPInstance(AudPostprocess *pInstance);
int LINKING SetSamplingRate(int nSamplingRate, AudPostprocess *pInstance);
int LINKING SetChannels(int nChannels, AudPostprocess *pInstance);
void LINKING EnableComponent(int nID, int bEnabled, AudPostprocess *pInstance);
void LINKING SetBGAGCParameters(float rRatio, float rVolume, float rMaxGain, AudPostprocess *pInstance);
void LINKING SetBGSurParameters(float rStereoWeight, float rImageWeight, AudPostprocess *pInstance);
void LINKING SetBGReverbPreset(const int nPresetID, float rDry, float rWet, AudPostprocess *pInstance);
void LINKING SetVocalFixParameters(int nBandFreqLimit, AudPostprocess *pInstance);
void LINKING SetVocalHarmonicParameters(float fvHarMag[10], AudPostprocess *pInstance);
void LINKING SetVocalAGCParameters(float rRatio, float rVolume, float rMaxGain, AudPostprocess *pInstance);
void LINKING SetVocalReverbPreset(const int nPresetID, float rDry, float rWet, AudPostprocess *pInstance);
void LINKING LoadPreset(const int nPresetID, AudPostprocess *pInstance);
void LINKING Reset(AudPostprocess *pInstance);
int LINKING ProcessBackground_s16(short *s16Buffer, int nFrameCount, AudPostprocess *pInstance);
int LINKING ProcessBackground_f32(float *f32Buffer, int nFrameCount, AudPostprocess *pInstance);
int LINKING ProcessVocal_s16(short *s16Buffer, int nFrameCount, AudPostprocess *pInstance);
int LINKING ProcessVocal_f32(float *f32Buffer, int nFrameCount, AudPostprocess *pInstance);

/**** 新增API ****/
// 获取当前的伴奏混响设置参数，返回0表示成功，-1表示失败
int LINKING GetBGReverbParameters(_EnvRealizeParameters_ *pParameters, AudPostprocess *pInstance);
// 设定伴奏混响参数
void LINKING SetBGReverbParameters(_EnvRealizeParameters_ *pParameters, AudPostprocess *pInstance);
// 获取当前的人声混响设置参数，返回0表示成功，-1表示失败
int LINKING GetVocalReverbParameters(_EnvRealizeParameters_ *pParameters, AudPostprocess *pInstance);
// 设定人声混响参数
void LINKING SetVocalReverbParameters(_EnvRealizeParameters_ *pParameters, AudPostprocess *pInstance);
//// 建议使用方法
// 1、先使用LoadPreset或SetXXXReverbPreset来加载一个预设
// 2、使用Get方法取得设置参数
// 3、调整参数并使用Set方法设置进去
/*****************/

#ifdef __cplusplus
}
#endif

#endif
